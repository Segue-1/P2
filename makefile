CC = gcc
IDIR =./include
CFLAGS = -I$(IDIR) -g -Wall -lm

EXEC1 = main
OBJS1 = main.o

SHARE = main.o scanner.o testScanner.o stack.o

DEPS = scanner.h testScanner.h stack.h 

DEPS2 = token.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: main

main: $(OBJS1) $(SHARE) $(DEPS2)
	gcc -o $(EXEC1) $^ $(CFLAGS)

clean:
	rm $(EXEC1) $(OBJS1) $(SHARE)

