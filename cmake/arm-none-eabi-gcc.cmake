# Toolchain file: arm-none-eabi-gcc
# Usage: cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_PREFIX arm-none-eabi-)

set(CMAKE_C_COMPILER    ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER  ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER  ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_LINKER        ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_OBJCOPY       ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE          ${TOOLCHAIN_PREFIX}size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C   ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(COMMON_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -mthumb-interwork -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin -fshort-enums")

set(CMAKE_C_FLAGS_INIT       "${COMMON_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS_INIT     "${COMMON_FLAGS} -std=gnu++17")
set(CMAKE_ASM_FLAGS_INIT     "${COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -mthumb-interwork --specs=nosys.specs --specs=nano.specs -Wl,--gc-sections -Wl,--print-memory-usage")
