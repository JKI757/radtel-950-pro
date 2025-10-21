cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)
# AT32 WorkBench generated symbols (macros)
set(WK_Defines_Syms
    USE_STDPERIPH_DRIVER
    AT32F403AVGT7
    $<$<CONFIG:Debug>:DEBUG>
)

# AT32 WorkBench generated include paths
set(WK_Include_Dirs
    ${CMAKE_SOURCE_DIR}/project/inc
    ${CMAKE_SOURCE_DIR}/libraries/drivers/inc
    ${CMAKE_SOURCE_DIR}/libraries/cmsis/cm4/core_support
    ${CMAKE_SOURCE_DIR}/libraries/cmsis/cm4/device_support
)

# AT32 WorkBench generated application sources
set(WK_Application_Src
    ${CMAKE_SOURCE_DIR}/project/src/main.c
    ${CMAKE_SOURCE_DIR}/project/src/syscalls.c
    ${CMAKE_SOURCE_DIR}/project/src/sysmem.c
    ${CMAKE_SOURCE_DIR}/project/src/at32f403a_407_wk_config.c
    ${CMAKE_SOURCE_DIR}/project/src/at32f403a_407_int.c
    ${CMAKE_SOURCE_DIR}/project/src/wk_system.c
    ${CMAKE_SOURCE_DIR}/project/src/wk_spi.c
    ${CMAKE_SOURCE_DIR}/project/src/wk_usart.c
    ${CMAKE_SOURCE_DIR}/project/src/wk_gpio.c
    ${CMAKE_SOURCE_DIR}/startup_at32f403a_407.s
    ${CMAKE_SOURCE_DIR}/project/src/rtc.c
)

# AT32 Drivers
set(AT32_Drivers_Src
    ${CMAKE_SOURCE_DIR}/libraries/cmsis/cm4/device_support/system_at32f403a_407.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_crm.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_tmr.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_rtc.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_gpio.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_i2c.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_usart.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_pwc.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_adc.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_spi.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_dma.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_flash.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_crc.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_wwdt.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_wdt.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_exint.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_misc.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_usb.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_acc.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_can.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_dac.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_xmc.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_emac.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_sdio.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_debug.c
    ${CMAKE_SOURCE_DIR}/libraries/drivers/src/at32f403a_407_bpr.c
)

# Drivers Middlewares

# Link directories setup
set(WK_LINK_DIRS

)

# Project static libraries
set(WK_LINK_LIBS
    AT32_Drivers
)

# Interface library for includes and symbols
add_library(at32_workbench INTERFACE)
target_include_directories(at32_workbench INTERFACE ${WK_Include_Dirs})
target_compile_definitions(at32_workbench INTERFACE ${WK_Defines_Syms})

# Create AT32_Drivers static library
add_library(AT32_Drivers OBJECT)
target_sources(AT32_Drivers PRIVATE ${AT32_Drivers_Src})
target_link_libraries(AT32_Drivers PUBLIC at32_workbench)

# Add AT32 WorkBench generated application sources to the project
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${WK_Application_Src})

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${WK_LINK_DIRS})

# Add libraries to the project
target_link_libraries(${CMAKE_PROJECT_NAME} ${WK_LINK_LIBS})

# Add the map file to the list of files to be removed with 'clean' target
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

# Validate that AT32 WorkBench code is compatible with C standard
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif()
