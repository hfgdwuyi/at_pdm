# Toolchain file: arm-none-eabi-gcc
# Usage: cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake
#
# Requires the ARM GNU Toolchain (includes newlib-nano):
#   https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Toolchain is stored locally under the project root for modularity.
# CMAKE_CURRENT_LIST_DIR is the directory containing this file (cmake/).
set(TOOLCHAIN_DIR ${CMAKE_CURRENT_LIST_DIR}/../tools/arm-gnu-toolchain-14.2.rel1-darwin-arm64-arm-none-eabi)

set(TOOLCHAIN_PREFIX ${TOOLCHAIN_DIR}/bin/arm-none-eabi-)

set(CMAKE_C_COMPILER    ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER  ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER  ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_LINKER        ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_OBJCOPY       ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE          ${TOOLCHAIN_PREFIX}size)
set(CMAKE_AR            ${TOOLCHAIN_PREFIX}gcc-ar)
set(CMAKE_RANLIB        ${TOOLCHAIN_PREFIX}gcc-ranlib)

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C   ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CPU_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -mthumb-interwork -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin -fshort-enums")
set(NEWLIB_INC "${TOOLCHAIN_DIR}/arm-none-eabi/include")

# Compile: -isystem gives the preprocessor access to newlib headers (stdint.h etc.)
# Link:   --specs=nano.specs only in linker flags avoids the duplication that
#         occurs when both CFLAGS and ASMFLAGS carry specs to the link step.
set(CMAKE_C_FLAGS_INIT       "${CPU_FLAGS} -std=gnu11 -isystem ${NEWLIB_INC}")
set(CMAKE_CXX_FLAGS_INIT     "${CPU_FLAGS} -std=gnu++17 -isystem ${NEWLIB_INC}")
set(CMAKE_ASM_FLAGS_INIT     "${CPU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${CPU_FLAGS} --specs=nano.specs -Wl,--gc-sections -Wl,--print-memory-usage")
