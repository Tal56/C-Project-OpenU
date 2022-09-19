#include <stdio.h>
#include <string.h>
#include "pre_processor.h"
struct Macro *head = NULL;

/*Checks whether it is the beginning of a macro or the end of a macro*/
int IsMacroOrEndm(char *line) 
{
    int index = 0 , mindex = 0 ;
    char macro [MAX_LINE];
    memset(macro , '\0' , MAX_LINE);
    while(isspace(line[index])) /*skips the white charecters*/
    	index ++;
	while ((!isspace(line[index])) && (line[index] != '\n'))
    {
	    macro[mindex] = line[index];
	    mindex++;
	    index++;
  	}
    if (strcmp(macro, "macro") == 0)
	{
    	return 2;
	}
    if (strcmp(macro, "endmacro") == 0)
	{
    	return 0;
	}
    return 1;
}

/*Enter the macro name in the macros table*/
void InsertName(struct Macro *temp, char *line) 
{
  int index = 0 , nindex = 0 ;
  char name [MAX_LINE];
  memset(name , '\0' , MAX_LINE);
  while(isspace(line[index]))/*skips the white charecters*/
    index++;
  while (!isspace(line[index]) && line[index] != '\n')/*reads the first word in the line and skips over it*/
  	index++;
  while(isspace(line[index]))
    index++;
  while (!isspace(line[index]) && line[index] != '\n')/*reads and saves the macro name */
  {
    name[nindex] = line[index];
    nindex++;
    index++;
  }
  strcpy(temp->mname,name);
}

/*Inserts the macro contents into the macros table*/
void InsertContent(struct Macro *temp, char *line, char *content) 
{
	/*char new_line [1] ={'\n'};
	strcat(line, new_line);*/
	strcpy(content, line);
	strcat(temp->mcontent, content);
}

/*adds a macro to macro table*/
void addToMtable(char *name, char *content) 
{
	if(head == NULL)
	{
		head = (struct Macro*)malloc(sizeof(struct Macro));
		memset(head->mname, '\0', MAX_LINE);
		memset(head->mcontent, '\0', MAX_CONTENT);
		strcpy(head->mname, name);
  		strcpy(head->mcontent, content);
    }
	else
	{
  		struct  Macro *temp = NULL;
  		temp = (struct Macro*)malloc(sizeof(struct Macro));
		memset(temp->mname, '\0' , MAX_LINE);
		memset(temp->mcontent , '\0' , MAX_CONTENT);
  		strcpy(temp->mname, name);
  		strcpy(temp->mcontent, content);
		temp ->next = head -> next;
  		head ->next = temp;
	}
}


/*creates the macro table*/
void macroTable(FILE *source_file)
{
	int macro_result;
	int flag=0;
	char line [MAX_LINE];
    char content [MAX_LINE];
    int i =1;
    struct  Macro* temp = NULL;
    temp = (struct Macro*)malloc(sizeof(struct Macro)); 
	memset(temp->mname, '\0' , MAX_LINE);
	memset(temp->mcontent , '\0' , MAX_CONTENT);
	memset(line, '\0', MAX_LINE);
    memset(content, '\0', MAX_LINE);
	while(fgets(line, MAX_LINE, source_file))/* Read lines until end of file */
	{
		macro_result = IsMacroOrEndm(line);
		if(macro_result == 0)/* if this line is end of macro */
		{
			flag = 0;
			addToMtable(temp->mname, temp->mcontent);
		}
		if(macro_result == 1)/*this line is neither definition macro and end macro*/
		{
			if(flag == 1)
				InsertContent(temp, line, content);
		}
		if(macro_result == 2)/* if this line is macro definition */
		{
				flag=1;
				InsertName(temp, line);	
		}
        memset(line, '\0', MAX_LINE);
        i++;
	}
}

/*checks if the current line is macro call*/
int IsMacroCall(char *line)
{
    int index = 0, first_word_index = 0;
    char first_word [MAX_LINE];
    struct Macro *temp = head;
    memset(first_word, '\0' , MAX_LINE);
    while(isspace(line[index]))
    {
    	index++;
    }
    while ((!isspace(line[index])) && (line[index] != '\n'))
    {
    	first_word[first_word_index] = line[index];
    	first_word_index++;
    	index++;
    }
	while(temp != NULL)
	{
		if(strcmp(temp->mname, first_word) == 0 )
        {
			return 1;
        }
		temp = temp->next;
	}
	return -1;
}

/* switch the macro call with its content*/
void switchMacro(char *line, FILE *file_am)
{
	int index = 0, mindex = 0;
    char mname [MAX_LINE];
	struct Macro *temp = head;
    memset(mname, '\0' , MAX_LINE);
    while(isspace(line[index]))/*skips the white charecters*/
    	index++;
    while ((!isspace(line[index])) && (line[index] != '\n'))/*reads the first (and only) word in the line and skips over it*/
    {
    	mname[mindex] = line[index];
    	mindex++;
    	index++;
	}
	while(temp != NULL)
	{
		if(strcmp(temp->mname, mname)==0)
	    {
			fprintf(file_am, "%s\n", temp->mcontent);
		}
		temp = temp->next;
	}
}
/*this function reads the lines until the end of file and for each 
  line checks its content and acts accordingly*/
void macro(FILE *source_file, FILE *file_am, char *line)
{
	int flag = 0;/*this flag will turn on if it's macro definition
				 and turn off if it's end of macro*/
	int MacroCall;
    int MacroOrEndm;  
	while(fgets(line, MAX_LINE, source_file)!=NULL)
	{
		MacroCall = IsMacroCall(line);
		MacroOrEndm= IsMacroOrEndm(line);
		if(MacroOrEndm == 2)/* if this line is macro definition */
		{
			flag = 1;
		}else
		  if(MacroOrEndm == 0)/* if this line is end of macro */
		  {
			 flag = 0;
		  }else
		   if(MacroOrEndm == 1)/*this line is neither definition macro and end macro*/
		  {
			 if(flag == 0 && MacroCall != 1)
			{
				fprintf(file_am, "%s", line);
			}	
		  }
		   if(MacroCall == 1)
		  {
			switchMacro(line, file_am);
		  }
	}
	
}

/* This function frees the allocated memory for the macros table*/
void free_macro()
{
	
	struct Macro *temp;
	while(head)
	{
		temp=head;
		head=head->next;
		free(temp);
	}
}


void PreWriteFile(FILE *source_file, FILE *file_am, char *input_filename)
{ 
    char line [MAX_LINE];
    memset(line, '\0', MAX_LINE);
    macroTable(source_file);
    fclose(source_file);
    source_file= fopen(input_filename, "r");
    if(source_file!=NULL)
    {
     macro(source_file, file_am, line);
	 free_macro();
    }
}
