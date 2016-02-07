set(GNUEFI ${PROJECT_SOURCE_DIR}/gnu_efi)

include_directories(${GNUEFI}/inc ${PROJECT_SOURCE_DIR}/include ${GNUEFI}/inc/x86_64 ${GNUEFI}/inc/protocol ${GNUEFI}/lib)

#REMOVED -fPIC 
#We use GNU_EFI_USE_MS_ABI to be sure EFI includes are in the correct convention
#We use also -m64 to be sure we have picked the correct cross-compiler and not i686-w64 
#REMOVED -maccumulate-outgoing-args -fshort-wchar -mno-red-zone -fno-stack-protector 
set(COMMONFLAGS "-DGNU_EFI_USE_MS_ABI -m64 -ffreestanding  -Wall -Werror")

#subsystem 10 is application
#NOTE: under 32bit is _efi_main
#REMOVED -Wl,-znocombreloc
set(LDFLAGS "-nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -Wl,-eefi_main -lgcc -Wl,-Bsymbolic -Wl,-nostdlib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 ${COMMONFLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c11 ${COMMONFLAGS}")


add_library(hello SHARED hello.cpp data.c)
target_link_libraries(hello ${LDFLAGS})

set(GNUEFILIB ${GNUEFI}/lib/data.c ${GNUEFI}/lib/str.c ${GNUEFI}/lib/print.c ${GNUEFI}/lib/misc.c ${GNUEFI}/lib/error.c ${GNUEFI}/lib/x86_64/math.c ${GNUEFI}/lib/guid.c ${GNUEFI}/lib/runtime/efirtlib.c  ${GNUEFI}/lib/console.c  ${GNUEFI}/lib/event.c  ${GNUEFI}/lib/runtime/rtdata.c ${GNUEFI}/lib/runtime/rtstr.c ${GNUEFI}/lib/runtime/efirtlib.c ${GNUEFI}/lib/dpath.c)

add_library(hello2 SHARED second.cpp ${GNUEFILIB})
target_link_libraries(hello2 ${LDFLAGS})

