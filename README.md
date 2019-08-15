Work in progress audio driver for 12" MacBooks (2015 and later) that was modified from davidjo's [snd_hda_macbookpro](https://github.com/davidjo/snd_hda_macbookpro)

Driver builds successfully on kernels 5.0 to 5.2 (haven't tested eariler kernels)
At this time, only the speakers work with this driver (no wired headphone support)
In order to use wired headphones, you must switch between this driver and stock kernel driver

**Terminology**   
headphones == wired headphones (not bluetooth)  
speaker driver == this driver   
headphone driver == stock kernel driver  


Compiling and installing driver:
-------------

**fedora package install**
```
dnf install wget make gcc kernel-devel
```
**ubuntu package install**  
```
apt install wget make gcc linux-headers-generic
```
**build driver**  
```
git clone https://github.com/cb22/macbook12-audio-driver.git
cd macbook12-audio-driver/
./install.cirrus.driver.sh
```

At this point, the speakers should (hopefully) be working. You can stop here if you don't plan on using headphones.



Switching between speaker and headphone drivers:
-----------
Things get a bit convulted here....

**Edit /etc/pulse/default.pa**  
   With the changes listed in the **pulse_audio_configs/default.pa** file

**Activate PulseAudio Changes**  
```
sudo chmod -x /usr/bin/pulseaudio
sudo killall -9 pulseaudio
sudo chmod +x /usr/bin/pulseaudio
pulseaudio --start  # as normal user
```
Note: Might be easier to just reboot  


**Use provided support scripts to switch between the speaker and headphone driver**  
1. **switch.audio.driver.sh**  
   Script will toggle between the headphone and speaker driver.
   Run as root (or with sudo). Will only work if there are no active audio streams. 

2. **pulse.switch.audio.sh**  
   Script will toggle driver and will work if active audio streams are present.
   Run as a normal user. Since this script calls script.audio.driver.sh, the normal user running this script must have the necessary sudo rights.
   ie:
   **/etc/sudoers**   
   youruser ALL=(ALL) NOPASSWD: /usr/local/sbin/switch.audio.driver.sh 

   Note: **pulse.switch.audio.sh** works perfectly on Fedora 30. I had trouble getting to work reliably on Ubuntu 19.04.
