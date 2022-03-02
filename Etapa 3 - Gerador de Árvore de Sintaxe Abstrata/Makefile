etapa3: parser.tab.o lex.yy.o hash.o ast.o main.o
	gcc main.o ast.o hash.o parser.tab.o lex.yy.o -o etapa3

main.o: main.c
	gcc -c main.c

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
	rm lex.yy.c parser.tab.c *.o etapa3