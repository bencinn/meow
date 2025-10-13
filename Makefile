main: main.c lex.yy.c y.tab.c minimal.json.o generator.c
	gcc -O2 -g -Wall -Wpedantic -Wextra -o main main.c log.c lex.yy.c y.tab.c generator.c minimal.json.o

minimal.json.o: minimal.json
	ld -r -b binary minimal.json -o minimal.json.o

y.tab.c y.tab.h: meow.y
	yacc -d -t -Wcounterexample meow.y

lex.yy.c: meow.l y.tab.h
	lex meow.l

.PHONY: minimal.json

