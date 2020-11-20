# // Makefile 

CC=gcc
CFLAGS= -std=c11 -pedantic -Wall -Wextra

#preklad hlavnych casti
#--------------------------------------
all:

#testy
#--------------------------------------
test_lex: test-lex
test_symtable: test-symtable
test_parser: test-parser

#preklad testovacich programov
test-lex: test-lex.o str.o scanner.o
	$(CC) $(CFLAGS) -g -o $@ $^

test-symtable: test-symtable.o symtable.o str.o scanner.o
	$(CC) $(CFLAGS) -g -o $@ $^

test-parser: test-parser.o parser.o symtable.o str.o scanner.o 
	$(CC) $(CFLAGS) -g -o $@ $^
#--------------------------------------
#objektove subory

scanner.o: scanner.c scanner.h str.h error.h
	$(CC) $(CFLAGS) -o $@ -c $<

str.o: str.c str.h
	$(CC) $(CFLAGS) -o $@ -c $<

test-lex.o: tests/test-lex.c scanner.h str.h error.h
	$(CC) $(CFLAGS) -o $@ -c $<

symtable.o: symtable.c symtable.h str.h
	$(CC) $(CFLAGS) -o $@ -c $<

test-symtable.o: tests/test-symtable.c symtable.h str.h error.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c $<

parser.o: parser.c parser.h symtable.h str.h error.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c $<

test-parser.o: tests/test-parser.c parser.h symtable.h str.h error.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c $<

#ostatne prikazy
.PHONY: clean clean-all

clean:
	rm -f *.o

clean-all:
	rm -f *.o test-lex test-symtable test-parser