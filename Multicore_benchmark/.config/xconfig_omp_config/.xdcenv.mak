#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/omp_1_01_03_02/packages;C:/ti/bios_6_35_04_50/packages;C:/ti/ipc_1_24_03_32/packages;C:/ti/pdk_C6678_1_1_2_6/packages;C:/ti/ccsv5/ccs_base;C:/Users/a0272339/Documents/UTD/uP/keystone_tms320c6678l/Multicore_benchmark/.config
override XDCROOT = C:/ti/xdctools_3_25_03_72
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/omp_1_01_03_02/packages;C:/ti/bios_6_35_04_50/packages;C:/ti/ipc_1_24_03_32/packages;C:/ti/pdk_C6678_1_1_2_6/packages;C:/ti/ccsv5/ccs_base;C:/Users/a0272339/Documents/UTD/uP/keystone_tms320c6678l/Multicore_benchmark/.config;C:/ti/xdctools_3_25_03_72/packages;..
HOSTOS = Windows
endif
