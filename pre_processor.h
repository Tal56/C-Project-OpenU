#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "decode.h"
#define MAX_LINE 81
#define MAX_CONTENT 800
#define MAX_NAME 30


struct Macro {
    char mname[MAX_LINE];
    char mcontent[MAX_CONTENT];
    struct Macro* next;
};

struct LableList {
	char lname[MAX_NAME];
	int contributes[2];
	struct Lable* next;
};


void PreWriteFile(FILE *source_file, FILE *file_am, char *input_filename);
void checkEndOfFile(char *fname, FILE *source_file);
int IsMacroOrEndm(char *line);
void InsertName(struct Macro *temp, char *line);
void InsertContent(struct Macro *temp, char *line, char *content);
void addToMtable(char *name , char *content);
void macroTable(FILE *source_file);
int IsMacroCall(char *line);
void switchMacro(char *line, FILE *file_am );
void macro (FILE *source_file, FILE *file_am, char *line);
void free_macro();
