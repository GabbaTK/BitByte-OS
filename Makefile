debug: clearbuild debugcompileefi debugcompilekernel createdisk copydatatodisk finishdisk
	qemu-system-x86_64 -drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF_CODE.fd -drive if=pflash,format=raw,file=/usr/share/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF_VARS.fd -drive format=raw,file=./bin/os.img -serial stdio -s -S

shell: clearbuild compileefi compilekernel
	qemu-system-x86_64 -drive format=raw,file=fat:rw:.,media=disk -bios /usr/share/ovmf/OVMF.fd -net none

qemu: clearbuild compileefi compilekernelqemu createdisk copydatatodisk finishdisk
#	qemu-system-x86_64 -bios /usr/share/OVMF/OVMF_CODE.fd -drive file=./bin/os.img,if=virtio,format=raw
	qemu-system-x86_64 -drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF_CODE.fd -drive if=pflash,format=raw,file=/usr/share/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF_VARS.fd -drive format=raw,file=./bin/os.img -serial stdio

all: clearbuild compileefi compilekernel createdisk copydatatodisk finishdisk
#	qemu-system-x86_64 -bios /usr/share/OVMF/OVMF_CODE.fd -drive file=./bin/os.img,if=virtio,format=raw

blankdeps:
	touch ./bin/bootloader/BOOTIA32.EFI
	touch ./bin/bootloader/bootloader32.efi

clearbuild:
#	find ./bin -maxdepth 1 -type f -delete
#	find ./bin/bootloader -maxdepth 1 -type f -delete
#	find ./bin/debug -maxdepth 1 -type f -delete
	rm -rf ./bin
	mkdir -p ./bin/bootloader
	mkdir -p ./bin/debug

debugcompileefi: compileefi
	x86_64-w64-mingw32-ld -nostdlib -e efi_main ./bin/bootloader/efi_main.o -o ./bin/debug/boot_debug.elf

compileefi:
	x86_64-w64-mingw32-gcc -Wall -Werror -m64 -mabi=ms -g -ffreestanding -c ./src/bootloader/efi_main.c -o ./bin/bootloader/efi_main.o -I/usr/share/edk2/MdePkg/Include -I/usr/share/edk2/MdePkg/Include/X64
	x86_64-w64-mingw32-gcc -Wall -Werror -m64 -mabi=ms -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e efi_main ./bin/bootloader/efi_main.o -o ./bin/bootloader/BOOTX64.EFI

compilemain:
#	gcc ./src/bootloader/main.c -c -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -I /usr/include/efi -I /usr/include/efi/x86_64 -o ./bin/bootloader/main.o
#	ld ./bin/bootloader/main.o /usr/lib/crt0-efi-x86_64.o -e efi_main -nostdlib -znocombreloc -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L /usr/lib -l:libgnuefi.a -l:libefi.a -o ./bin/bootloader/main.so
#	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64 ./bin/bootloader/main.so ./bin/bootloader/bootloader.efi
	gcc -I /usr/include/efi -I /usr/include/efi/x86_64 -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -DEFI_FUNCTION_WRAPPER -c ./src/bootloader/main.c -o ./bin/bootloader/main.o
	ld -nostdlib -znocombreloc -shared -Bsymbolic -T /usr/lib/elf_x86_64_efi.lds /usr/lib/crt0-efi-x86_64.o ./bin/bootloader/main.o -L/usr/lib -lgnuefi -lefi -o ./bin/bootloader/main.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64 ./bin/bootloader/main.so ./bin/bootloader/bootloader.efi

