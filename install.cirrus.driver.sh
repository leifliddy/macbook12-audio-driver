#!/bin/bash

while [ $# -gt 0 ]
do
    case $1 in
    -i|--install) dkms_action='install';;
    -k|--kernel) dkms_kernel=$2; [[ -z $dkms_kernel ]] && echo '-k|--kernel must be followed by a kernel version' && exit 1;;
    -r|--remove) dkms_action='remove';;
    -u|--uninstall) dkms_action='remove';;
    (-*) echo "$0: error - unrecognized option $1" 1>&2; exit 1;;
    (*) break;;
    esac
    shift
done

if [[ $dkms_action == 'install' ]]; then
    bash dkms.sh
    exit
elif [[ $dkms_action == 'remove' ]]; then
    bash dkms.sh -r
    exit
fi

[[ -n $dkms_kernel ]] && uname_r=$dkms_kernel || uname_r=$(uname -r)
kernel_version=$(echo $uname_r | cut -d '-' -f1) #ie 6.4.15

major_version=$(echo $kernel_version | cut -d '.' -f1)
minor_version=$(echo $kernel_version | cut -d '.' -f2)
major_minor=${major_version}${minor_version}
kernel_short_version="$major_version.$minor_version" #ie 5.2

build_dir="build"
patch_dir='patch_cirrus'
hda_dir="$build_dir/hda"

[[ -d $hda_dir ]] && rm -rf $hda_dir
[[ ! -d $build_dir ]] && mkdir $build_dir

# attempt to download linux-x.x.x.tar.xz kernel
wget -c https://cdn.kernel.org/pub/linux/kernel/v$major_version.x/linux-$kernel_version.tar.xz -P $build_dir

if [[ $? -ne 0 ]]; then
   # if first attempt fails, attempt to download linux-x.x.tar.xz kernel
   kernel_version=$kernel_short_version
   wget -c https://cdn.kernel.org/pub/linux/kernel/v$major_version.x/linux-$kernel_version.tar.xz -P $build_dir
fi

[[ $? -ne 0 ]] && echo "kernel could not be downloaded...exiting" && exit

# remove old kernel tar.xz archives
find build/ -type f | grep -E linux.*.tar.xz | grep -v $kernel_version.tar.xz | xargs rm -f

tar --strip-components=3 -xvf $build_dir/linux-$kernel_version.tar.xz --directory=build/ linux-$kernel_version/sound/pci/hda
mv $hda_dir/Makefile $hda_dir/Makefile.orig
mv $hda_dir/patch_cirrus.c $hda_dir/patch_cirrus.c.orig
cp $patch_dir/Makefile $patch_dir/patch_cirrus.c $patch_dir/patch_cirrus_a1534_setup.h $patch_dir/patch_cirrus_a1534_pcm.h $hda_dir/

# if kernel version is >= 6.12 then change
# snd_pci_quirk to hda_quirk
# SND_PCI_QUIRK to HDA_CODEC_QUIRK
# but leave alone SND_PCI_QUIRK_VENDOR

if (( major_version > 6 || (major_version == 6 && minor_version >= 12) )); then
   sed -i 's/snd_pci_quirk/hda_quirk/' $hda_dir/patch_cirrus.c
   sed -i 's/SND_PCI_QUIRK\b/HDA_CODEC_QUIRK/' $hda_dir/patch_cirrus.c
fi

if (( major_version == 6 && minor_version <= 11 )); then
   sed -i 's/hda_quirk/snd_pci_quirk/' $hda_dir/patch_cirrus.c
fi

# if kernel version is < 5.6 then change
# timespec64 to timespec
# ktime_get_real_ts64 to getnstimeofday

if [ $major_minor -lt 56 ]; then
   sed -i 's/timespec64/timespec/' $hda_dir/patch_cirrus.c
   sed -i 's/timespec64/timespec/' $hda_dir/patch_cirrus_a1534_pcm.h
   sed -i 's/ktime_get_real_ts64/getnstimeofday/' $hda_dir/patch_cirrus_a1534_pcm.h
fi

update_dir="/lib/modules/$(uname -r)/updates"
[[ ! -d $update_dir ]] && mkdir $update_dir
make
make install
echo -e "\ncontents of $update_dir"
ls -lA $update_dir
