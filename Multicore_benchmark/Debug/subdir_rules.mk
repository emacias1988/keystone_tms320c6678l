################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
configPkg/linker.cmd: ../omp_config.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_25_03_72/xs" --xdcpath="C:/ti/omp_1_01_03_02/packages;C:/ti/bios_6_35_04_50/packages;C:/ti/ipc_1_24_03_32/packages;C:/ti/pdk_C6678_1_1_2_6/packages;C:/ti/ccsv5/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.elf.C66 -p ti.omp.examples.platforms.evm6678 -r debug -c "C:/ti/ccsv5/tools/compiler/c6000_7.4.4" "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/compiler.opt: | configPkg/linker.cmd
configPkg/: | configPkg/linker.cmd

omp_hello.obj: ../omp_hello.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6600 --abi=eabi -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --include_path="C:/Users/a0272339/Documents/UTD/uP/keystone_tms320c6678l/Multicore_benchmark/mathlib_tests" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages/ti/platform" --include_path="C:/ti/mathlib_c66x_3_0_1_1/packages" --display_error_number --diag_warning=225 --diag_wrap=off --openmp --preproc_with_compile --preproc_dependency="omp_hello.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

tables.obj: ../tables.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6600 --abi=eabi -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --include_path="C:/Users/a0272339/Documents/UTD/uP/keystone_tms320c6678l/Multicore_benchmark/mathlib_tests" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages/ti/platform" --include_path="C:/ti/mathlib_c66x_3_0_1_1/packages" --display_error_number --diag_warning=225 --diag_wrap=off --openmp --preproc_with_compile --preproc_dependency="tables.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


