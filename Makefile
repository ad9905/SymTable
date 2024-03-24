# Dependency rules for non-file targets
all: testsymtablelist testsymtablehash
clobber: clean
	rm -f *~ \#*\#
clean:
	rm -f testsymtablelist testsymtablehash *.o

# Dependency rules for file targets
testsymtablelist: testsymtable.o symtablelist.o
	gcc217 testsymtable.o symtablelist.o -o testsymtablelist

testsymtablehash: testsymtable.o symtablehash.o
	gcc217 testsymtable.o symtablehash.o -o testsymtablehash

testsymtable.o: testsymtable.c symtablelist.c symtablehash.c
	gcc217 -c testsymtable.c

symtablelist.o: symtablelist.c
	gcc217 -c symtablelist.c

symtablehash.o: symtablehash.c
	gcc217 -c symtablehash.c


