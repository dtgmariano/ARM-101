################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MA2000/functions.c \
../MA2000/robot.c 

OBJS += \
./MA2000/functions.o \
./MA2000/robot.o 

C_DEPS += \
./MA2000/functions.d \
./MA2000/robot.d 


# Each subdirectory must supply rules for building sources it contributes
MA2000/%.o: ../MA2000/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -I"C:\ARM\PROGRAMAS\LPC2148\MA2000_Firmware_v_0_24" -O0 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


