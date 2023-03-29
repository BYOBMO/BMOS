# BMOS
BMO Operating System

## License
Source code in this project is licensed under GNU General Public License v3.0

<p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/">Graphical and audio assets (Images, Icons, Videos, Audio) are licensed under <a href="http://creativecommons.org/licenses/by-nc-nd/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">CC BY-NC-ND 4.0<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/nc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/nd.svg?ref=chooser-v1"></a>
  
Fonts are licensed under their respective licenses. See fonts folder for specific licenses.
  
## Install
Make sure you have installed all of this first:
https://github.com/BYOBMO/BMO-Utils

```
sudo apt update
```
```
sudo apt-get install cmake
```
  ```
sudo apt-get install libsdl2-image-dev
  ```
  ```
sudo apt-get install libsdl2-ttf-dev
  ```
  ```
sudo apt-get install libsdl2-mixer-dev
```
```
  sudo apt-get install libboost-all-dev
```

  ## Build
  ```
  mkdir build
cd build
cmake ..
make
make install
```

## GPU
Set GPU memory to 256.
```
sudo raspi-config
```

Performance Options->GPU Memory

## Manual Start
```
cd /home/pi/bmos
/home/pi/bmos/bmos
```
  
## Auto Start
```
sudo cp /home/pi/bmos/scripts/10-bmos.sh /etc/profile.d
sudo chmod +r /etc/profile.d/10-bmos.sh
```

## Key Mappings
Keys are mapped in the /home/pi/bmos/bmo.txt file.

Supported commands are face, mp4, system, and vg.

### face
The face command changes BMO's static face.
```
<key>:face:<filename>
```

Example:
```
1:face:bmo01.jpg
```
Will map keyboard "1" to to show "bmo01.jpg".

### mp4
The video command starts a video, usually a beemote.
```
<key>:mp4:<videofile>:<imagefile>
```

Example:
```
q:mp4:cn.mp4:bmo01.jpg
```
Will map keyboard "q" to play the mp4 file "cn.mp4" and then switch to "bmo01.jpg" when done.

### system
The "system" command will launch a script. Be careful with this. Starting the wrong script can damage your system.

Example:
```
Z:system:/home/pi/bmos/scripts/waveleft.sh
```

Executes the script tha waves BMO's left arm.

### vg
The "vg" command will launch EmulationStation.
Example:
```
;:vg:videogames.mp4
```

This maps the semicolon ";" key to start EmulationStation. It will play videogames.mp4 before launching.

To get out of EmulationStation, you usually need to press F4. It depends on how you have your ket bindings set up for EmulationStation.

## Exiting
Alt-F4 will terminate the application and return to the CLI.

