# meaty-skeleton

this is a proof of concept fork of osdev's meaty-skeleton, in process of being transformed into a real operating system  

this OS is not supposed to show my skills of programming nor my knowledge of how computers work and whatnot. this OS is a PoC showing (with proof) the capability of Artificial Intelligence when writing code. so far about 25% of code was written by ChatGPT, most of it worked first try which is pretty insane. what I'm trying to prove is that AI will soon replace either programmers or the knowledge base of programmers. this project is an example of using AI's skills instead of your own (assuming you have any).

## screenshots or something  
![1](readme/1.png)
![2](readme/2.jpg)  

## roadmap
- [x] boot kernel from grub
- [x] kernel
- [x] io operations [ai]
- [x] printing functions
- [x] remapping the pic [ai]
- [x] basic keyboard handling [ai]
- [x] libc (not full obviously) [½ ai]
- [x] math library [ai]
- [x] gdt [ai]
- [x] idt [ai]
- [x] isr [ai]
- [x] serial
- [ ] pit
- [ ] irq
- [x] framebuffer
- [x] basic graphics functions
- [ ] double buffering
- [ ] multithreading
- [x] higher half kernel
- [ ] rings
- [ ] permissions
- [x] paging
- [ ] memory map
- [ ] memory manager

## build and run
you need the i686-elf- toolchain, check osdev wiki for instructions on building it  
you also need objcopy  

### building

```
cd meaty-skeleton/kernel/arch/i386
objcopy -O elf32-i386 -B i386 -I binary font.psf font.o
cd ../../../
chmod +x qemu.sh
```

### running

```
./qemu.sh
```
