obj-m := interrupt.o
all:
	make -C /lib/modules/5.15.0-53-generic/build M=$(PWD) modules
clean:
	make -C /lib/modules/5.15.0-53-generic/build M=$(PWD) clean

