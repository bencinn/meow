all: main

main: main.c lex.yy.c y.tab.c minimal.json.o generator.c
	gcc -O2 -g -Wall -Wpedantic -Wextra -o main main.c log.c lex.yy.c y.tab.c generator.c minimal.json.o

minimal.json.o: minimal.json
	ld -r -b binary minimal.json -o minimal.json.o

y.tab.c y.tab.h: meow.y
	yacc -d -t -Wcounterexample meow.y

lex.yy.c: meow.l y.tab.h
	lex meow.l

clean:
	-rm y.tab.c y.tab.h lex.yy.c minimal.json.o out/* out.sb3 main

out.sb3: test.meow main
	-rm out/*
	./main ./test.meow ./out
	zip -r out.sb3 ./out/*

.PHONY: minimal.json test.meow

