#
#
#
CFLAGS= -Wall

all:	rxbind

install: rxbind

clean:
	rm -f rxbind rxbind.o

run:	rxbind
	sudo ./rxbind

rxbind: rxbind.o
	$(CC) -o rxbind rxbind.o -lpigpio -lrt -lpthread
