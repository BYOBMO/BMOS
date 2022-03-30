# BMOS
BMO Operating System

## License
Source code in this project is licensed under GNU General Public License v3.0

<p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/">Graphical and audio assets (Images, Icons, Videos, Audio) are licensed under <a href="http://creativecommons.org/licenses/by-nc-nd/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">CC BY-NC-ND 4.0<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/nc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/nd.svg?ref=chooser-v1"></a>
  
Fonts are licensed under their respective licenses. See fonts folder for specific licenses.
  
## Install
```
sudo apt update
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

  ## Build
  ```
  mkdir build
cd build
cmake ..
make
make install
```
  
## Manual Start
  ```
  /home/pi/bmos/bmos
  ```
  
## Auto Start
  ```
  sudo cp /home/pi/bmos/scripts/10-bmos.sh /etc/profile.d
  sudo +r /etc/profile.d/10-bmos.sh
  ```
