assembler: assembler.o
	gcc -Wall -pedantic assembler.o service/preAssembler.c service/assembler.c service/assemblyEncoder.c structs/list.c structs/shortData.c structs/symbol.c \
util/assemblyUtils.c util/constantBuilderUtils.c util/readerUtils.c validators/assemblyValidator.c validators/fileValidator.c validators/progValidator.c io/messagesHandler.c enums/enums.c -o assembler
assembler.o: main.c
	gcc -c -ansi -Wall -pedantic main.c -o assembler.o