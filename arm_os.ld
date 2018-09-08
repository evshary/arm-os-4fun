SECTIONS
{
    .text 0x08000000 :
    AT(0x08000000)
    {
        KEEP(*(.init_isr))
        *(.text)
        *(.rodata)
        _flash_start_data = .;
    }

    .data 0x20000000 :
    AT(LOADADDR(.text)+SIZEOF(.text))
    {
        _start_data = .;
        *(.data)
        _end_data = .;
    }

    .bss (ADDR(.data)+SIZEOF(.data)) :
    {
        _start_bss = .;
        *(.bss)
        _end_bss = .;
    }

    _end_stack = 0x20000000+40K;
}
