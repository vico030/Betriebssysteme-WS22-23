
### start qemu on andorra
`LD_LIBRARY_PATH=/usr/local/lib:/import/sage-7.4/local/lib/ && export LD_LIBRARY_PATH`

`qemu-bsprak -kernel kernel.elf`

### obj dump
`arm-none-eabi-objdump -fhd kernel.elf`

### start qemu with kernel local
`./arm-softmmu/qemu-system-arm \
-nographic -M portux920t -m 64M -kernel *path to kernel*`

### Local Execution
`../qemu-build/arm-softmmu/qemu-system-arm -nographic -M portux920t -m 64M -kernel kernel.elf`

### Local Debug
`../qemu-build/arm-softmmu/qemu-system-arm -nographic -M portux920t -m 64M -kernel kernel.elf -S -gdb tcp::4444`

`arm-none-eabi-gdb kernel.elf`

`(gdb) target remote :4444`
