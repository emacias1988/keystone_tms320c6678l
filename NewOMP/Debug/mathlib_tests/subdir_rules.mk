################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
mathlib_tests/log.obj: ../mathlib_tests/log.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6600 --abi=eabi -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/Users/a0272339/Documents/UTD/uP/keystone/NewOMP" --include_path="C:/Users/a0272339/Documents/UTD/uP/keystone/NewOMP/mathlib_tests" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages/ti/platform" --include_path="C:/ti/mathlib_c66x_3_0_1_1/packages" --display_error_number --diag_warning=225 --diag_wrap=off --openmp --preproc_with_compile --preproc_dependency="mathlib_tests/log.pp" --obj_directory="mathlib_tests" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


