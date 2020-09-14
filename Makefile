
CC = g++
OBJS = Huffman.o main.o Node.o
EXEC = Huffman.exe
CFLAGS = -std=c++14

$(EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $@
	
Huffman.o: Huffman.cpp Huffman.h Node.h
main.o: main.cpp Huffman.h Node.h
Node.o: Node.cpp Node.h

clean:
	rm -rf $(OBJS)
