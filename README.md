# Betaflight SITL - X-Plane Bridge
The bridge for connecting Betaflight SITL to X-Plane simulators airplane model 

The BF-SITL-X-Plane Bridge can be used for debug airplanes version of betaflight firmware.

https://youtu.be/C_nt6-sjDcw?si=touE_RLNcDkhfH8M

# SITL manual for Windows 10,11
To run Betaflight SITL from Windows you need to install [Windows Linux Subsystem (WSL) with Ubuntu OS](https://learn.microsoft.com/en-us/windows/wsl/install).
The link to [SITL using manual](https://betaflight.com/docs/development/sitl)
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
5. Enable "Enable manual connection mode" in configurators option.
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
Aircraft\Extra Aircraft\Wing640 directory contains model what is similar my airplane. This is flywing type airplane with one motors and ailerons control by roll and pitch together. The wing spane is 620mm, the weight is around 280g. This model is created in x-plane maker programm. 
The Aircraft\Extra Aircraft\Wing640\plugins\Betaflight-SITL\setup.xml file contains setup follow my SITL/cli_setup_example.txt diff file.
You should change some settings for your fly wing airplane in this file.
You can use this bridge for other airplanes types (2 motors, normal scheme airplane), but you need to create own model in plane maker. 

## LINKS
Betaflight: https://betaflight.com/

Betaflight SITL manual: https://betaflight.com/docs/development/sitl

X-Plane: https://www.x-plane.com/

X-Plane SDK: https://developer.x-plane.com/sdk/

X-Plane Datarefs: https://developer.x-plane.com/datarefs/

Plane maker manual: https://developer.x-plane.com/manuals/planemaker/

INAV-X-Plane-HITL: https://github.com/RomanLut/INAV-X-Plane-HITL


