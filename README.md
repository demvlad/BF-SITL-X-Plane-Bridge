# Betaflight SITL - X-Plane Bridge
The bridge for connecting Betaflight SITL to X-Plane simulators airplane model 

The BF-SITL-X-Plane Bridge can be used for debug airplanes version of betaflight firmware.

https://youtu.be/C_nt6-sjDcw?si=touE_RLNcDkhfH8M

## Install manual (for Windows 10,11)

1. Download "release" directory from this repository.
2. Install X-Plane 11 or 12 version. The Demo version can be used for short test flights.
3. Copy "release/Aircraft" directory to X-Plane directory.
4. Install Windows Linux Subsystem (WSL) https://learn.microsoft.com/en-us/windows/wsl/install and set Ubuntu.
5. Copy "release/websockify" and "release/SITL" directories to your PC disk.
6. Run WSL from command line by using cmd. Go to your websockify directory and run websockify proxi server by command:  ./websockify 127.0.0.1:6761 127.0.0.1:5761
8. Run second WSL instance from command line by using cmd. Go to your SITL directory and run SITL: ./betaflight_SITL.elf
10. Run Betaflight configurator: https://app.betaflight.com/
11. Enable "Enable manual connection mode" in configurators option.
12. Set port value in configurator "port" field as ws://127.0.0.1:6761
13. Press "Connect" button to connect SITL software
14. Load yours RC airplane "diff" data into SITL by using Configurator cli, save config. Can use SITL/cli_setup_example.txt config file for example.
15. Restart SITL and Betaflight configurator, connect to SITL
16. Go to Motors configurator tab and set motors protocol as PWM, save config.   
17. Restart SITL and Betaflight configurator, connect to SITL, set pid loop time in configurator page (select 1k ... 8k), save config.

## Setup X-Plane airplane model
Aircraft\Extra Aircraft\Wing640 directory contains model what is similar my airplane. This is flywing type airplane with one motors and ailerons control by roll and pitch together. The wing spane is 620mm, the weight is around 280g. This model is created in x-plane maker programm. 
The Aircraft\Extra Aircraft\Wing640\plugins\Betaflight-SITL\setup.xml file contains setup follow my SITL/cli_setup_example.txt diff file.
You should change some settings for your fly wing airplane in this file.
You can use this bridge for other airplanes types (2 motors, normal scheme airplane), but you need to create own model in plane maker. 
For example, the repository https://github.com/RomanLut/INAV-X-Plane-HITL contain bigger fly wing model.

## LINKS
Betaflight: https://betaflight.com/

Betaflight SITL manual: https://betaflight.com/docs/development/sitl

X-Plane: https://www.x-plane.com/

X-Plane SDK: https://developer.x-plane.com/sdk/

X-Plane Datarefs: https://developer.x-plane.com/datarefs/

Plane maker manual: https://developer.x-plane.com/manuals/planemaker/

INAV-X-Plane-HITL: https://github.com/RomanLut/INAV-X-Plane-HITL


