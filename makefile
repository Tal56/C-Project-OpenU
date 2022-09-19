assembler: prog.o pre_processor.o first_pass.o lable_functions.o second_pass.o decode.o
        gcc -g -ansi -Wall -pedantic prog.o pre_processor.o first_pass.o lable_functions.o second_pass.o decode.o -lm -o assembler

prog.o: prog.c pre_processor.h prototypes.h assembler.h structs.h decode.h
        gcc -c -ansi -Wall -pedantic prog.c -o prog.o

pre_processor.o: pre_processor.c pre_processor.h structs.h
        gcc -c -ansi -Wall -pedantic pre_processor.c -o pre_processor.o

first_pass.o: first_pass.c prototypes.h assembler.h structs.h decode.h
        gcc -c -ansi -Wall -pedantic first_pass.c -o first_pass.o

lable_functions.o: lable_functions.c prototypes.h assembler.h structs.h decode.h
        gcc -c -ansi -Wall -pedantic lable_functions.c -o lable_functions.o

decode.o: decode.c prototypes.h assembler.h structs.h decode.h
        gcc -c -ansi -Wall -pedantic decode.c -o decode.o

second_pass.o: second_pass.c prototypes.h assembler.h structs.h
        gcc -c -ansi -Wall -pedantic second_pass.c -o second_pass.o

