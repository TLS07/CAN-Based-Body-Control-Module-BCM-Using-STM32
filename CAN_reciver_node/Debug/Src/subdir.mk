################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/I2C_drivers.c \
../Src/func.c \
../Src/init.c \
../Src/lcd_drivers.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/I2C_drivers.o \
./Src/func.o \
./Src/init.o \
./Src/lcd_drivers.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/I2C_drivers.d \
./Src/func.d \
./Src/init.d \
./Src/lcd_drivers.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/I2C_drivers.cyclo ./Src/I2C_drivers.d ./Src/I2C_drivers.o ./Src/I2C_drivers.su ./Src/func.cyclo ./Src/func.d ./Src/func.o ./Src/func.su ./Src/init.cyclo ./Src/init.d ./Src/init.o ./Src/init.su ./Src/lcd_drivers.cyclo ./Src/lcd_drivers.d ./Src/lcd_drivers.o ./Src/lcd_drivers.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

