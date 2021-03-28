CFLAGS=-ggdb -Og
all: src/test_main.c src/tokenizer.h
	$(CC) $(CFLAGS) src/test_main.c -o test.exe

run: all
	./test.exe
