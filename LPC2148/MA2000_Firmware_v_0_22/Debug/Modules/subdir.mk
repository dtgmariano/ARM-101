################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/adc.c \
../Modules/gpio.c \
../Modules/iprintf.c \
../Modules/pwm.c \
../Modules/timer.c \
../Modules/uart.c 

OBJS += \
./Modules/adc.o \
./Modules/gpio.o \
./Modules/iprintf.o \
./Modules/pwm.o \
./Modules/timer.o \
./Modules/uart.o 

C_DEPS += \
./Modules/adc.d \
./Modules/gpio.d \
./Modules/iprintf.d \
./Modules/pwm.d \
./Modules/timer.d \
./Modules/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/%.o: ../Modules/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -I"C:\Dropbox\ARM\PROGRAMAS\LPC2148\MA2000_Firmware_v_0_22" -O0 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


