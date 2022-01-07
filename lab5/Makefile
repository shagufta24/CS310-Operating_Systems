# -*- MakeFile -*-

all: heft-driver

heft-driver: heft-driver.o
	gcc heft-driver.o -o heft-driver

heft-driver.o: heft-driver.c heft.c
	gcc -c heft-driver.c -c heft-driver.o

clean:
	rm -fr *.o heft-driver

clean-temp:
	rm -fr *.o