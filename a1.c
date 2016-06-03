/* Andrew Creevey -- 12236284 -- Assignment 1 -- 159.331 */
/* This program reads in text based strings and deals with them accordingly */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Function declarations */
void PrintName ();
char reverse (char input[]);
int CheckStatement(char *statement);
void Strip(char *strip, int len);
void StripQuotes(char *strip);
void Identify (char* input);
void Expression (char* input);
int Punctuation (char p);
int Letter (char l);
int Number (char n);

/* Stucture declarations */
struct Content {
	int len;
	char *input;
};
struct Lines {
	Content name;
	Content text;
};
struct ExpStruct {
	char *line;
};

int main () {
	PrintName(); /* call function to display name at top of cmd */

	char c;
	int len = 0, lineCount = 0, maxLines = 10, numVar = 20, quote = 0, stringCapacity = 80, found = 0;
	/* I am using a the structure "Lines" to store my input */
	/* It takes on the form similar to a 3d array: set[num_of_lines][2][len_of_input] */
	Lines *set = NULL;
	set = (Lines*) malloc (maxLines * sizeof(Lines));
	ExpStruct *NumLines;
	NumLines = (ExpStruct*) malloc (numVar * sizeof(ExpStruct));
	
	int z;
	while(1) { 															/* Whole program loops so don't have to put in whole string at once */
		
		/* Ensure all variables are NULL */
		for (z = 0; z < numVar; z++) {
			NumLines[z].line = NULL;
		}
		
		/*/////////////////////
		// Start the program //
		/////////////////////*/
		stringCapacity = 80;
		/* Check that we still have room for the line thats about to come in */
		if (lineCount >= maxLines-1) {
			maxLines *= 2;
			set = (Lines*) realloc (set, maxLines * sizeof (Lines));
		}
		
		char *string = NULL, *inputBuffer = NULL;				/* Make my strings, set them to null */
		inputBuffer = (char*) malloc ((stringCapacity+1) * sizeof(char));	/* Allocate temp string */
		int i = 0;
		printf("Please enter a string: \n");					/* enter a string into the program */
		while (1) { 
			/*///////////////////////////////////////////
			// Read in string, one character at a time //
			///////////////////////////////////////////*/
			if (stringCapacity <= i) { 							/* Check if there is enough space in the inputBuffer */
				stringCapacity += 80;								/* Resize the capacity incase of another overflow */
				inputBuffer = (char*) realloc (inputBuffer, (stringCapacity + 1) * sizeof(char)); /* Reallocate  */
			}
			c = getchar();												/* Grab the next character */
			/*/////////////////////////////////
			// Check that the input is valid //
			/////////////////////////////////*/
			if (Letter(c) == 0 && Punctuation(c) == 0 && Number(c) == 0) {
				printf("c = %c\n", c);							/* Random input, prints to show error */
				printf("[82] Error: Unrecognised character, please ensure correct input. Exiting...\n");
				exit(0);
			}
			if (c == '\"') {											/* If next character is " then put in the buffer   */
				inputBuffer[i] = c;									/* increment the quote count (if 1 and a ; appears */
				quote++; 												/* the ; is in the quote marks)							*/						
			}
			else if (c == ';') {
				if (quote%2 == 1) {										/* If quote == 1, that means the ';' is in a string of text */
					inputBuffer[i] = c;								/* e.g. set example "example; text"; 							 */
				} else {
					inputBuffer[i] = c;								/* If we end up in this part, the ; is at the end of the string */
					i++;													/* So put it in the string, increase i, 								 */
					inputBuffer[i] = '\0';							/* And then finish the string with a \0 								 */
					break;												/* And break 																	 */
				}
			} else if (c == '\n') {
				if (quote%2 == 1) {
					inputBuffer[i] = c;								/* e.g. set example "example; text"; 							 */
				} else {
					inputBuffer[i] = c;								/* If we end up in this part, the ; is at the end of the string */
					//i++;													/* So put it in the string, increase i, 								 */
					//inputBuffer[i] = '\0';							/* And then finish the string with a \0 								 */
					//break;												/* And break 																	 */
				}
			} else {
				inputBuffer[i] = c;									/* If all okay, add to buffer */
			}
			i++;															/* Increase i and loop */
		}
		c = getchar();													/* This removes the '\n' from the buffer */
		
		string = (char*) malloc ((strlen(inputBuffer)+1)*sizeof(char));	/* Create the string array with size of len */
		strcpy (string, inputBuffer);								/* Copy the buffer into the string array */
		inputBuffer[0] = '\0';										/* Set first character to empty */
		free (inputBuffer);											/* Free the buffer */
		printf("\n");													/* For formatting */	
		
		len = strlen(string);
		i = 0;
		char temp[len+1], statementStr[16];
		strcpy(temp,string);
		/*///////////////////
		// Deal with input //
		///////////////////*/
		while (1) {
			/*//////////////////////////////////////////////////////
			// Seperate the statement from identifier/expressions //
			//////////////////////////////////////////////////////*/
			int j = 0;
			while ((temp[j] != ' ' && temp[j] != ';')) {		/* continue until you hit the first space or semicolon (e.g. exit;) */
				if (j > 15) { printf("Statement is too long!\n"); break; } /* Limit of 15 because the longest statement is 15 char's long (printwordlength)*/
				statementStr[j] = temp[j];							/* Copy into new string */
				j++;														/* Increment i */
			} statementStr[j] = '\0'; 								/* Close the string off */
			
			/*//////////////////////////////////////////////////////////////
			// Discovers which statement and calls corresponding function //
			//////////////////////////////////////////////////////////////*/
			int length = 0, s = 0;
			switch (CheckStatement(statementStr)) {
				case 0:
					/*///////////
					//   Set   //
					///////////*/
					Strip(temp, 4);									/* call strip function, remove "set " from string */
				
					if (islower(temp[0]) && Letter(temp[0]) != 1) { 					/* Checking identifier doesnt start with a number */
						printf("[150] Error: Must start variable name a lowercase letter! Exiting...\n"); /* If so, exit */
						exit(0);
					}
					length = strlen(temp);
					
					char *identifier, *expression;
					identifier = NULL, expression = NULL;
					identifier = (char*) malloc ((length+1)*sizeof(char)); /* Allocate identifier */
					
					strcpy (identifier, temp);						/* copy the temp file into indentifier */
					Identify(identifier);							/* cut down to only the identifier */
					
					len = strlen (temp);
					expression = (char*) malloc ((length+1)*sizeof(char));
					
					strcpy (expression, temp);						/* copy the temp file into expression ++++++++++++++++++++++++++++ */
					Expression(expression);							/* cut down to on the expression */
					
					/* Count number of spaces inside whole expression (between variables): e.g. "The cat" + "hello" would be 2 spaces */
					/* Also change every space between variables to a semicolon */
					int exprLen, quotes, spaces, varCount;
					spaces = 0, quotes = 0, varCount = 0;
					exprLen = strlen (expression);						/*  */
					for (i = 0; i < exprLen; i++) {						/*  */
						if (expression[i] == '\"') quotes++;			/* If we come accross quote marks, increase quote count */
						if (expression[i] == ' ' && quotes%2 != 1) { /* If current = a space and the quotes%2 checks we not in a literal */
							expression[i] = ';';								/* Change the space to a semicolon */
							spaces++; varCount++;
						}
					}
					varCount++;
					
					/* Check memory allocation is good */
					if (varCount >= numVar-1) {
						numVar *= 2;											/* Double current memory allocation */
						NumLines = (ExpStruct*) realloc (NumLines, (numVar+1) * sizeof (Lines)); 
					}
					
					/* Get the absolute max length of all variables */
					/* This helps with memory allocation of the super long identifiers/expressions */
					/* (Looking at you testfile #2)*/
					
					int checked;
					checked = 0;
					if (expression[0] == '\"' && varCount == 1) checked = 2;
					if (Letter(expression[0]) == 1 && varCount == 1) checked = 3;
					
					int maxLen;
					maxLen = 0;
					if (lineCount == 0) {						/* If we are on the first iteration, there will be nothing in the set[][] */
						maxLen = exprLen;							/* So then maxLen = expression */
					} else {
						for (i = 0; i <= lineCount; i++) {	/* If we have been through before, maxlen += all set[text][len] */
							maxLen += set[i].text.len + 1;
						}
					}	
					int x, tracker;
					if (varCount > 1) { 													/* This was giving me errors as if varcount == 1 i was allocating it un needed memory */
						/* Allocate memory space to all the variables */
						for (i = 0; i < varCount; i++) {
							NumLines[i].line = (char*) malloc ((maxLen+1) * sizeof(char)); /* Make size = to maximum possible len (explained line 185-187) */
						}
						
						/*////////////////////////////////////////
						// Copy all the variables into NumLines //
						////////////////////////////////////////*/
						
						i = 0, tracker = 0, x = 0, quotes = 0;
						while (i < varCount) {											/* While smaller than variable count */
							tracker = 0;
							while (expression[x] != ';' && x < exprLen) {		/* While expression[x] is not a semicolon and not at end of string*/
								if (expression[x] == '\"') quotes++;				/* If " come up, track it. This is so we allow semicolons inside quotes's 	e.g. "hello;" */
								NumLines[i].line[tracker] = expression[x];		/* Copy character into NumLines[i].line */
								x++; tracker++;											/* Increment counters */
							}
							if (quotes%2 == 1) {											/* If quotes%2 = 1, that means that a semicolon appeared in the middle of quote marks */
								while (expression[x] != '\"') {						/* Therefore, while not equal to ", keep going */
									NumLines[i].line[tracker] = expression[x];	/* Copy character into NumLines[i].line */
									x++; tracker++;										/* Increment counters */
								}
								NumLines[i].line[tracker] = expression[x];		/* Copy character into NumLines[i].line */
								x++; tracker++; quotes++;								/* We have reached the second quote mark, so increase quotes again (now quotes%2 = 0) */
							}
							NumLines[i].line[tracker] = '\0';						/* Close line at current position */
							if (expression[x] == ';') x++;							/* If we end up on a semicolon, go to next variable */
							i++;																/* Increase variable number */
						}
					}
					/*//////////////////////////////
					// Deal with NumLines content //
					//////////////////////////////*/
					if (varCount > 2) {
						checked = 1;
						for (i = 0; i <= (varCount-2); i++) {					/* While i is always 2 less than varCount (because*/
							char tempstr1[maxLen], tempstr2[maxLen], tempstr3[maxLen];
							strcpy (tempstr1, NumLines[i].line);
							strcpy (tempstr2, NumLines[i+1].line);
							strcpy (tempstr3, NumLines[i+2].line);
							
							if (strcmp(tempstr1, "SPACE") == 0) strcpy (tempstr1, " ");		
							if (strcmp(tempstr2, "SPACE") == 0) strcpy (tempstr2, " ");	/* If any are equal to SPACE, change it to a " " */
							if (strcmp(tempstr3, "SPACE") == 0) strcpy (tempstr3, " ");
							
							if (strcmp(tempstr1, "TAB") == 0) strcpy (tempstr1, "\t"); 
							if (strcmp(tempstr2, "TAB") == 0) strcpy (tempstr2, "\t"); /* If any are equal to TAB, change it to a "\t" */
							if (strcmp(tempstr3, "TAB") == 0) strcpy (tempstr3, "\t"); 
							
							if (strcmp(tempstr1, "NEWLINE") == 0) strcpy (tempstr1, "\n"); 
							if (strcmp(tempstr2, "NEWLINE") == 0) strcpy (tempstr2, "\n"); /* If any are equal to NEWLINE, change it to a "\n" */
							if (strcmp(tempstr3, "NEWLINE") == 0) strcpy (tempstr3, "\n"); 
							
							if (tempstr1[0] == '\"') StripQuotes(tempstr1);
							if (tempstr2[0] == '\"') StripQuotes(tempstr2); /* If any have the variables have quotes, remove them */
							if (tempstr3[0] == '\"') StripQuotes(tempstr3);
							
							if (lineCount > 0) {
								for (x = 0; x < lineCount; x++) {  /* Find any matching input names, that are already there */
									if (strcmp(tempstr1, set[x].name.input) == 0) { strcpy(tempstr1, set[x].text.input); break; } /* If found, copy into temp1 */
									if (strcmp(tempstr3, set[x].name.input) == 0) { strcpy(tempstr3, set[x].text.input); break; }
								}
							}
							if (strcmp(tempstr2, "+") == 0) { /* If we are adding strings */
								strcat (tempstr1, tempstr3);
								strcat (tempstr1, "\0");
								strcpy (NumLines[i+2].line, tempstr1);
								/*printf("NumLines[%d].line = %s\n", i, NumLines[i+2].line); error checking*/
							} else {
								printf("[277] Error: Need a '+' here to be able to add anything together! Exiting...\n");
								exit(0);
							}
							i++;
							if (i >= varCount-2) {  /*printf("Breaking...\n");*/ break;}
						}
						i++;
					}
					
					/* If we have gone in the above if statement, copy NumLines into checked */
					if (checked == 1) {
						expression = (char*) realloc (expression, (strlen(NumLines[i].line) + 1) * sizeof(char));
						strcpy (expression, NumLines[i].line);
					}
					
					for (x = 0; x < varCount; x++) {
						s = lineCount;															/* easier to type s than lineCount */
						found = 0;
						if (checked == 1 || checked == 2) {
							/* Check if there is already a variable with same name as identifier */
							if (expression[0] == '\"') StripQuotes(expression);
							if (lineCount > 0) {
								for (i = 0; i < s; i++) {
									if (strcmp (set[i].name.input, identifier) == 0) { /* If the identifier name already exists */
										length = strlen (expression);
										if (set[i].text.len != length) {					/* If new string is bigger than the previous one */
											set[i].text.len = strlen(expression)+1; 	/* Overwrite the previous length */
											set[i].text.input = (char*) realloc (set[i].text.input, set[i].text.len * sizeof (char));
										}
										strcpy (set[i].text.input, expression); 		/* Overwrite the previous text */
										found++;													/* Increase found */
										printf("Overwriting previous variable...\n");
										printf("set[%d][name] : %s\n", i, set[i].name.input);
										printf("set[%d][input]: %s\n\n", i, set[i].text.input);
										lineCount--;											/* We are overwriting previous variable, so linecount doesnt increase */
										break;
									}
								}
							}
							/* If no match ups, write to new variable at position 's' */
							if (found == 0) {													/* This means the identifier was not previously declared */
								set[s].name.len = strlen(identifier)+1;				/* Set the length of the name */
								set[s].name.input = (char*) malloc ((set[s].name.len+1) * sizeof(char)); /* Allocate the size to be same name.len */
								strcpy(set[s].name.input, identifier); 				/* Copy the identifier into name.input */
								set[s].text.len = strlen(expression)+1; 				/* Set the length of the next */
								set[s].text.input = (char*) malloc ((set[s].text.len+1) * sizeof(char)); /* Allocate the size to be same text.len */
								strcpy(set[s].text.input, expression); 				/* Copy the identifier into text.input */
								printf("set[%d][name] : %s\n", s, set[s].name.input);
								printf("set[%d][input]: %s\n\n", s, set[s].text.input);
								break;
							}
							printf("print me\n");
							break;
						} else if (Letter(expression[0]) == 1 && lineCount == 0) {
							/* If you try to do "set one two;" as the first input, it will fail */
							printf("[329] Error: You need to create that variable before you can assign it. Exiting...\n");
							exit(0);
						} else if (Letter(expression[0]) == 1 && lineCount > 0) {
							/*////////////////////////////////////////////////////////
							// If you try to do "set one two;" it will come in here //
							////////////////////////////////////////////////////////*/
							found = 0;
							for (i = 0; i < s; i++) {
								/* IMPORTANT: */
								/* This block deals with the following: If you typed "set two one;" we must check if 'two' exists and that 'one' exists */
								if (strcmp (set[i].name.input, identifier) == 0) { 		/* If the identifier name already exists */
									for (j = 0; j < s; j++) {
										if (strcmp (set[j].name.input, expression) == 0) { /* Check if the expression name already exists */
											length = strlen (expression);
											if (set[i].text.len != length) {						/* If new string is bigger than the previous one */
												set[i].text.len = strlen(set[i].text.input)+1; 		/* Overwrite the previous length */
												set[i].text.input = (char*) realloc (set[i].text.input, set[i].text.len * sizeof (char));
											}
											strcpy (set[i].text.input, set[j].text.input); 	/* Overwrite the previous text */
											found++;														/* Increase found */
											printf("Overwriting previous variable...\n");
											printf("set[%d][name] : %s\n", i, set[i].name.input); /* Print outcome */
											printf("set[%d][input]: %s\n\n", i, set[i].text.input);
											lineCount--;												/* We are overwriting previous variable, so linecount doesnt increase */
											break;
										}
									}
								}
							}
							if (found == 0) {
							/* If the identifier name is new AKA didnt find any matches, come in here */
								for (i = 0; i < s; i ++) {
									if (strcmp (set[i].name.input, expression) == 0) {		/* Check if the expression name already exists */
										found++;
										set[s].name.len = strlen(identifier)+1;				/* Set the length of the name */
										set[s].name.input = (char*) malloc (set[s].name.len * sizeof(char)); /* Allocate the size to be same name.len */
										strcpy(set[s].name.input, identifier); 				/* Copy the identifier into name.input */
										set[s].text.len = strlen(set[i].text.input)+1; 				/* Set the length of the next */
										set[s].text.input = (char*) malloc (set[s].text.len * sizeof(char)); /* Allocate the size to be same text.len */
										strcpy(set[s].text.input, set[i].text.input); 		/* Copy the identifier into text.input */
										printf("set[%d][name] : %s\n", s, set[s].name.input);	/* Print outcome */
										printf("set[%d][input]: %s\n\n", s, set[s].text.input);
									}
								}
							}
							if (found == 0) {
								/* If still no matches, give an error */
								printf("[376] Error: That variable name has not been assigned yet. Exiting...\n");
								exit(0);
							}
						}
						else { 
							printf("[381] Error: Something went wrong. Exiting...\n");
							exit(0);
						}
					}
					lineCount++;												/* Add to linecount */
					if (varCount > 0) {
						for (i = 0; i < varCount; i++) {
							free(NumLines[i].line);
						}
					}
					free (identifier);
					free (expression);										/* Free variables */
					free (string);
					fflush(stdout);
					break;
				case 1: 
					/*//////////////
					//   Append   //
					//////////////*/
					Strip (temp, 7);										/* Remove the statement */
					length = strlen(temp);								/* Get the length of the temp */ 
					
					char *append;											/* Make a new temp string called append */
					append = (char*) malloc (length*sizeof(char)); /* Allocate memory to printme */
					while (temp[i] != ' ') { 							/* Remove the ';' from the end of the string */
						append[i] = temp[i]; 							/* While not ';' copy into new string */
						i++;
					} append[i] = '\0';									/* Close string */
					
					length = strlen(append);							/* Get the length of the temp */ 
					Strip (temp, length+1);
					if (temp[0] == '\"') StripQuotes(temp);
					found = 0;												/* Reset found to 0 (found used for error checking) */
					for (i = 0; i < lineCount; i++) {				/*  */
						if (strcmp (set[i].name.input, append) == 0) {
							found++;											/* Made a match, increase found */
							set[i].text.len += strlen (temp);		/* Update the size */
							set[i].text.input = (char*) realloc (set[i].text.input, set[i].text.len * sizeof(char)); /* Rellocate the size to be same text.len */
							strcat(set[i].text.input, temp);			
							printf("set[%d][name] : %s\n", i, set[i].name.input);
							printf("set[%d][input]: %s\n\n", i, set[i].text.input); /* Display text on screen */ 
							break;											/* Then break */ 
						}
					}
					if (found == 0) { printf("[425] Error: No matching identifiers found. Exiting...\n"); exit(0); } /* Used for error checking */
					free (append);	
					free (string);											/* Free variables */
					break;
				case 2: 
					/*////////////
					//   List   //
					////////////*/
					Strip (temp, 5);										/* Remove the statement */
					found = 0;												/* Error checking */
					printf("Identifier list (%d):\n", lineCount);/* Print header */
					for (i = 0; i < lineCount; i++) {				/* While smaller than num of lines */
						found++;
						printf("%s: ", set[i].name.input);			/* Print the name and content of the string */
						printf("%s\n", set[i].text.input);			
					}
					printf("\n");											/* For formatting */ 
					if (found == 0) { printf("[442] Error: No matching identifiers found. Exiting...\n"); exit(0); } /* Used for error checking */
					
					free (string);											/* Free variables */
					break;
				case 3: 
					/*///////////////
					//   Reverse   //
					///////////////*/
					Strip (temp, 8);
					i = 0;
					length = strlen (temp);
					char *backwards;											/* Make a new temp string called printme */
					backwards = (char*) malloc (length*sizeof(char)); /* Allocate memory to printme */
					while (temp[i+1] != '\0') { 							/* Remove the ';' from the end of the string */
						backwards[i] = temp[i]; 							/* While not ';' copy into new string */
						i++;
					} backwards[i] = '\0';
					
					if (backwards[0] == '\"') {							/* If you try to do print "string" */
						StripQuotes(backwards);
						reverse (backwards);
						printf("Reverse is: %s\n\n", backwards);
						free (backwards);										/* Free variables */
						free (string);	
						break;
					} else {
						if (Letter(backwards[0]) == 0) {						/* Error handling */
							printf("[469] Error: That is not a valid variable name. Please only use letters for the first letter. Exiting...\n");
							exit(0);
						}
						found = 0;												/* Reset found to 0 (found used for error checking) */
						for (i = 0; i < lineCount; i++) {				/*  */
							if (strcmp (set[i].name.input, backwards) == 0) {
								found++;
								reverse (set[i].text.input);
								printf("Reverse is: %s\n\n", set[i].text.input);
								break;											/* Then break */ 
							}
						}
					}
					if (found == 0) { printf("[482] Error: No matching identifiers found. Exiting...\n"); exit(0); } /* Used for error checking */
					free (backwards);
					free (string);
					break;
				case 4: 
					/*/////////////
					//   Print   //
					/////////////*/
					Strip (temp, 6);										/* Remove the statement */
					length = strlen(temp);								/* Get the length of the temp */
					i = 0;
					char *printme;											/* Make a new temp string called printme */
					printme = (char*) malloc (length*sizeof(char)); /* Allocate memory to printme */
					while (temp[i+1] != '\0') { 							/* Remove the ';' from the end of the string */
						printme[i] = temp[i]; 							/* While not ';' copy into new string */
						i++;
					} printme[i] = '\0';									/* Close string */
					
					i = 0;
					if (printme[i] == '\"') {							/* If you try to do print "string" */
						i++;													
						while (printme[i] != '\"') {					/* While not ", keep printing characters */
							printf("%c", printme[i]);					
							i++;
						} printf("\n\n");
						free (printme);									/* Free variables */
						free (string);	
						break;
					} else {
						found = 0;												/* Reset found to 0 (found used for error checking) */
						for (i = 0; i < lineCount; i++) {				/*  */
							if (strcmp (set[i].name.input, printme) == 0) {
								found++;											/* Made a match, increase found */
								printf("%s\n\n", set[i].text.input);	/* Display text on screen */ 
								break;											/* Then break */ 
							}
						} 
					}
					if (found == 0) { printf("[520] Error: No matching identifiers found. Exiting...\n"); exit(0); } /* Used for error checking */
					free (printme);	
					free (string);											/* Free variables */
					break;
				case 5: 
					/*////////////////////
					//   Print Length   //
					////////////////////*/
					Strip (temp, 12);										/* Remove the statement */
					length = strlen(temp);								/* Get the length of the temp */ 
					
					char *pLen;												/* Make a new temp string called pLen */
					pLen = (char*) malloc (length*sizeof(char));	/* Allocate memory to pLen */
					while (temp[i] != ';') {  							/* Remove the ';' from the end of the string */
						pLen[i] = temp[i];								/* While not ';' copy into new string */
						i++;
					} pLen[i] = '\0';										/* Close string */
					
					found = 0;												/* Reset found to 0 (found used for error checking) */
					for (i = 0; i < lineCount; i++) {
						if (strcmp (set[i].name.input, pLen) == 0) {
							found++;											/* Made a match, increase found */
							printf("Length is: %d\n\n", set[i].text.len); /* Display length of expression on screen */ 
							break;											/* Break from string */
						}
					}
					if (found == 0) { printf("[546] Error: No matching identifiers found. Exiting...\n"); exit(0); } /* Used for error checking */
					free (string);
					free (pLen);											/* Free variables */
					break;
				case 6: 
					/*///////////////////
					//   Print Words   //
					///////////////////*/
					Strip (temp, 11);										/* Remove the statement */
					length = strlen(temp);								/* Get the length of the temp */ 
					
					char *pWords;											/* Make a new temp string called pWords */
					pWords = (char*) malloc ((length+1)*sizeof(char)); /* Allocate memory to pWords */
					while (temp[i+1] != '\0') { 							/* Remove the ';' from the end of the string */
						pWords[i] = temp[i];								/* While not ';' copy into new string */
						i++;
					} pWords[i] = '\0';									/* Close string */
					
					found = 0;												/* Reset found to 0 (found used for error checking) */
					for (i = 0; i < lineCount; i++) {
						if (strcmp (set[i].name.input, pWords) == 0) {
							found++;											/* Made a match, increase found */
							printf("Words are: \n");
							int k = 0; spaces = 0;
							for (k = 0; k < set[i].text.len; k++) {/* While k is smaller than length of expression */
								printf("%c", set[i].text.input[k]);
								if (set[i].text.input[k] == ' ') printf("\n"); /* If there is a ' ', increase space count */
							}
							printf("\n\n"); 									/* Display space count + 1 (+1 because no space at end of string) */
							break;											/* Break out of the outer (i) loop */
						}
					}
					if (found == 0) { printf("[578] Error: No matching identifiers found. Exiting...\n"); exit(0); } /* Used for error checking */
					free (pWords);
					free (string);											/* Free variables */
					break;
					
					break;
				case 7: 
					/*////////////////////////
					//   Print Word Count   //
					////////////////////////*/
					Strip (temp, 15);										/* Remove the statement */
					length = strlen(temp);								/* Get the length of the temp */ 
				
					char *pWordCount;										/* Make a new temp string called pWordCount */
					pWordCount = (char*) malloc ((length+1)*sizeof(char)); /* Allocate memory to pWords */
					while (temp[i+1] != '\0') { 							/* Remove the ';' from the end of the string */
						pWordCount[i] = temp[i];						/* While not ';' copy into new string */
						i++;
					} pWordCount[i] = '\0';								/* Close string */
					
					found = 0;												/* Reset found to 0 (found used for error checking) */
					for (i = 0; i < lineCount; i++) {
						if (strcmp (set[i].name.input, pWordCount) == 0) {
							found++;											/* Made a match, increase found */
							int k = 0; spaces = 0;
							for (k = 0; k < set[i].text.len; k++) {/* While k is smaller than length of expression */
								if (set[i].text.input[k] == ' ') spaces++; /* If there is a ' ', increase space count */
							}
							printf("Wordcount is: %d\n\n", spaces+1); /* Display space count + 1 (+1 because no space at end of string) */
							break;											/* Break out of the outer (i) loop */
						}
					}
					if (found == 0) { printf("[610] Error: No matching identifiers found. Exiting...\n"); exit(0); } /* Used for error checking */
					free (pWordCount);
					free (string);											/* Free variables */
					break;
				case 8: 
					/*////////////
					//   Exit   //
					////////////*/
					printf("Exit\n");										/* Print exit on screen */
					free (NumLines);
					free (string);											/* Free variables */
					free (set);
					exit(0);													/* Exit from the program */
				case 9: 
					/*/////////////
					//   Error   //
					/////////////*/
					printf("[627] Error: Invalid input. Statement not recognised. Exiting...\n");
					free (NumLines);
					free (string);											/* Free variables */
					free (set);
					exit(0);
				default: 
					printf("[633] Error: Default case entered, something went wrong. Exiting...");								/* Free variables */
					free (set);	
					exit(0);
			}
			break;
		}
	}
}

