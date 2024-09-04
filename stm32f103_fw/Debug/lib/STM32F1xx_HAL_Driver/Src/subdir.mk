################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
../lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c 

OBJS += \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.o \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.o \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.o \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.o \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.o \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.o \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.o \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.o \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.o \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.o \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.o \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.o \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.o 

C_DEPS += \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.d \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.d \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.d \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.d \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.d \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.d \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.d \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.d \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.d \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.d \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.d \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.d \
./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.d 


# Each subdirectory must supply rules for building sources it contributes
lib/STM32F1xx_HAL_Driver/Src/%.o lib/STM32F1xx_HAL_Driver/Src/%.su lib/STM32F1xx_HAL_Driver/Src/%.cyclo: ../lib/STM32F1xx_HAL_Driver/Src/%.c lib/STM32F1xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src/bsp" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src/common" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src/hw" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/CMSIS/Include" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/STM32F1xx_HAL_Driver/Inc/Legacy" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-lib-2f-STM32F1xx_HAL_Driver-2f-Src

clean-lib-2f-STM32F1xx_HAL_Driver-2f-Src:
	-$(RM) ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.su ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.su ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.su ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.su ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.su ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.su ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.su ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.su ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.su ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.su ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.su ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.su ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.cyclo ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.d ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.o ./lib/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.su

.PHONY: clean-lib-2f-STM32F1xx_HAL_Driver-2f-Src

