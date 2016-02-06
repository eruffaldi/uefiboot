set(GNUEFI $(PROJECT_SOURCE_DIR)/gnu_efi/)

-ffreestanding -Ipath/to/gnu-efi/inc -Ipath/to/gnu-efi/inc/x86_64 -Ipath/to/gnu-efi/protocol 

include_directories(${GNUEFI}/inc ${GNUEFI}/inc/x86_64 ${GNUEFI}/protocol)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -ffreestanding)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -ffreestanding)

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main -lgcc)

add_executable(hello first.cpp ${GNUEFI}/lib/data.c)
target_link_libraries(hello)