char reverse (char input[]) {
	/* Declare variables */
	int i = 0, j = 0, k = 0, len = 0, spaces = 0, tracker = 0;
	int templen = 0;
	j = len = strlen (input);
	char words[len], sdrow[len], sentence[len];
	
	while (true) {								
		/* While not at a space or end of line, copy up until then into temp1 */
		while (input[i] != ' ' && input[i] != '\0'){
			words[spaces] = input[i];
			spaces++; 
			i++;
		} words[spaces] = '\0';
		templen = strlen(words);
		
		/* Reverse word */
		for (j = spaces, k = 0; k < templen; j--, k++) {
			sdrow[k] = words[j-1];
		} sdrow[k] = '\0';
		
		/* Copy the word into the big string */
		k = 0;
		while (sdrow[k] != '\0') { 			/* While not at the end of temp */
			sentence[tracker] = sdrow[k];
			tracker++; k++;
		}
		if (input[i] != '\0') {					/* If we are not at the end of the string */
			sentence[tracker] = ' '; 			/* Include a space between words */
			tracker++;
			sentence[tracker] = '\0';			/* And finish with a \0 (This will be overwritten next loop) */
		} else sentence[tracker] = '\0';		/* If at the end of the sentence, end with a \0 */
		spaces = 0; i++;
		if (i > len) break;
	}
	strcpy(input, sentence);					/* Copy the big string into input */
	return *input;									/* Return input */
}

