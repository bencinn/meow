all: main

main: main.c lex.yy.c y.tab.c minify.o generator.c
	gcc -O2 -Wall -Wpedantic -Wextra -o main main.c log.c lex.yy.c y.tab.c minify.o generator.c

minify.o: minify.json
	ld -r -b binary minify.json -o minify.o

y.tab.c y.tab.h: meow.y
	yacc -d -t -Wcounterexample meow.y

lex.yy.c: meow.l y.tab.h
	lex meow.l

clean:
	-rm y.tab.c y.tab.h lex.yy.c minify.json minify.o out/* out.sb3 main

out.sb3: test.meow main
	-mkdir out
	-rm out/*
	./main ./test.meow ./out
	zip -r out.sb3 ./out/*

minify.json: minimal.json
	jq --compact-output < minimal.json > minify.json

.PHONY: test.meow
