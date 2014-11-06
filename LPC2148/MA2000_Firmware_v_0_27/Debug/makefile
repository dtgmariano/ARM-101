################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Modules/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
MA2000_Firmware_v_0_27.hex \

SECONDARY_LIST += \
MA2000_Firmware_v_0_27.lst \

SECONDARY_SIZE += \
MA2000_Firmware_v_0_27.siz \


# All Target
all: MA2000_Firmware_v_0_27.elf secondary-outputs

# Tool invocations
MA2000_Firmware_v_0_27.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Yagarto Windows GCC C Linker'
	arm-none-eabi-gcc -T"../lpc2148_rom.ld" -nostartfiles -Wl,-Map,MA2000_Firmware_v_0_27.map -mcpu=arm7tdmi -g3 -gdwarf-2 -o"MA2000_Firmware_v_0_27.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

MA2000_Firmware_v_0_27.hex: MA2000_Firmware_v_0_27.elf
	@echo 'Invoking: ARM Yagarto Windows GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex MA2000_Firmware_v_0_27.elf "MA2000_Firmware_v_0_27.hex"
	@echo 'Finished building: $@'
	@echo ' '

MA2000_Firmware_v_0_27.lst: MA2000_Firmware_v_0_27.elf
	@echo 'Invoking: ARM Yagarto Windows GNU Create Listing'
	arm-none-eabi-objdump -h -S -C MA2000_Firmware_v_0_27.elf >"MA2000_Firmware_v_0_27.lst"
	@echo 'Finished building: $@'
	@echo ' '

MA2000_Firmware_v_0_27.siz: MA2000_Firmware_v_0_27.elf
	@echo 'Invoking: ARM Yagarto Windows GNU Print Size'
	arm-none-eabi-size  --format=berkeley -x MA2000_Firmware_v_0_27.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(EXECUTABLES)$(SECONDARY_LIST)$(S_UPPER_DEPS) MA2000_Firmware_v_0_27.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