void Identify (char* input) {
	/*///////////////////////////////////////////////
	// Return the name of the identifier           //
	///////////////////////////////////////////////*/
	int count = 0, i = 0;
	while (input[count] != ' ') {
		count++;								/* get the length of the variable name*/
	}						
	char name[count+1];					/* make new array that is [variable name length] long */
	while (input[i] != ' ') {			/* write to name */
		if (Punctuation(input[i]) == 1) {
			printf("[692] Error: Cannot have punctuation in variable names. Exiting... \n");
			exit(0);
		}
		name[i] = input[i];				
		i++;
	} name[i] = '\0';						/* close the string */
	strcpy (input, name);				/* copy the string back into input as cant return local var */
}

void Expression (char* input) {
	/*/////////////////////////
	// Return the expression //
	/////////////////////////*/
	
	int i = 0;
	while (input[i] != ' ') i++;			/* Count how long until there is a space, (going to remove up to here) */
	Strip (input, i+1);						/* Remove the identifier (strip), then return the expression */
	
	char expr[strlen(input)+1];			/* Create a new char string that is the same length as input */
	strcpy(expr, input);						/* Copy input into expr */ 
	
	/* We go into here when the first character is not a quote mark */
	i = 0;
	while (expr[i+1] != '\0') { 			/* Read until the string ends, also cuts off the last ';'*/
		input[i] = expr[i];					/* write into temp[0] when input[1] (because we skipped ") */
		i++;
	} 
	if (input[i] != ';') {printf("[719] Error: You either didnt enter an expression, or didn't end input with a semicolon. Exiting...\n"); exit(0); }
	input[i] = '\0';							/* close the string */
}

