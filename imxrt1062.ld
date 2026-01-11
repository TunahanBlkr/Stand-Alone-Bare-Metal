MEMORY
{
    ITCM (rwx):  ORIGIN = 0x00000000, LENGTH = 128K
	DTCM (rwx):  ORIGIN = 0x20000000, LENGTH = 128K
	RAM (rwx):   ORIGIN = 0x20200000, LENGTH = 256K
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

        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.text.itcm));
        LONG(    ADDR(.text.itcm));
        LONG(  SIZEOF(.text.itcm));
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        __bss_section_table_end = .;
        __section_table_end = . ;

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

    .text.itcm : ALIGN(4)
    {
        FILL(0xff)
        __itcm_start__ = . ;
        *(.ramfunc*)
        . = ALIGN(32) ;
        __itcm_end__ = . ;
     } > ITCM AT>FLASH

    .uninit_RESERVED (NOLOAD) : ALIGN(4)
    {
        _start_uninit_RESERVED = .;
        KEEP(*(.bss.$RESERVED*))
       . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > DTCM AT> DTCM

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
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        __bss_end__ = .;
    } > DTCM AT> DTCM

    .noinit (NOLOAD): ALIGN(4)
    {
        __noinit_start__ = .;
        *(.noinit*)
         . = ALIGN(4) ;
        __noinit_end__ = .;
    } > DTCM AT> DTCM

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

    _HeapSize = 0x3000;
    .heap :  ALIGN(4)
    {
        _pvHeapStart = .;
        . += _HeapSize;
        . = ALIGN(4);
        _pvHeapLimit = .;
    } > DTCM

     _StackSize = 0x2000;
    .heap2stackfill  :
    {
        . += _StackSize;
    } > DTCM

    .stack ORIGIN(DTCM) + LENGTH(DTCM) - _StackSize - 0:  ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);
        _vStackTop = . + _StackSize;
    } > DTCM
	
	_teensy_model_identifier = 0x25;

	.debug_info     0 : { *(.debug_info) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
}