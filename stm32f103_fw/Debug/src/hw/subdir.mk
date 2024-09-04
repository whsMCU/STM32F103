################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hw/gpio.c \
../src/hw/tim.c 

OBJS += \
./src/hw/gpio.o \
./src/hw/tim.o 

C_DEPS += \
./src/hw/gpio.d \
./src/hw/tim.d 


# Each subdirectory must supply rules for building sources it contributes
src/hw/%.o src/hw/%.su src/hw/%.cyclo: ../src/hw/%.c src/hw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src/bsp" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src/common" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src/hw" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/CMSIS/Include" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/STM32F1xx_HAL_Driver/Inc/Legacy" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-hw

clean-src-2f-hw:
	-$(RM) ./src/hw/gpio.cyclo ./src/hw/gpio.d ./src/hw/gpio.o ./src/hw/gpio.su ./src/hw/tim.cyclo ./src/hw/tim.d ./src/hw/tim.o ./src/hw/tim.su

.PHONY: clean-src-2f-hw

