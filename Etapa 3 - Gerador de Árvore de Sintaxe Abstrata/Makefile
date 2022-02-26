etapa3: parser.tab.c lex.yy.c
	gcc lex.yy.c -o etapa3

parser.tab.c: parser.y
	bison parser.y -d

lex.yy.c: scanner.l 
	flex scanner.l

clean:
	rm etapa3 lex.yy.c parser.tab.c