#compileefi32:
#	i686-w64-mingw32-gcc -Wall -Werror -m32 -mabi=ms -ffreestanding -c ./src/bootloader/efi_main.c -o ./bin/bootloader/efi_main32.o -I/usr/share/edk2/MdePkg/Include -I/usr/share/edk2/MdePkg/Include/X64
#	i686-w64-mingw32-gcc -Wall -Werror -m32 -mabi=ms -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e efi_main ./bin/bootloader/efi_main32.o -o ./bin/bootloader/BOOTIA32.EFI
##	clang -target i386-w64-mingw32 -ffreestanding -fno-builtin -fms-extensions -c ./src/bootloader/efi_main.c -o ./bin/bootloader/efi_main32.o -I/usr/share/edk2/MdePkg/Include -I/usr/share/edk2/MdePkg/Include/X64
##	lld-link /subsystem:efi_application /entry:efi_main /out:./bin/bootloader/bootloader32.efi ./bin/bootloader/efi_main32.o /dll /nodefaultlib
#
#compilemain32:
##	i686-w64-mingw32-gcc ./src/bootloader/main.c -c -fno-stack-protector -fpic -fshort-wchar -I /usr/include/efi -I /usr/include/efi/ia32 -DEFI_FUNCTION_WRAPPER -m32 -o ./bin/bootloader/main32.o
##	ld ./bin/bootloader/main32.o /usr/lib32/crt0-efi-ia32.o -e efi_main -nostdlib -znocombreloc -T /usr/lib32/elf_ia32_efi.lds -shared -Bsymbolic -L /usr/lib32 -l:libgnuefi.a -l:libefi.a -o ./bin/bootloader/main32.so
##	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-ia32 ./bin/bootloader/main32.so ./bin/bootloader/bootloader32.efi
#	i686-linux-gnu-gcc -ffreestanding -fshort-wchar -fpic -fno-stack-protector -mno-red-zone -I /usr/include/efi -I /usr/include/efi/ia32 -DEFI_FUNCTION_WRAPPER -c ./src/bootloader/main.c -o ./bin/bootloader/main.o
#	ld -defsym=_DYNAMIC=0 -nostdlib -znocombreloc -Bsymbolic -e efi_main -T /usr/lib32/elf_ia32_efi.lds /usr/lib32/crt0-efi-ia32.o ./bin/bootloader/main.o -L/usr/lib32 -lgnuefi -lefi -o ./bin/bootloader/main.so
#	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-ia32 ./bin/bootloader/main.so ./bin/bootloader/bootloader32.efi

compilekernelqemu:
	gcc -ffreestanding -DQEMU -c ./src/kernel/kernel.c -o ./bin/kernel.o -m64 -O2 -nostdlib -fno-pie -no-pie
	ld --oformat binary -T ./src/kernel/linker.ld -e _start -o ./bin/kernel.bin ./bin/kernel.o

debugcompilekernel: compilekernelqemu
	ld --oformat elf64-x86-64 -T ./src/kernel/linkerdebug.ld -e _start -o ./bin/debug/kernel.elf ./bin/kernel.o

compilekernel:
	gcc -ffreestanding -c ./src/kernel/kernel.c -o ./bin/kernel.o -m64 -O2 -nostdlib -fno-pie -no-pie
	ld --oformat binary -T ./src/kernel/linker.ld -e _start -o ./bin/kernel.bin ./bin/kernel.o
#	objcopy -O binary ./bin/kernel.elf ./bin/kernel.bin

createdisk:
	dd if=/dev/zero of=./bin/uefi.img bs=512 count=131072
	parted ./bin/uefi.img --script mklabel gpt
	parted ./bin/uefi.img --script mkpart ESP fat32 2048s 100%
	parted ./bin/uefi.img --script set 1 boot on
	# Format the ESP partition inside uefi.img (offset 2048*512=1048576)
	sudo losetup -f --show -o 1048576 ./bin/uefi.img > ./bin/loopdev.txt
	LOOPDEV=$$(cat ./bin/loopdev.txt); sudo mkfs.vfat -F 32 $$LOOPDEV; sudo losetup -d $$LOOPDEV; rm ./bin/loopdev.txt
	# Mount the ESP partition directly from uefi.img using offset
	sudo mkdir -p ./bin/mount
	sudo mount -o loop,offset=1048576 ./bin/uefi.img ./bin/mount

copydatatodisk:
#   Copy the bootloader
	sudo mkdir -p ./bin/mount/EFI/BOOT
	sudo cp ./bin/bootloader/BOOTX64.EFI ./bin/mount/EFI/BOOT/
#	sudo cp ./bin/bootloader/BOOTIA32.EFI ./bin/mount/EFI/BOOT/
#	sudo cp ./bin/bootloader/bootloader.efi ./bin/mount/
#	sudo cp ./bin/bootloader/bootloader32.efi ./bin/mount/
#   Copy the kernel
	sudo cp ./bin/kernel.bin ./bin/mount/

finishdisk:
	sudo umount ./bin/mount/
	mv ./bin/uefi.img ./bin/os.img

real: copydatatodisk
	xorriso -as mkisofs -iso-level 3 -full-iso9660-filenames -volid "ISOIMAGE" -output ./bin/BitByte.iso ./bin/mount
	rm -rf ./bin/mount
	mkdir ./bin/mount
