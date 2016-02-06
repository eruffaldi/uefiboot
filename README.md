#UEFI Bare Bone Exercise#
by Emanuele Ruffaldi
using CMake,mxe and VirtualBox

Related instructiosn from OSDEV: http://wiki.osdev.org/UEFI_Bare_Bones
Other related project (Make+QEmu): https://github.com/tqh/efi-example http://www.rodsbooks.com/efi-programming/hello.html


Requirements:
- GCC Cross Compiler x86_64-w64-mingw32. MXE is fine
- MTools 
- GNU-efi package. I have embedded in this example version 3.0.3

##OSX##

Install mxe and then with MacPort install MTools

##Linux##
Following OSDEV:

sudo apt-get install qemu binutils-mingw-w64 gcc-mingw-w64 xorriso mtools

#Build#

##Build##
I have prepared a CMake but it is fundamental to specify the cross-compiler to CMake, for example under OSX

	cmake -DCMAKE_TOOLCHAIN_FILE=/Applications/mxe/usr/i686-w64-mingw32.static/share/cmake/mxe-conf.cmake

##Prepare Image##

	dd if=/dev/zero of=fat.img bs=1k count=1440
	mformat -i fat.img -f 1440 ::
	mmd -i fat.img ::/EFI
	mmd -i fat.img ::/EFI/BOOT
	echo "\EFI\BOOT\BOOTX64.EFI" > startup.nsh
	mcopy -i fat.img startup.nsh ::/

##Deploy##

Deployment of the example hello is:

	make &&	mcopy -oi fat.img src/libhello.dll ::/EFI/BOOT/BOOTX64.EFI

##Manually Mount##

mcopy allows for managing the filesystem directly but sometimes it is good to mount the floppy. Under OSX image files needs to be mapped first as block devices and then mounted with mount:

	hdiutil attach -imagekey diskimage-class=CRawDiskImage -nomount fat.img
	mount -t msdos /dev/disk2 tmp

Unmounting requires detach, WHERE /dev/disk2 is our disk

	hdiutil detach /dev/disk2

#Testing with VirtualBox#

In OSDEV you can find QEmu instructions using the OVMF firmware. I have tested with VirtualBox booting a VM with the specified FAT image

##Setup##

Creation

	VM='MyUEFI'
	VBoxManage createvm --name $VM --ostype "Other" --register
	VBoxManage modifyvm $VM --ioapic on
	VBoxManage modifyvm $VM --boot1 floppy
	VBoxManage modifyvm $VM --memory 1024 --vram 128
	VBoxManage modifyvm $VM --firmware efi
	VboxManage storagectl $VM --name "Floppy" --add floppy

Finally we specify our image

	VboxManage storageattach  $VM --storagectl "Floppy" --port 0 --device 0 --type fdd --medium "fat.img"

##Start##

First start 

	VboxManage startvm $VM

Thanks to the startup.nsh VirtualBox will boot into our UEFI applications otherwise we'll need to specify via the UEFI Interactive Shell. 

A cool feature of VirtualBox is that, if your UEFI application has not crashed, you can run it again with \EFI\BOOT\BOOTX64.EFI without rebooting the Virtual Machine. Just press arrow up.

###VirtualBox Issue###

VirtualBox was not sending keys to the UEFI application. The driver can be found here: http://www.virtualbox.org/svn/vbox/trunk/src/VBox/Devices/EFI/Firmware/IntelFrameworkModulePkg/Csm/BiosThunk/KeyboardDxe/BiosKeyboard.c  


#Future#

Examples online of Timer: http://kurtqiao.github.io/uefi/2015/01/06/wait-for-event.html


