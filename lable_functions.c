#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "decode.h"
#include "structs.h"

/* This function offsets the addresses of a certain group of labels (data/instruction labels)
   by a given delta (num).*/
void offset_addresses(labelPtr label, int num, boolean is_data)
{
    while(label)
    {
        /* We don't offset external labels (their address is 0). 
         is_data and inActionStatement must have different values in order to meet the same criteria
         and the XOR operator gives us that */
        if(!(label -> external) && (is_data ^ (label -> inActionStatement)))
        {
            label -> address += num;
        }
        label = label -> next;
    }
}

/* This function searches a label in the list and changes his entry field to TRUE and returns TRUE
else if the label doesn't exist return FALSE. */
int make_entry(labelPtr h, char *name)
{
    labelPtr label = get_label(h, name);
    if(label != NULL)
    {
        if(label -> external)
        {
            err = ENTRY_CANT_BE_EXTERN;
            return FALSE;
        }
        label -> entry = TRUE;
        entry_exists = TRUE; /* Global variable that holds that there was at least one entry in the program */
        return TRUE;
    }
    else
        err = ENTRY_LABEL_DOES_NOT_EXIST;
    return FALSE;
}

/* This function returns the address of a given label, if the label doesn't exist return FALSE (0).*/
unsigned int get_label_address(labelPtr h, char *name)
{
    labelPtr label = get_label(h, name);
    if(label != NULL) return label -> address;
    return FALSE;
}

/* This function check if a label is in the array and an external label is so return 1 else return 0 */
boolean is_external_label(labelPtr h, char *name)
{
    labelPtr label = get_label(h, name);
    if(label != NULL) return label -> external;
    return FALSE;
}

/* This function checks if a given name is a name of a label in the list */
boolean is_existing_label(labelPtr h, char *name)
{
    return get_label(h, name) != NULL;
}

/* This function checks if a given label name is in the list if so return 1 else return 0. */
labelPtr get_label(labelPtr h, char *name)
{
	while(h)
	{
        if(strcmp(h->name,name)==0) /* we found a label with the name given */
			return h;
		h=h->next;
	}
	return NULL;
}

/* This function adds a new label to the linked list of labels given its info. */
labelPtr add_label(labelPtr *hptr, char *name, unsigned int address, boolean external, ...)
{	
	va_list p;
	
	labelPtr t=*hptr;
	labelPtr temp; /* Auxiliary variable to store the info of the label and add to the list */

	if(is_existing_label(*hptr, name))
	{
		err = LABEL_ALREADY_EXISTS;
		return NULL;
	}
	temp=(labelPtr) malloc(sizeof(Labels)); 
	if(!temp) /*if we couldn't allocate memory to temp then print an error massage and exit the program*/
	{
		printf("\nerror, cannot allocate memory\n");
		exit(ERROR);
	}

	/* Storing the info of the label in temp */
	strcpy(temp->name, name);
    temp -> entry = FALSE;
	temp -> address = address;
	temp -> external = external;

	if(!external) /* An external label can't be in an action statement */
	{
		va_start(p,external);
		temp -> inActionStatement = va_arg(p,boolean);
	}
    else
    {
        extern_exists = TRUE;
    }

	/* If the list is empty then we set the head of the list to be temp */
	if(!(*hptr))
	{	
		*hptr = temp;
		temp -> next = NULL;
		return temp;	
	}

	/* Setting a pointer to go over the list until he points on the last label and then stting temp to be the new last label */
	while(t -> next != NULL)
		t = t->next;
	temp -> next = NULL;
	t -> next = temp;
	va_end(p);
	return temp;
}

/* This function frees the allocated memory for the symbols table*/
void free_labels(labelPtr *hptr)
{
	
	labelPtr temp;
	while(*hptr)
	{
		temp=*hptr;
		*hptr=(*hptr)->next;
		free(temp);
	}
}

/* This function gets a label's name, searches the list for it and deletes the label.
   If it managed to delete the label return 1 else return 0*/
int delete_label(labelPtr *hptr, char *name)
{
    labelPtr temp = *hptr;
    labelPtr prevtemp;
    while (temp) 
	{
        if (strcmp(temp->name, name) == 0) 
		{
            if (strcmp(temp->name, (*hptr)->name) == 0) 
			{
                *hptr = (*hptr)->next;
                free(temp);
            } 
			else 
			{
                prevtemp->next = temp->next;
                free(temp);
            }
            return 1;
        }
        prevtemp = temp;
        temp = temp->next;
    }
    return 0;

}

/* This function prints the list */
void print_labels(labelPtr h)
{
    while (h)
    {
        printf("\nname: %s, address: %d, external: %d", h->name, h->address, h->external);
        if (h->external == 0)
            printf(", is in action statement: %d -> ", h->inActionStatement);
        else
            printf(" -> ");
        h = h->next;
    }
    printf("*");
}

/* This function adds a node to the end of the list */
extPtr add_ext(extPtr *hptr, char *name, unsigned int reference)
{
    extPtr t=*hptr;
    extPtr temp;

    temp=(extPtr) malloc(sizeof(ext));
    if(!temp)
    {
        printf("\nerror, cannot allocate memory\n");
        exit(1);
    }

    temp -> address = reference;
    strcpy(temp->name, name);

    if(!(*hptr)) /* If the list is empty */
    {
        *hptr = temp;
        temp -> next = temp;
        temp -> prev = temp;
        return temp;
    }


    ((*hptr)->prev)->next = temp;
    temp->next = t;
    temp->prev = t->prev;
    (*hptr)->prev = temp;

    return temp;
}

/* This function frees the allocated memory for the list */
void free_ext(extPtr *hptr)
{
    /* Frees the extern list by going over each extern node and free it*/
    extPtr temp=*hptr;
    if(temp) {
        unsigned int last_reference = ((*hptr)->prev)->address;
        unsigned int reference = 0;
        do {
            temp = *hptr;
            reference = temp->address;
            *hptr = (*hptr)->next;
            free(temp);
        } while (reference != last_reference);
    }
}

/* This function prints the ext list */
void print_ext(extPtr h)
{
    extPtr orig=h;
    do
    {
        printf("\nname: %s, reference: %d - >", h->name, h->address);
        h=h->next;
    } while(h -> address != orig -> address);
    printf("*\n");
}
