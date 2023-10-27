Work in progress audio driver for the 12" MacBook that's largely based on davidjo's [snd_hda_macbookpro](https://github.com/davidjo/snd_hda_macbookpro)

**macbooks supported**:
MacBook9,1 (2016) and Macbook10,1 (2017)  
Note: the Macbook8,1 (2015) is not supported

**Kernels supported**:
\>= 5.0

Only the speakers work with this driver (no wired headphone support)  
However, the stock kernel driver supports wired headphones  
So in order to use wired headphones, you must remove the module that this project creates and reboot  

Compiling and installing driver:
-------------

**fedora package install**
```
dnf install dkms gcc kernel-devel make wget
```
**ubuntu package install**
```
apt install dkms gcc linux-headers-generic make wget
```
**arch package install**
```
pacman -S dkms gcc linux-headers make wget
```
1. **build and install dkms module** (experimental feature)  
this will build the module for the current/active kernel  
and will auto-compile this module whenever you install a newer kernel  
```
git clone https://github.com/leifliddy/macbook12-audio-driver.git
cd macbook12-audio-driver/
# run the following command as root or with sudo
./install.cirrus.driver.sh -i
reboot

# to uninstall the dkms feature run:
./install.cirrus.driver.sh -u
```

2. (backup method if dkms didn't work) **manually build and install module for current kernel**
```
git clone https://github.com/leifliddy/macbook12-audio-driver.git
cd macbook12-audio-driver/
# run the following command as root or with sudo
./install.cirrus.driver.sh
reboot
```
