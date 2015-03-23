-c
-heap  0x2000
-stack 0x2000


/* Memory Map 1 - the default */
MEMORY
{
        L1D:     o = 00f00000h   l = 00007FFFh 
        L1P:     o = 00e00000h   l = 00007FFFh 
        MSMC:    o = 0C000000h   l = 001FFFFFh 
        L2SRAM:  o = 10800000h   l = 00080000h
        /* Can be accessed by the core, in the case an external interface (i.e. EDMA)
        it needs to use the global address specific to the core */
        //L2SRAM_L :o = 00820000h	 l = 00020000h
} 

SECTIONS
{
    .csl_vect   >       L2SRAM
    .text       >       L2SRAM
    .stack      >       L2SRAM
    .bss        >       L2SRAM
    .cinit      >       L2SRAM
    .cio        >       L2SRAM
    .const      >       L2SRAM
    .data       >       L2SRAM
    .switch     >       L2SRAM
    .sysmem     >       L2SRAM
    .far        >       L2SRAM
    .testMem    >       L2SRAM
    .fardata    >       L2SRAM
    .neardata   >       L2SRAM
    .rodata     >       L2SRAM
    
}
