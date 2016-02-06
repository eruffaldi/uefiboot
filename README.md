#UEFI Bare Bone Exercise#
by Emanuele Ruffaldi
using CMake,mxe and VirtualBox

Using indications from OSDEV: http://wiki.osdev.org/UEFI_Bare_Bones

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

I have prepared a CMake but it is fundamental to specify the cross-compiler to CMake, for example under OSX

	cmake -DCMAKE_TOOLCHAIN_FILE=/Applications/mxe/usr/i686-w64-mingw32.static/share/cmake/mxe-conf.cmake


#Testing#

In OSDEV you can find QEmu instructions using the OVMF firmware. I have tested with VirtualBox