#UEFI Bare Bone Exercise#
by Emanuele Ruffaldi
using CMake,mxe and VirtualBox/Qemu

There are two ways to make UEFI applications due to the fact that they are PE executables following Microsoft function call convention. The first is via a ELF Linux toolchain coupled with the extraction of the binary and relocation of the symbols. This is the path followed by the Haiku example [2] originated from the full Gnu EFI library. The second way adopted here is based on a cross compiler that builds Windows 64-bit applications as also discussed in OSDEV [1]. Here will use MXE supporting both Linux and OSX as hosting compilers. While 32-bit EFI are possible here we'll support only 64-bit.

Requirements:
- GCC Cross Compiler x86_64-w64-mingw32. MXE is fine
- MTools 
- GNU-efi package. I have embedded in this example version 3.0.3 having not an official github. Other github do exists such as https://github.com/vathpela/gnu-efi

##OSX##

Install mxe and then with MacPort install MTools

##Linux##
Following OSDEV [1] the requirements are satisfied with the following, in addition to mxe:

sudo apt-get install qemu binutils-mingw-w64  mtools

Mxe can be replaced also by gcc-mingw-w64 but then custom scripts are needed for building additional packages

#Build#

##Build##
I have prepared a CMake but it is fundamental to specify the cross-compiler to CMake, for example under OSX

	cmake -DCMAKE_TOOLCHAIN_FILE=/Applications/mxe/usr/x86_64-w64-mingw32.static/share/cmake/mxe-conf.cmake

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
	make &&	mcopy -oi fat.img src/libhello2.dll ::/EFI/BOOT/BOOTX64.EFI

##Manually Mount##

mcopy allows for managing the filesystem directly but sometimes it is good to mount the floppy. Under OSX image files needs to be mapped first as block devices and then mounted with mount:

	hdiutil attach -imagekey diskimage-class=CRawDiskImage -nomount fat.img
	mount -t msdos /dev/disk2 tmp

Unmounting requires detach, WHERE /dev/disk2 is our disk

	hdiutil detach /dev/disk2

#Testing with QEmu#

Following the instruction from OSDEV it is needed QEmu and the OVMF firmware (http://tianocore.sourceforge.net/wiki/OVMF). Then the boot is straightforward after having installed qemu and placed OVMF.fd somewhee:
	
	qemu-system-x86_64 -L OVMF_dir/ -bios OVMF.fd -drive file=fat.img,if=ide,id=drive-ide0-0-0

#Testing with VirtualBox#

##Setup##

Creation of VM

	VM='MyUEFI'
	VBoxManage createvm --name $VM --ostype "Other" --register
	VBoxManage modifyvm $VM --ioapic on
	VBoxManage modifyvm $VM --boot1 floppy
	VBoxManage modifyvm $VM --memory 1024 --vram 128
	VBoxManage modifyvm $VM --firmware efi64
	VboxManage storagectl $VM --name "Floppy" --add floppy

For verification:

	VBoxManage debugvm $VM osinfo

Finally we specify our image

	VboxManage storageattach  $VM --storagectl "Floppy" --port 0 --device 0 --type fdd --medium "fat.img"

##Start##

First start 

	VboxManage startvm $VM

Thanks to the startup.nsh VirtualBox will boot into our UEFI applications otherwise we'll need to specify via the UEFI Interactive Shell. 

A cool feature of VirtualBox is that, if your UEFI application has not crashed, you can run it again with \EFI\BOOT\BOOTX64.EFI without rebooting the Virtual Machine. Just press arrow up.

#Multiprocessing#

Traditionally MP has been quite boring to be initialized due to the direct exchange with the APIC plus the preparation of the MP memory structures (see smpboot.c in Linux and the Intel MP Specification). UEFI makes things much easier firstly because it already initializes the AP and then picks one processor as the BSP (Boot Strap Processor). Then there are two possible UEFI protocols that allows for managing tasks over the AP, one is EfiMpServiceProtocol, the other TODO. As discussed in [5] it is quite uncommon to have none of such protocols, and then, if they are totally missing, it only remains to scan the CPU configuration via CPUID plus starting directly tasks over the APs with INIT-SIP1. Even in this latter case the AP are ready for work. What is important to highlight is that UEFI services can be called safely only from the BSP, meaning that we need to initialize peripherals, or, alternatively to use AP as worker processors.

Data exchange and synchronization between the CPUs can be performed with the CPU atomic instructions, and, maybe even at high-level with C++11 atomic constructs that are directly based on assembly instructions.

#Future ideas#
This is an exercises for harnessing the power of UEFI, without the aim of making a pseudo-OS, but, anyway there are interesting services and aspect of UEFI that can be looked at for making it usable:

- Timer (http://kurtqiao.github.io/uefi/2015/01/06/wait-for-event.html)
- Floating point issues
- Network
- UGA Graphics

EFI_FRAMEWORK_MP_SERVICES_PROTOCOL_GUID
EFI_MP_SERVICES_PROTOCOL_GUID

#References#

[1] Related instructiosn from OSDEV: http://wiki.osdev.org/UEFI_Bare_Bones
[2] Other related project (Make+QEmu): https://github.com/tqh/efi-example 
[3] http://www.rodsbooks.com/efi-programming/hello.html
[4] http://www.uefi.org/sites/default/files/resources/Plugfest_Multiprocessing-with_UEFI-McDaniel.pdf)
[5]