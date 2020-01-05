#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source;C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/kernel/tirtos/packages;C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack
override XDCROOT = C:/ti/ccs930/xdctools_3_60_02_34_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source;C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/kernel/tirtos/packages;C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack;C:/ti/ccs930/xdctools_3_60_02_34_core/packages;..
HOSTOS = Windows
endif
