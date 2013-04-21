Object = parser.tab.o lex.yy.o command.o history.o assist.o jobs.o execute.o

shell: $(Object)
	gcc -o shell $(Object)

parser.tab.c:parser.y
	bison -d parser.y

lex.yy.c:token.l
	flex token.l

clean:
	rm -r *.o shell

