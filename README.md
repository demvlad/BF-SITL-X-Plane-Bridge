# Betaflight SITL - X-Plane Bridge
The bridge for connecting Betaflight SITL to X-Plane simulators airplane model 

The BF-SITL-X-Plane Bridge can be used for debug airplanes version of betaflight firmware.

https://youtu.be/C_nt6-sjDcw?si=touE_RLNcDkhfH8M

# SITL manual for Windows 10,11
To run Betaflight SITL from Windows you need to install [Windows Linux Subsystem (WSL) with Ubuntu OS](https://learn.microsoft.com/en-us/windows/wsl/install).

The link to [SITL using manual](https://betaflight.com/docs/development/sitl).

To get last airplane SITL version you need to build [Betaflight](https://betaflight.com/) with [SITL](https://betaflight.com/docs/development/sitl) target by setting USE_WING options.  

The example of ready to use compiled SITL file can be loaded from "release/SITL" directory of this repository. 

## SITL setup
1. To setup SITL by using [Betaflight configurator](https://app.betaflight.com/) you need to run [websockify proxy](https://github.com/novnc/websockify-other)

Copy websockify file from "release/websockify" repository directory to your PC disk.

2. Run WSL from command line by using cmd console. Go to your websockify directory and run websockify proxi server by command:  
./websockify 127.0.0.1:6761 127.0.0.1:5761
![websockify run](/images/websockfy.jpg)
3. Run second WSL instance from command line by using cmd. Go to your SITL directory and run SITL: ./betaflight_SITL.elf
![SITL run](/images/SITL.jpg)
4. Run [Betaflight configurator](https://app.betaflight.com/)

5. Set "Enable manual connection mode" in configurators option.

6. Set port value in configurator "port" field as ws://127.0.0.1:6761
![BF Configurator](/images/BF_cfg_setup.jpg)
7. Press "Connect" button to connect SITL software
![BF Configurator](/images/BF_cfg_show.jpg)
8. Load yours RC airplane "diff" data into SITL by using Configurator cli, save config. Can use SITL/cli_setup_example.txt config file for example.
![BF Configurator cli setup](/images/BF_cfg_cli.jpg)
9. Restart SITL and Betaflight configurator, connect to SITL

10. Go to Motors configurator tab and set motors protocol as PWM, save config.   
![BF Configurator cli setup](/images/BF_cfg_motors.jpg)
11. Restart SITL and Betaflight configurator, connect to SITL, set pid loop freq on the configuration page (select 1k ... 8k), save config.
![BF Configurator cli setup](/images/BF_cfg_pid_loop.jpg)
The SITL is configured.

## Setup X-Plane airplane model
***Aircraft\Extra Aircraft\Wing640*** directory contains model what is similar my airplane. This is flywing type airplane with one motors and ailerons control by roll and pitch together. The wingspan is 620mm, the weight is around 280g. 

***Copy this directory in X-Plane install path.***

This model is created in [x-plane maker](https://developer.x-plane.com/manuals/planemaker/) programm. Use this programm to edit model performance.


**Notice:**
If yours airplane has other controls (elevator, rudder, 2-motors) you need to create own X-Plane model in [plane maker](https://developer.x-plane.com/manuals/planemaker/). 
You can copy "plugins" directory into your X-Plane model to use this Betaflight SITL bridge.


The *setup.xml* (release/Aircraft/Extra Aircraft/Wing640/plugins/Betaflight-SITL/setup.xml) file contains setup follow my SITL/cli_setup_example.txt diff file.
You can change some settings for yours fly wing airplane in this setup file.

The ***"Modes"*** section contains flight modes setup: Arm, Manual, Angle. The Acro is default.
You can write in this section rc channels numbers and RC value to On flight mode.

The ***"Input"*** section contains RC input settings. You should write rc aux channel numbers for *"throttle"*, *"stick_pitch"*, *"stick_roll"* controls.
You can add *"stick_yaw"* row into this section if yours plane has rudder or 2-motors differencial control.

The ***"Output"*** section contains servo output settings. You can write servo numbers for *"motor1"*, *"aileron_left"*, *"aileron_right"* channels.
You can add *"elevator"*, *"rudder"* or *"motor2"* row if yours plane has these controls.

# Run X-Plane airplane model by using SITL control
1. Run X-Plane and select Wing640 model. Start flight.
![Run X-Plane](/images/X-Plane-start.jpg)
![Run X-Plane](/images/X-Plane-main.jpg)
2. Start WSL session, go to SITL directory and start SITL, set your windows IP address as SITL run parameter (172.20.144.1 in my case):

./betaflight_SITL.elf 172.20.144.1

![Run SITL](/images/SITL_XPlane_Start.jpg)

3. Go to X-Plane "plugins" menu. By using this menu you can Arm/Disarm (set throttle to idle before) SITL and select flight mode.

![X-Plane Menu](/images/SITL_XPlane_Menu.jpg)
4. Let's fly!

You can use [Betaflight configurator](https://app.betaflight.com/) during simulation to check RC channels, flight modes state and Arm disabled flags.
Look at ["SITL setup"](#sitl-setup) manual above.



## LINKS
Betaflight: https://betaflight.com/

Betaflight SITL manual: https://betaflight.com/docs/development/sitl

X-Plane: https://www.x-plane.com/

X-Plane SDK: https://developer.x-plane.com/sdk/

X-Plane Datarefs: https://developer.x-plane.com/datarefs/

Plane maker manual: https://developer.x-plane.com/manuals/planemaker/

INAV-X-Plane-HITL: https://github.com/RomanLut/INAV-X-Plane-HITL, Thank's for start point my project.