void Strip(char *strip, int len) {
	/*////////////////////////////////////////////////////
	//  Remove the statement and space from the string  //
	// e.g. (set one "The cat") becomes (one "The cat") //
	////////////////////////////////////////////////////*/
	int i = 0, j = 0; 
	j = strlen(strip);
	char temp[j+1];					/* create new string */
	strcpy (temp, strip);			/* copy contents into temp */
	for (i = 0; i < j; i++) {
		strip[i] = temp[i+len];		/* here we remove the statement name */
	} 										/* and space from the string */
}

void StripQuotes(char *strip) {
	/*///////////////////////////////////////
	//  Remove the quotes from the string  //
	//  e.g. ("The cat") becomes (The cat) //
	///////////////////////////////////////*/
	int i = 1, j = 0; 									/* i=1 so we miss first " mark */
	j = strlen(strip);
	char temp[j+1];										/* create new string */
	strcpy (temp, strip);								/* copy contents into temp */
	while (temp[i] != '\"') { 							/* Could be quote marks inside the expression (Daniel pls no) */
		strip[i-1] = temp[i];							/* Copy expression into strip */
		i++;
	} strip[i-1] = '\0';									/* and space from the string */
}

int Punctuation (char p) {
	/*////////////////////////////////////////////
	// Returns true if character is punctuation //
	////////////////////////////////////////////*/
	if(p == ',' || p == '.' || p == '/' || p == '?' ||
		p == '\''|| p == '"' || p == ';' || p == ':' ||
		p == '[' || p == ']' ||	p == '\\'|| p == ')' ||
		p == '(' || p == '&' ||	p == '*' || p == '^' ||
		p == '%' || p == '$' ||	p == '#' || p == '@' ||
		p == '!' || p == '+' || p == '-' || p == '*' ||
		p == '=' || p == '|' ||	p == '<' || p == '>' ||
		p == ' ' || p == '\"'|| p == '\n'|| p == ' ') {
		return 1;
	} else return 0;
}

