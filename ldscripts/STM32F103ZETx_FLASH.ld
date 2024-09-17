ENTRY(start_rtthread_kernel)

MEMORY
{
    RAM (xrw)      : ORIGIN = 0x20000400, LENGTH = 60K
    FLASH (rx)      : ORIGIN = 0x08020040, LENGTH = 200K
}

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    .text :
    {
        . = ALIGN(8);
        *(.text)
        *(.text*)
        . = ALIGN(8);
    } >FLASH

    .msh :
    {
        . = ALIGN(8);
        __fsymtab_start = .;
        KEEP(*(FSymTab))
        __fsymtab_end = .;
        . = ALIGN(8);

    } >FLASH
    .components :
    {
        . = ALIGN(8);
        __rt_init_start = .;
        KEEP(*(SORT(.rti_fn*)))
        __rt_init_end = .;
        . = ALIGN(8);
    } >FLASH

    .rodata :
    {
        . = ALIGN(8);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(8);
    } >FLASH

    _sidata = LOADADDR(.data);

    .data : 
    {
        . = ALIGN(8);
        _sdata = .;
        *(.data)
        *(.data*)

        . = ALIGN(8);
        _edata = .;
    } >RAM AT> FLASH

    . = ALIGN(4);
    .bss :
    {
        _sbss = .;
        *(.bss)
        *(.bss*)
        . = ALIGN(4);
        _ebss = .;
    } >RAM
    . = ALIGN(8);
    end = .;
}

PROVIDE(_stack = ORIGIN(RAM) + LENGTH(RAM));
