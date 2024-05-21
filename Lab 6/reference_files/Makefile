#obj-m += hello1.o
obj-m += hello2.o
#obj-m += my_chardev.o
#obj-m += my_chardev_mem.o
#obj-m += my_chardev_lock.o
#obj-m += my_chardev_block.o
#obj-m += irq_test.o
#obj-m += exam_dev.o


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean 
	-rm *~ Module.markers modules.order
