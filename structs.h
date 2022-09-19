/*
	This file contains definitions of types, structures and varibles that are being used in the program.
*/

#ifndef STRUCTS_H

#define STRUCTS_H

#include "assembler.h"

#define MACHINE_RAM 2000


typedef enum {FALSE, TRUE} boolean; /* Defining a boolean type */



/* Defining linked list of labels and a pointer to that list */
typedef struct structLabels * labelPtr;
typedef struct structLabels {
	char name[LABEL_LENGTH]; 
	unsigned int address; 
	boolean external; 
	boolean inActionStatement; 
	boolean entry; 
	labelPtr next; 
} Labels;

/* Defining a circular double-linked list to store each time the program uses an extern label, and a pointer to that list */
typedef struct ext * extPtr;
typedef struct ext {
    char name[LABEL_LENGTH]; 
    unsigned int address; /* the address in memory where the external address should be replaced */
    extPtr next; 
    extPtr prev; /* a pointer to the previous extern in the list */
} ext;


extern int ic, dc;
extern int err;
extern boolean was_error;
extern labelPtr symbols_table;
extern extPtr ext_list;
extern const char base32[32];
extern const char *commands[];
extern const char *directives[];
extern boolean entry_exists, extern_exists;
extern unsigned int data[]; /* Data array of words */
extern unsigned int instructions[]; /* Instructions array of words */


#endif
