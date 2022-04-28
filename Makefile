assgnmake:
	lex lexical.l
	yacc -d parser.y 
	gcc lex.yy.c y.tab.c code_gen.c ast.c lst.c -g -o output
	./output < $(input)
	lex label_scan.l
	gcc lex.yy.c LAT.c -g -o LabelTranslate
	./LabelTranslate
clean:
	rm *.xsm y.tab.h y.tab.c lex.yy.c output LabelTranslate
