#reference: The Linux Kernel Module Programming Guide, Peter Jay Salzman, Michael Burian, Ori Pomerantz, Bob Mottram, Jim Huang
obj-m += kernelmod.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean