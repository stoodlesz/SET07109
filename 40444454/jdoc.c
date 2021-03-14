#include <stdio.h>
#include <string.h>

// argc number of arguments
// argv pointer to argument array
// argument_array[1] is the first argument

int main(int number_of_arguments, char **argument_array) //Argc and **argv elaborated for my own clarity
{
	char input_file_name[100];
	char output_file_name[100];
	if (number_of_arguments > 1) //Need more than one arg for this to work
	{
		int index = 1;
		while (index < number_of_arguments - 1)
		{
			if (strlen(argument_array[index]) == 2 && argument_array[index][0] == '-') //If length of string is 2 and '-' then conditions are met.
			{
				if (argument_array[index][1] == 'i') // -i for input, one character
				{
					//We know that the input file is the next in the argument list
					printf("the input file is: %s \n", argument_array[index + 1]);
					strcpy(input_file_name, argument_array[index + 1]); //+1 to the index because the next word is the input file, then copies this into input string
					//Skip the next item because we've already dealt with it in previous line
					index++;
				}
				else if (argument_array[index][1] == 'o') // -o for output, one character
				{
					//We know that the output file is the next in the argument list
					printf("the output file is: %s \n", argument_array[index + 1]);
					strcpy(output_file_name, argument_array[index + 1]);
					//Skip the next item because we already dealt with it
					index++;
				}
			}
			index++; //Skip to the next item after the input
		}
	}

	FILE *file = fopen(input_file_name, "r");		  //Opens and reads file
	FILE *write_file = fopen(output_file_name, "a+"); //Opens and appends file

	//Variables used
	char line[1000];	 //Stores a single line
	char token[100];	 //Stores a single token (a word)
	int token_index = 0; //Stores where we are in the token array
	int line_index = 0;	 //Stores where we are in the line array
	int comments = 0;	 //Will use this to count comments
	int line_count = 0;	 //Simple line count
	int flag = 0;		 //A flag for the comments to comments_string
	int flag_param = 0;	 //A flag for the @param java comment to param_string
	int flag_return = 0; //A flag for @return java comment to return_string
	int flag_author = 0; //A flag for @author java comment to return_string
	int flag_method = 0;
	int flag_class = 0;
	char comments_string[1000];	   //String to store the comments
	char return_string[1000] = ""; //String to store return java comment
	char param_string[1000] = "";  //String to store parameter java comment
	char author_string[1000] = ""; //String to store author java comment
	char method_string[1000] = "";
	char class_string[1000] = "";
	int blank_lines = 0;	   //For non blank lines counter
	int print_flag_param = 0;  //Flag to print the param
	int print_flag_return = 0; //Flag to print the return
	int print_flag_author = 0;
	int print_flag_class = 0;
	int print_flag_method = 0;
	int counter_for_public_void = 0; //Counter when public void is detected (not for class) in tokens
	int counter_for_class = 0;		 //Counter when class is detected in tokens

	while (fgets(line, 1000, file) != NULL) //Grabs the file and starts a while loop for each line
	{
		line_count++;					//Iterates through the lines and starts a counter
		int line_length = strlen(line); //Sets the length of the line as the strlen
		line_index = 0;					//Sets the index at position 0

		//Loops over every line in the given file
		while (line_index < line_length && line[line_index] != '\n')
		{
			//Get the next token (word)
			token[0] = '\0'; //Reset token by setting first character to '\0'
			token_index = 0; //Reset token index

			//Loops over each token in each line
			while (line[line_index] != ' ' && line[line_index] != '\n' && line[line_index] != '\t')
			{
				token[token_index] = line[line_index]; //Copy the chracter into token
				line_index++;						   //Increments to move along one in the sentence
				token_index++;						   //Increments to move along one in the token (word)
			}

			//End of token
			token[token_index] = '\0'; //Inserts a '\0' to mark the end

			if (strcmp(token, "/**") == 0)
			{
				comments++; //Increments the javadoc count
				flag = 1;
				fprintf(write_file, "%s", token); //Prints the token /** to the output file
			}
			if (strcmp(token, "{") == 0)
			{
				flag = 0;									//Ends loop, as flag is 0 -> NO spaces added after
				strcat(comments_string, token);				//Adding the words(tokens) to the comments string
				strcat(comments_string, "\n");				//Adding newline at the end of each comment
				fprintf(write_file, "%s", comments_string); //Writing the comments string to the txt
				comments_string[0] = '\0';					//Prints the comments string
			}
			if (flag == 1) //When true, store token into the comment_string and prints a space after
			{
				strcat(comments_string, token);
				strcat(comments_string, " ");
			} //Set the flag to 1 & after each comment it adds a space

			//To print the method without the int or void or what follows after.
			if (strcmp("public", token) == 0) //Detects the token "public"
			{
				flag_method = 1; //Token is now flagged
				counter_for_public_void = 1;
			}
			if (flag_method == 1) //When equal to 1, start counter. So when public is detected.
			{
				counter_for_public_void++;
			}
			if (counter_for_public_void == 4) //Fourth word, concat and print e.g "public void int..."
			{
				strcat(method_string, token);
				print_flag_method = 1; //Prints only when it gets to the fourth word.
			}
			if (strcmp(token, "{") == 0) //Detects { and stops counter, this is now when it should print.
			{
				flag_method = 0;			 //No longer flagged, stop counting and stops concatinating words into the string.
				counter_for_public_void = 0; //Reset counter.
			}

			//To print the class name only.
			if (strcmp("class", token) == 0) //Class is second word "public class..."
			{
				flag_class = 1;
				flag_method = 0; //Stops the counter, doesn't flag and count this as a "method" as it has detected the token "class"
				counter_for_class = 1;
			}
			if (flag_class == 1) //When true, and class is detected.
			{
				counter_for_class++;
			}
			if (counter_for_class == 3) //Third word
			{
				strcat(class_string, token);
				print_flag_class = 1; //Prints only when it reaches the third word.
			}
			if (strcmp(token, "{") == 0) //End of token to be flagged when { detected.
			{
				flag_class = 0;
				counter_for_class = 0; //Counter reset.
			}

			if (strcmp(token, "*/") == 0)
			{
				flag_author = 0;
			}
			if (flag_author == 1)
			{
				strcat(author_string, token);
				strcat(author_string, " ");
			}
			//New flag to detect if a comment contains a javadoc comment with @author
			if (strcmp(token, "@author") == 0)
			{
				flag_author = 1;
				print_flag_author = 1;
			}

			//New flag to detect if a comment contains a javadoc comment with @param
			if (strcmp(token, "*/") == 0) //End of parameter loop
			{
				flag_param = 0;
			}
			if (flag_param == 1)
			{
				strcat(param_string, token);
				strcat(param_string, " ");
			}
			if (strcmp(token, "@param") == 0)
			{
				flag_param = 1;
				print_flag_param = 1;
			}

			//Now the same for the @return
			if (strcmp(token, "*/") == 0)
			{
				flag_return = 0;
			}
			if (flag_return == 1)
			{
				strcat(return_string, token);
				strcat(return_string, " ");
			}
			if (strcmp(token, "@return") == 0)
			{
				flag_return = 1;
				print_flag_return = 1;
			}
			line_index++; //Moves past the space in the sentence and counts
		}

		if (print_flag_author == 1) //After detecting if the token author is in the line, it prints what's in the string
		{
			printf("Author: %s\n\n", author_string);
			print_flag_author = 0; //Sets to 0 again after printing.
			strcpy(author_string, "");
		}

		if (print_flag_method == 1)
		{
			printf("Method: %s\n\n", method_string);
			print_flag_method = 0;
			strcpy(method_string, "");
		}

		if (print_flag_class == 1)
		{
			printf("Class: %s\n\n", class_string);
			print_flag_class = 0;
			strcpy(class_string, "");
		}

		if (print_flag_return == 1) //Flag for when @return is found, now can print what is in the string
		{
			printf("Returns: %s\n\n", return_string);
			print_flag_return = 0;
			strcpy(return_string, "");
		}

		if (print_flag_param == 1) //Same for @param as for @return, prints what is in the param string
		{
			printf("Parameter: %s\n\n", param_string);
			print_flag_param = 0;
			strcpy(param_string, "");
		}

		//When the line_index is 0 then the loop never entered and this line was only a \n character
		if (line_index == 0)
		{
			blank_lines++;
		}
	}

	//Print file statistics
	printf("\n");
	printf("Total comments              = %d\n", comments);
	printf("Total lines				    = %d\n", line_count);
	printf("Total non blank lines       = %d\n", line_count - blank_lines);
	return 0;
}
