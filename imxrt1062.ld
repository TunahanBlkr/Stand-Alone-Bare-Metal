MEMORY
{
    ITCM (rwx):  ORIGIN = 0x00000000, LENGTH = 512K
	DTCM (rwx):  ORIGIN = 0x20000000, LENGTH = 512K
	OCRAM (rwx): ORIGIN = 0x20200000, LENGTH = 512K
	FLASH (rwx): ORIGIN = 0x60000000, LENGTH = 7936K
}

ENTRY(ImageVectorTable)

SECTIONS
{
    .boot_hdr : ALIGN(4)
    {
        __boot_hdr_start__ = ABSOLUTE(.) ;
        KEEP(*(.boot_hdr.conf))
        FILL(0xff)
        . = 0x1000 ;
        KEEP(*(.boot_hdr.ivt))
        . = 0x1020 ;
        KEEP(*(.boot_hdr.boot_data))
        __boot_hdr_end__ = ABSOLUTE(.) ;
        . = 0x2000 ;
    } >FLASH

    .text : ALIGN(4)
    {
        FILL(0xff)
        . = ALIGN(1024);
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        . = ALIGN(4) ;
        *(.startup)
        *(.text*)
        /* FreeRTOS Debug Config unutma */
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
    } > FLASH

    .preinit_array : ALIGN(4)
    {
        __preinit_array_start = .;
        KEEP(*(.preinit_array*))
        __preinit_array_end = .;
    } > FLASH

    .init_array : ALIGN(4)
    {
        __init_array_start = .;
        KEEP(*(.init_array*))
        __init_array_end = .;
    } > FLASH

    .ARM.extab : ALIGN(4)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .ARM.exidx : ALIGN(4)
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH

    .text.itcm : ALIGN(4)
    {
        FILL(0xff)
        __ram_func_start__ = . ;
        *(.ramfunc*)
        . = ALIGN(32) ;
        __ram_func_end__ = . ;
     } > ITCM AT>FLASH

    .data : ALIGN(4)
    {
       FILL(0xff)
       __data_start__ = . ;
       *(vtable)
       *(.data*)
       . = ALIGN(4) ;
       __data_end__ = . ;
    } > DTCM AT>FLASH

    .bss : ALIGN(4)
    {
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
    } > DTCM AT> DTCM

    .noinit (NOLOAD): ALIGN(4)
    {
        __noinit_start__ = .;
        *(.noinit*)
         . = ALIGN(4) ;
        __noinit_end__ = .;
    } > DTCM AT> DTCM

    DMA_MAX_SIZE = 0x1000;
    .noncacheable_region (NOLOAD) :
    {
        . = ALIGN(DMA_MAX_SIZE);
        __dma_buffer_start__ = .;
        *(.dma_buffer*)
        . = ALIGN(32);
        __dma_buffer_end__ = .;
    } > OCRAM AT> OCRAM

    __itcm_start__ = ADDR(.text.itcm);
    __itcm_end__   = ADDR(.text.itcm) + SIZEOF(.text.itcm);
    __itcm_load__  = LOADADDR(.text.itcm);

    __dtcm_start__ = ADDR(.data);
    __dtcm_end__   = ADDR(.data) + SIZEOF(.data);
    __dtcm_load__  = LOADADDR(.data);

    __bss_start__  = ADDR(.bss);
    __bss_end__    = ADDR(.bss) + SIZEOF(.bss);

    _pvHeapStart   = ORIGIN(OCRAM) + DMA_MAX_SIZE + 0x1000;
    _pvHeapLimit   = ORIGIN(OCRAM) + LENGTH(OCRAM);

    __dma_buffer_size__ = __dma_buffer_end__ - __dma_buffer_start__;

    __itcm_block_count__    = (SIZEOF(.text.itcm) + 0x7FFF) >> 15;
    __flexram_bank_config__ = 0xAAAAAAAA | ((1 << (__itcm_block_count__ * 2)) - 1);

    _vStackTop = ORIGIN(DTCM) + ((16 - __itcm_block_count__) << 15);
	
	_teensy_model_identifier = 0x25;

	.debug_info     0 : { *(.debug_info) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }

    ASSERT(__dma_buffer_size__ <= DMA_MAX_SIZE, "error: dma_buffer is too big")
}