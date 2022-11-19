
### start qemu on andorra
qemu-bsprak -kernel kernel.elf

### obj dump
arm-none-eabi-objdump -fhd kernel

### start qemu with kernel local
./arm-softmmu/qemu-system-arm \
-nographic -M portux920t -m 64M -kernel *path to kernel*

### set LD_LIBRARY_PATH
 LD_LIBRARY_PATH=/usr/local/lib:/import/sage-7.4/local/lib/ && export LD_LIBRARY_PATH

### Local Execution
../qemu-build/arm-softmmu/qemu-system-arm -nographic -M portux920t -m 64M -kernel kernel.elf

