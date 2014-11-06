################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := cs-rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
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
LPC214x_USB.hex \

SECONDARY_LIST += \
LPC214x_USB.lst \

SECONDARY_SIZE += \
LPC214x_USB.siz \


# All Target
all: LPC214x_USB.elf secondary-outputs

# Tool invocations
LPC214x_USB.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Windows GCC C Linker'
	arm-none-eabi-gcc -T"../lpc2148-rom.ld" -nostartfiles -Wl,-Map,LPC214x_USB.map -mcpu=arm7tdmi-s -g3 -gdwarf-2 -o "LPC214x_USB.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

LPC214x_USB.hex: LPC214x_USB.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex LPC214x_USB.elf  "LPC214x_USB.hex"
	@echo 'Finished building: $@'
	@echo ' '

LPC214x_USB.lst: LPC214x_USB.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Listing'
	arm-none-eabi-objdump -h -S -C LPC214x_USB.elf > "LPC214x_USB.lst"
	@echo 'Finished building: $@'
	@echo ' '

LPC214x_USB.siz: LPC214x_USB.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Print Size'
	arm-none-eabi-size  --format=berkeley -x LPC214x_USB.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(EXECUTABLES)$(SECONDARY_LIST)$(S_UPPER_DEPS) LPC214x_USB.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
