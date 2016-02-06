set(GNUEFI ${PROJECT_SOURCE_DIR}/gnu_efi)

include_directories(${GNUEFI}/inc ${GNUEFI}/inc/x86_64 ${GNUEFI}/inc/protocol ${GNUEFI}/lib)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -ffreestanding)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -ffreestanding)

set(LDFLAGS -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -Wl,-e_efi_main -lgcc)

add_library(hello SHARED first.cpp data.c)
target_link_libraries(hello ${LDFLAGS})

set(GNUEFILIB ${GNUEFI}/lib/data.c ${GNUEFI}/lib/str.c ${GNUEFI}/lib/print.c)

add_library(hello2 SHARED second.cpp ${GNUEFILIB})
target_link_libraries(hello2 ${LDFLAGS})

