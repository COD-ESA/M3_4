# Конфигурация
# cmake --preset blue_pill_config
# ИЛИ
# cmake --preset black_pill_config

# Сборка
# cmake --build --preset blue

cmake_minimum_required(VERSION 3.21)

project(learn_F103_F411 C ASM)

#флаги оптимизации
set(OPTIMIZ_FLAGS
        -mthumb
        -Wall
        -fdata-sections
        -ffunction-sections
)

#флаги компиляции для МК (blue_pill)
set(CPU_FLAGS_blue 
        -mcpu=cortex-m3 
        -mfloat-abi=soft
)

#флаги компиляции для МК (black_pill)
set(CPU_FLAGS_black 
        -mcpu=cortex-m4 
        -mfloat-abi=hard
        -mfpu=fpv4-sp-d16
)

#флаги линковки для МК (blue_pill)
set(LINK_FLAGS_blue
        -T${CMAKE_SOURCE_DIR}/blue_pill/Ld/linker_script.ld
        -Wl,--gc-sections
)

#флаги линковки для МК (black_pill)
set(LINK_FLAGS_black
        -T${CMAKE_SOURCE_DIR}/black_pill/Ld/linker_script.ld
        -Wl,--gc-sections
)

# add_compile_options(
#     ${OPTIMIZ_FLAGS}
# )

# эта конструция не нужна так как использую настройку линковки через target_link_options
# из=за того что несколько проектов собирает CMake
# add_link_options(
#     ${CPU_FLAGS_blue}
#     ${LINK_FLAGS_blue}
# )

#сохраняю в переменную BLUE_PILL_SOURCES список файлов исходников
set(BLUE_PILL_SOURCES
        blue_pill/Src/c13Led_On.c
        blue_pill/Src/clockMCU.c
        blue_pill/Src/main.c
        blue_pill/Startup/startup.c
)

#сохраняю в переменную BLACK_PILL_SOURCES список файлов исходников
set(BLACK_PILL_SOURCES
        black_pill/Src/main.c
        black_pill/Src/NVIC_EnableIRQ.c
        black_pill/Src/initClockingMod.c
        black_pill/Src/initFlash.c
        black_pill/Src/led_light.c
        black_pill/Src/configTIM1.c
        black_pill/Src/configPWM.c
        black_pill/Src/configPWM_TIM_DMA.c
        # black_pill/Src/InitTim3.c
        black_pill/Startup/startup.c

)

add_executable(blue_pill.elf
    ${BLUE_PILL_SOURCES}
)

add_executable(black_pill.elf
    ${BLACK_PILL_SOURCES}
)

#дефайн под МК
target_compile_definitions(blue_pill.elf PRIVATE STM32F103xB)

target_compile_definitions(black_pill.elf PRIVATE STM32F411xE)

target_compile_options(blue_pill.elf PRIVATE ${CPU_FLAGS_blue})

target_compile_options(black_pill.elf PRIVATE ${CPU_FLAGS_black})

target_link_options(blue_pill.elf PRIVATE
    ${CPU_FLAGS_blue}
    ${LINK_FLAGS_blue}
)

target_link_options(black_pill.elf PRIVATE
    ${CPU_FLAGS_black}
    ${LINK_FLAGS_black}
)

# создание переменной COMMON_INCLUDES_BLUE и запись в нее общих исходников для F103
set(COMMON_INCLUDES_BLUE
    "C:/Users/set/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.6/Drivers/CMSIS/Core/Include"
    "C:/Users/set/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.6/Drivers/CMSIS/Device/ST/STM32F1xx/Include"
)
# Добавление исходников для платы blue_pill
target_include_directories(blue_pill.elf PRIVATE
    blue_pill/Inc
    ${COMMON_INCLUDES_BLUE}
)

# создание переменной COMMON_INCLUDES_BLACK и запись в нее общих исходников для F411
set(COMMON_INCLUDES_BLACK
    "H:/Repositoris STM/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Include"
    "H:/Repositoris STM/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Core/Include"
    "H:/Repositoris STM/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include"
)
# Добавление исходников для платы black_pill
target_include_directories(black_pill.elf PRIVATE
    black_pill/Inc
    ${COMMON_INCLUDES_BLACK}
)

# для сборки blue_pill
add_custom_command(TARGET blue_pill.elf POST_BUILD

    COMMAND arm-none-eabi-objcopy -O ihex
        $<TARGET_FILE:blue_pill.elf>
        $<TARGET_FILE_DIR:blue_pill.elf>/blue_pill.hex

    COMMAND arm-none-eabi-objcopy -O binary
        $<TARGET_FILE:blue_pill.elf>
        $<TARGET_FILE_DIR:blue_pill.elf>/blue_pill.bin

    COMMENT "Generating HEX and BIN for blue_pill"
)
add_custom_command(TARGET blue_pill.elf POST_BUILD
    COMMAND arm-none-eabi-size $<TARGET_FILE:blue_pill.elf>

    COMMENT "Calculating size of blue_pill.elf"
)

# для сборки black_pill
add_custom_command(TARGET black_pill.elf POST_BUILD

    COMMAND arm-none-eabi-objcopy -O ihex
        $<TARGET_FILE:black_pill.elf>
        $<TARGET_FILE_DIR:black_pill.elf>/black_pill.hex

    COMMAND arm-none-eabi-objcopy -O binary
        $<TARGET_FILE:black_pill.elf>
        $<TARGET_FILE_DIR:black_pill.elf>/black_pill.bin

    COMMENT "Generating HEX and BIN for black_pill"
)
add_custom_command(TARGET black_pill.elf POST_BUILD
    COMMAND arm-none-eabi-size $<TARGET_FILE:black_pill.elf>

    COMMENT "Calculating size of black_pill.elf"
)

# сборка проекта без линковки

set(COMMON_INCLUDES_BLACK_NOT_LINK
    "H:/Repositoris STM/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Include"
    "H:/Repositoris STM/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Core/Include"
    "H:/Repositoris STM/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include"
)
add_library(no_link_objects OBJECT
     black_pill/Src/main.c
     black_pill/Src/initClockingMod.c
)
target_include_directories(no_link_objects PRIVATE
    ${COMMON_INCLUDES_BLACK_NOT_LINK}
    black_pill/Inc
)
target_compile_options(no_link_objects PRIVATE
    ${CPU_FLAGS_black}
    ${OPTIMIZ_FLAGS}
)

target_compile_definitions(no_link_objects PRIVATE
    STM32F411xE
)