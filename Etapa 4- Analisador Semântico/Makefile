etapa4: parser.tab.o lex.yy.o hash.o ast.o semantic.o main.o
	gcc main.o lex.yy.o parser.tab.o hash.o ast.o semantic.o  -o etapa4

main.o: main.c
	gcc -c main.c

semantic.o: semantic.c
	gcc -c semantic.c

ast.o: ast.c
	gcc -c ast.c

hash.o: hash.c
	gcc -c hash.c

parser.tab.o: parser.tab.c
	gcc -c parser.tab.c

parser.tab.c: parser.y
	bison parser.y -d

lex.yy.o: lex.yy.c 
	gcc -c lex.yy.c

lex.yy.c: scanner.l 
	flex scanner.l

clean:
	rm lex.yy.c parser.tab.c parser.tab.h *.o etapa4