OUTPUT_FORMAT("binary")
ENTRY(_start)

SECTIONS
{
  . = 0x101000;  /* Load address, adjust if needed */
  .start : {
    *(.start)
  }

  .text : {
    *(.text)
  }
  .data : {
    *(.data)
  }
  .bss : {
    *(.bss)
  }
}
