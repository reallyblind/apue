CFLAGS+=-pthread
LDFLAGS+=-pthread

all:mysemaphore

mysemaphore:main.o mysemaphore.o
	gcc $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o mysemaphore

