#CFLAGS= -Wall -O2
CFLAGS=  -O -fPIC -shared 
CC=mipsel-linux-gcc
COMMAND=-L/yuan/pro/https/lib  -lssl -lcrypto
#COMMAND=-L/yuan/pro/practice/aes/lib-pc -lcrypto -lssl
obj +=bngo_aes.o URLEncode.o
TARGET = libbngo_aes.so
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o 
