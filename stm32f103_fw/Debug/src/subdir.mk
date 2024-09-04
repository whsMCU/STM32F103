################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c 

OBJS += \
./src/main.o 

C_DEPS += \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o src/%.su src/%.cyclo: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src/bsp" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src/common" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src/hw" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/CMSIS/Include" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/STM32F1xx_HAL_Driver/Inc/Legacy" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src

clean-src:
	-$(RM) ./src/main.cyclo ./src/main.d ./src/main.o ./src/main.su

.PHONY: clean-src

