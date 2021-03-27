
all: src/test_main.c src/tokenizer.h
	$(CC) src/test_main.c -o test.exe

run: all
	./test.exe
