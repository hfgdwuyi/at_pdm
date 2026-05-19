cmake_minimum_required(VERSION 3.20)

project(at_pdm C CXX ASM)

# =============================================================================
# Build configuration
# =============================================================================

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Build type: Debug or Release" FORCE)
endif()

# =============================================================================
# Common compile definitions (must be set before any target)
# =============================================================================

add_compile_definitions(
  USE_HAL_DRIVER
  STM32F429xx
  USE_STM324x9I_EVAL
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-Og -g3)
else()
  add_compile_options(-O2)
endif()

# =============================================================================
# Include directories
# =============================================================================

set(INC_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/core
  ${CMAKE_CURRENT_SOURCE_DIR}/module
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/STM324x9I_EVAL
  ${CMAKE_CURRENT_SOURCE_DIR}/CanFestival/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/CanFestival/driver
  ${CMAKE_CURRENT_SOURCE_DIR}/CanFestival/driver/stm32
  ${CMAKE_CURRENT_SOURCE_DIR}/lwip
  ${CMAKE_CURRENT_SOURCE_DIR}/lwip/arch
  ${CMAKE_CURRENT_SOURCE_DIR}/lwip/lwip_app/lwip_comm
  ${CMAKE_CURRENT_SOURCE_DIR}/lwip/lwip-1.4.1/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/lwip/lwip-1.4.1/src/include/ipv4
  ${CMAKE_CURRENT_SOURCE_DIR}/lwip/lwip-1.4.1/src/include/lwip
  ${CMAKE_CURRENT_SOURCE_DIR}/lwip/lwip-1.4.1/src/include/netif
)

# =============================================================================
# HAL Driver  (static library)
# =============================================================================

file(GLOB HAL_SOURCES
  Drivers/STM32F4xx_HAL_Driver/Src/*.c
)

add_library(hal STATIC ${HAL_SOURCES})
target_include_directories(hal PUBLIC ${INC_DIRS})
target_compile_definitions(hal PRIVATE USE_HAL_DRIVER STM32F429xx)

# =============================================================================
# lwIP  (static library)
# =============================================================================

file(GLOB_RECURSE LWIP_SOURCES
  lwip/lwip-1.4.1/src/core/*.c
  lwip/lwip-1.4.1/src/core/ipv4/*.c
  lwip/lwip-1.4.1/src/core/snmp/*.c
  lwip/lwip-1.4.1/src/api/*.c
  lwip/lwip-1.4.1/src/netif/ethernetif.c
  lwip/lwip-1.4.1/src/netif/etharp.c
  lwip/arch/*.c
  lwip/lwip_app/lwip_comm/*.c
  lwip/lwip_app/tcp_server_demo/*.c
  lwip/lwip_app/tcp_client_demo/*.c
  lwip/lwip_app/udp_demo/*.c
  lwip/lwip_app/web_server_demo/*.c
)

add_library(lwip STATIC ${LWIP_SOURCES})
target_include_directories(lwip PUBLIC ${INC_DIRS})
target_compile_definitions(lwip PRIVATE USE_HAL_DRIVER STM32F429xx)

# =============================================================================
# CanFestival  (static library)
# =============================================================================

file(GLOB CANFEST_SOURCES
  CanFestival/src/*.c
  CanFestival/driver/stm32/*.c
)

add_library(canfestival STATIC ${CANFEST_SOURCES})
target_include_directories(canfestival PUBLIC ${INC_DIRS})
target_compile_definitions(canfestival PRIVATE USE_HAL_DRIVER STM32F429xx)

# =============================================================================
# Core + Module  (object library - merged into final executable)
# =============================================================================

file(GLOB CORE_SOURCES core/*.c)
file(GLOB MODULE_SOURCES module/*.c)

# =============================================================================
# Application  (main executable)
# =============================================================================

file(GLOB APP_SOURCES
  Src/*.c
  Src/startup_stm32f429xx.s
)

add_executable(${PROJECT_NAME}.elf
  ${APP_SOURCES}
  ${CORE_SOURCES}
  ${MODULE_SOURCES}
)

target_include_directories(${PROJECT_NAME}.elf PRIVATE ${INC_DIRS})

target_link_libraries(${PROJECT_NAME}.elf
  hal
  lwip
  canfestival
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
  -T ${CMAKE_CURRENT_SOURCE_DIR}/ld/STM32F429NIHx_FLASH.ld
)

# =============================================================================
# Post-build: generate .hex and .bin, print size
# =============================================================================

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf>
          ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf>
          ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin
  BYPRODUCTS ${PROJECT_NAME}.hex ${PROJECT_NAME}.bin
  COMMENT "Generating .hex and .bin files"
)