int Letter (char l) {
	/*///////////////////////////////////////
	//     Converts to lowercase first     //
	// Returns true if character is letter //
	///////////////////////////////////////*/
	//l = (tolower(l));						/* Convert to lowercase */
	if ((l >= 'a' && l <= 'z') || (l >= 'A' && l <= 'Z')){
		return 1; 							/* if character between a & z, return true */
	} else return 0;						/* else return false */
}

int Number (char n) {
	/*///////////////////////////////////////
	// Returns true if character is number //
	///////////////////////////////////////*/
	if(n >= '0' && n <= '9') {
		return 1;
	}
	else return 0;
}

int CheckStatement(char *statement) {
	/*///////////////////////////////////////
	// Finds the statement, returns number //
	///////////////////////////////////////*/
	for (int i = 0; statement[i]; i++) {
		/* Change all to lowercase */
		statement[i] = tolower(statement[i]);
	}
	
	if (strcmp (statement, "set") == 0) {
		/* Set */
		return 0;
	}
	else if(strcmp (statement, "append") == 0) {
		/* Append */
		return 1;
	}
	else if(strcmp (statement, "list") == 0) {
		/* List */
		return 2;
	}
	else if(strcmp (statement, "reverse") == 0) {
		/* Reverse */
		return 3;
	}
	else if(strcmp (statement, "print") == 0) {
		/* Print */
		return 4;
	}
	else if(strcmp (statement, "printlength") == 0) {
		/* Print length */
		return 5;
	}
	else if(strcmp (statement, "printwords") == 0) {
		/* Print words */
		return 6;
	}
	else if(strcmp (statement, "printwordcount") == 0) {
		/* Print word count */
		return 7;
	}
	else if(strcmp (statement, "exit") == 0) {
		/* Exit */
		return 8;
	}
	else {
		/* Statement not recognised */
		return 9;
	}
}

void PrintName () {
	/*/////////////////////////////////////////////////////////////////
	// Used to print to the top of the console when starting program //
	/////////////////////////////////////////////////////////////////*/
	printf("----------------------------------------\n");
	printf("  159.331 Assignment 1 Semester 1 2016  \n");
	printf(" Submitted by: Andrew Creevey, 12236284 \n");
	printf("----------------------------------------\n");
	printf("\n");
}