#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "prototypes.h"
#include "decode.h"
#include "pre_processor.h"

/* We define global variables here because they are defined as extern,
   and therefore must be initialized in a specific C file.*/
 
unsigned int data[MACHINE_RAM];
unsigned int instructions[MACHINE_RAM];
int ic;
int dc;
int err;
labelPtr symbols_table;
extPtr ext_list;
boolean entry_exists, extern_exists, was_error;

const char base32[32] = {
        '!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c',
        'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u', 'v'};

const char *commands[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
        "red", "prn", "jsr", "rts", "hlt"
};

const char *directives[] = {
        ".data", ".string", ".struct", ".entry", ".extern"
};

void reset_global_vars()
{
    symbols_table = NULL;
    ext_list = NULL;

    entry_exists = FALSE;
    extern_exists = FALSE;
    was_error = FALSE;
}


/* This function handles all activities in the program, it receives command line arguments for filenames */
int main(int argc, char *argv[])
{
    int i;
    char *input_filename;
	char *output_filename;
    FILE *fp;
	FILE *file_am;
    for(i = 1; i < argc; i++)
    {
        printf("%s\n", argv[i]);
        input_filename = create_file_name(argv[i], FILE_INPUT); /* Appending .as to filename */
        printf("%s\n",input_filename);
        output_filename = create_file_name(argv[i], FILE_OUTPUT); /*Appending .am to filename*/
        printf("%s\n",output_filename);
        fp = fopen(input_filename, "r");
        if(fp != NULL) /* If file exists */
	    { 
            printf("Started %s assembling process\n", input_filename);
            reset_global_vars();
	        output_filename = strtok(argv[i], ".");
			/*strcat(output_filename , ".am");*/
			file_am = fopen(output_filename, "w+");
			PreWriteFile(fp, file_am,input_filename);
            rewind(file_am);
            first_pass(file_am);
            if (was_error == FALSE) 
	        { 
                rewind(file_am);
                second_pass(file_am, output_filename);
            }

            printf("\nFinished %s assembling process\n", input_filename);
        }
        else 
	    write_error(CANNOT_OPEN_FILE);
        free(input_filename);
    }

	return 0;
}
