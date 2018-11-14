################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include driver/subdir.mk
-include CMSIS/EFM32GG/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Homework_version_2.axf

# Tool invocations
Homework_version_2.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -T "Homework_version_2.ld" -Xlinker --gc-sections -Xlinker -Map="Homework_version_2.map" --specs=nano.specs -o Homework_version_2.axf "./CMSIS/EFM32GG/startup_efm32gg.o" "./CMSIS/EFM32GG/system_efm32gg.o" "./driver/caplesense.o" "./driver/gpiointerrupt.o" "./driver/segmentlcd.o" "./emlib/em_acmp.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_core.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_lcd.o" "./emlib/em_lesense.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./emlib/em_usart.o" "./emlib/retargetio.o" "./emlib/retargetserial.o" "./src/IT_handlers.o" "./src/init.o" "./src/main.o" "./src/program_logic.o" "./src/segmentlcd_individual.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: Homework_version_2.hex'
	arm-none-eabi-objcopy -O ihex "Homework_version_2.axf" "Homework_version_2.hex"
	@echo ' '

	@echo 'Building bin file: Homework_version_2.bin'
	arm-none-eabi-objcopy -O binary "Homework_version_2.axf" "Homework_version_2.bin"
	@echo ' '

	@echo 'Building s37 file: Homework_version_2.s37'
	arm-none-eabi-objcopy -O srec "Homework_version_2.axf" "Homework_version_2.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "Homework_version_2.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Homework_version_2.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
