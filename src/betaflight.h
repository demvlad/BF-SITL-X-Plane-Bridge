/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

// SITL (software in the loop) simulator

#pragma once
#include <stdint.h>
#define BF_HOST "127.0.0.1"

#define PORT_PWM_RAW    9001    // Out
#define PORT_PWM        9002    // Out
#define PORT_STATE      9003    // In
#define PORT_RC         9004    // In

#define PWM_MIN	1000.0f
#define PWM_MAX	2000.0f

#define SIMULATOR_MAX_RC_CHANNELS   16
#define SIMULATOR_MAX_PWM_CHANNELS  16


typedef struct {
    double timestamp;                   // in seconds
    double imu_angular_velocity_rpy[3]; // Wx Wy Wz rad/s -> range: +/- 8192; +/- 2000 deg/se
    double imu_linear_acceleration_xyz[3];    // m/s/s body frame -> sim 1G = 9.80665, FC 1G = 256
    double imu_orientation_quat[4];     //w, x, y, z
    double velocity_xyz[3];             // m/s, earth frame   UNUSED
    double position_xyz[3];             // meters, NED from origin    UNUSED
    double pressure;
} fdm_packet;

typedef struct {
    double timestamp;                               // in seconds
    uint16_t channels[SIMULATOR_MAX_RC_CHANNELS];   // RC channels
} rc_packet;

typedef struct {
    float motor_speed[4];   // normal: [0.0, 1.0], 3D: [-1.0, 1.0]
} servo_packet;

typedef struct {
    uint16_t motorCount; // Count of motor in the PWM output.
    float pwm_output_raw[SIMULATOR_MAX_PWM_CHANNELS];   // Raw PWM from 1100 to 1900
} servo_packet_raw;

