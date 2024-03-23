
TARGET=ftpc

all: $(TARGET)

$(TARGET): utils.o ftpc.o
	ld -m elf_i386 -dynamic-linker /lib/ld-linux.so.2 -L. utils.o ftpc.o -o ftpc /usr/lib/gcc/i386-redhat-linux/3.4.3/../../../crt1.o /usr/lib/gcc/i386-redhat-linux/3.4.3/../../../crti.o /usr/lib/gcc/i386-redhat-linux/3.4.3/crtbegin.o -L/usr/lib/gcc/i386-redhat-linux/3.4.3 -L/usr/lib/gcc/i386-redhat-linux/3.4.3 -L/usr/lib/gcc/i386-redhat-linux/3.4.3/../../.. -lc /usr/lib/gcc/i386-redhat-linux/3.4.3/crtend.o

utils.o: utils.c
	gcc -DHAVE_CONFIG_H -I. -O2 -g -c -o utils.o utils.c

ftpc.o: ftpc.c
	gcc -DHAVE_CONFIG_H -I. -O2 -g -c -o ftpc.o ftpc.c

clean:
	rm *.o $(TARGET) libutils.so

