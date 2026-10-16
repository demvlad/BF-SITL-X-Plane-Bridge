#include "BetaflightLink.h"

CBetaflightLink::CBetaflightLink(const CXmlSetup *pXmlSetup): sendto_fc_state_sock(INVALID_SOCKET), sendto_fc_rc_sock(INVALID_SOCKET),
                                                              recv_fc_motor_output_sock(INVALID_SOCKET), recv_fc_pwm_output_sock(INVALID_SOCKET),
                                                              isConnected(false),
                                                              m_pSetupRC((TSetupRC *const)&pXmlSetup->SetupRC)
{
    sitl_ip_address = pXmlSetup->sitl_ip_address;

    memset(&bfOutFlightState, 0, sizeof(bfOutFlightState));
    memset(&bfOutCommandRC, 0, sizeof(bfOutCommandRC));
    memset(&bfInMotors, 0, sizeof(bfInMotors));
    memset(&bfInServos, 0, sizeof(bfInServos));
    memset(&dest, 0, sizeof(dest));

    bfOutFlightState.imu_orientation_quat[0] = 1.0;
    bfOutFlightState.imu_linear_acceleration_xyz[2] = 9.81;
    bfOutFlightState.pressure = 101325.0;

    for (int ch = 0; ch < SIMULATOR_MAX_RC_CHANNELS; ch++) {
        bfOutCommandRC.channels[ch] = UNDEFINED_PWM_PULSE;
    }
}

bool CBetaflightLink::Initialize()
{
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(1, 1), &WSAData) != 0) {
        return false;
    }

    sendto_fc_state_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sendto_fc_rc_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    recv_fc_motor_output_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    recv_fc_pwm_output_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    u_long iMode = 1;
    ioctlsocket(recv_fc_motor_output_sock, FIONBIO, &iMode);
    ioctlsocket(recv_fc_pwm_output_sock, FIONBIO, &iMode);

    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;

    local.sin_port = htons((uint16_t)PORT_PWM_RAW);
    bind(recv_fc_pwm_output_sock, (sockaddr*)&local, sizeof(local));

    local.sin_port = htons((uint16_t)PORT_PWM);
    bind(recv_fc_motor_output_sock, (sockaddr*)&local, sizeof(local));

    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(sitl_ip_address.empty() ? "0.0.0.0" : sitl_ip_address.c_str());

    isConnected = false;

    return true;
}

void CBetaflightLink::Terminate()
{
    if (sendto_fc_state_sock != INVALID_SOCKET) {
        closesocket(sendto_fc_state_sock);
    }

    if (sendto_fc_rc_sock != INVALID_SOCKET) {
        closesocket(sendto_fc_rc_sock);
    }

    if (recv_fc_motor_output_sock != INVALID_SOCKET) {
        closesocket(recv_fc_motor_output_sock);
    }

    if (recv_fc_pwm_output_sock != INVALID_SOCKET) {
        closesocket(recv_fc_pwm_output_sock);
    }

    WSACleanup();
}

void CBetaflightLink::Restart()
{
    Terminate();
    Initialize();
}

CBetaflightLink::~CBetaflightLink(void)
{
    Terminate();
}

bool CBetaflightLink::sendFlightState()
{
    if (isConnected) {
        dest.sin_port = htons((uint16_t)PORT_STATE);
        sendto(sendto_fc_state_sock, (const char*)&bfOutFlightState, sizeof(bfOutFlightState), 0, (sockaddr*)&dest, sizeof(dest));

        dest.sin_port = htons((uint16_t)PORT_RC);
        sendto(sendto_fc_rc_sock, (const char*)&bfOutCommandRC, sizeof(bfOutCommandRC), 0, (sockaddr*)&dest, sizeof(dest));
        return true;
    }

    return false;
}

bool CBetaflightLink::receiveMotorCommands()
{
    struct sockaddr_in from;
    int fromlen = sizeof(from);
    char buf[1024];
    bool data_readed = false;
    int res;

    res = recvfrom(recv_fc_pwm_output_sock, buf, sizeof(buf), 0, (sockaddr*)&from, &fromlen);
    if (res != -1) {
        if (!isConnected) {
            if (sitl_ip_address.empty()) {
                dest.sin_addr.S_un.S_addr = from.sin_addr.S_un.S_addr;
            }
            isConnected = true;
        }
        memcpy(&bfInServos, buf, sizeof(bfInServos));
        data_readed = true;
    }

    res = recvfrom(recv_fc_motor_output_sock, buf, sizeof(buf), 0, (sockaddr*)&from, &fromlen);
    if (res != -1) {
        if (!isConnected) {
            if (sitl_ip_address.empty()) {
                dest.sin_addr.S_un.S_addr = from.sin_addr.S_un.S_addr;
            }
            isConnected = true;
        }
        memcpy(&bfInMotors, buf, sizeof(bfInMotors));
        data_readed = true;
    }
    return data_readed;
}

void CBetaflightLink::setMode(std::string modeName)
{
    if (m_pSetupRC->Modes.count(modeName) == 0 ) {
        return;
    }

    const TModeRC& mode = m_pSetupRC->Modes[modeName];
    bfOutCommandRC.channels[mode.aux] = mode.value;
}

void CBetaflightLink::setThrottle(double throttle)
{
    const TControlRC& throttleSetup = m_pSetupRC->Controls["throttle"];
    if (throttleSetup.isEnabled()) {
        bfOutCommandRC.channels[throttleSetup.aux] = (uint16_t)(PWM_MIN + (PWM_MAX - PWM_MIN) * throttle);
    }
}

void CBetaflightLink::setStickPitch(double stickPitch)
{
    const TControlRC& pitchSetup = m_pSetupRC->Controls["stick_pitch"];
    if (pitchSetup.isEnabled()) {
        bfOutCommandRC.channels[pitchSetup.aux] = (uint16_t)(PWM_MIN + 0.5 * (PWM_MAX - PWM_MIN)  * (stickPitch + 1.0));
    }
}

void CBetaflightLink::setStickRoll(double stickRoll)
{
    const TControlRC& rollSetup = m_pSetupRC->Controls["stick_roll"];
    if (rollSetup.isEnabled()) {
        bfOutCommandRC.channels[rollSetup.aux] = (uint16_t)(PWM_MIN + 0.5 * (PWM_MAX - PWM_MIN)  * (stickRoll + 1.0));
    }
}

void CBetaflightLink::setStickYaw(double stickYaw)
{
    const TControlRC& yawSetup = m_pSetupRC->Controls["stick_yaw"];
    if (yawSetup.isEnabled()) {
        bfOutCommandRC.channels[yawSetup.aux] = (uint16_t)(PWM_MIN + 0.5 * (PWM_MAX - PWM_MIN)  * (stickYaw + 1.0));
    }
}
