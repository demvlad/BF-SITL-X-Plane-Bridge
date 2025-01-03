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
6. Run WSL from command line by using cmd.
7. Go to your websockify directory in WSL and run websockify proxi server by command:  ./websockify 127.0.0.1:6761 127.0.0.1:5761
8. Run second WSL instance from command line by using cmd.
9. Go to your SITL directory in WSL and run SITL: ./betaflight_SITL.elf
10. Run Betaflight configurator https://app.betaflight.com/
11. Enable "Enable manual connection mode" in configurators option.
12. Set port value in configurator "port" field as ws://127.0.0.1:6761
13. Press "Connect" button to connect SITL software
14. Load yours RC airplane "diff" data into SITL by using Configurator cli, save config. 
15. Restart SITL and Betaflight configurator, connect to SITL
16. Go to Motors configurator tab and set motors protocol as PWM, save config.   
17. Restart SITL and Betaflight configurator, connect to SITL, set pid loop time in configurator page (select 1k ... 8k), save config.

## LINKS
Betaflight: https://betaflight.com/

Betaflight SITL manual: https://betaflight.com/docs/development/sitl

X-Plane: https://www.x-plane.com/

X-Plane SDK: https://developer.x-plane.com/sdk/

X-Plane Datarefs: https://developer.x-plane.com/datarefs/

Plane maker manual: https://developer.x-plane.com/manuals/planemaker/

INAV-X-Plane-HITL: https://github.com/RomanLut/INAV-X-Plane-HITL


