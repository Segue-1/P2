#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "token.h"
#include "stack.h"
#include "scanner.h"
#include "testScanner.h"

void print_usage();
static char *read_stdin();
static char *load_file(char const * path);

int main (int argc, char* argv[]) {

	// Check number of arguments
	if (argc > 2) {
		printf("Too many arguments\n");
		print_usage();
		exit(0);
	}	
	else if (argc == 2) {
		// File was passed, check if it exists
		if (access(argv[1], F_OK) == 0) {
			printf("File exists, continuing...\n");
		}
		else {
			printf("File doesn't exist, exiting...\n");
			exit(0);
		}
		
		// Load input and call test scanner
		char *input = load_file(argv[1]);	
		testScanner(input);

		exit(0);
	}
	else if (argc == 1) {


		printf("Getting input from stdin...\n");
		printf("Simulate EOF by entering EOF0 \n");

		char *input = read_stdin();
		testScanner(input);

		exit(0);
	}
	else {
		// Shouldn't be possible... 0 args
		printf("How did you do that?\n");
		print_usage();
		exit(0);
	}


	return 0;
}


void print_usage(){

	printf("---Usage---\n");
	printf("Enter './main' to use text from keyboard\n");
	printf("Enter './main < FILENAME' to use text from file\n");
	printf("Enter './main FILENAME' to use text from file\n");

}

static char * read_stdin(){


	size_t capacity = 4096, offset = 0;  // Set size of buffer and offset.
	char *buffer = malloc(capacity * sizeof (char));
	int c; 

	// Read until EOF
	while ((c = fgetc(stdin)) != '0' && !feof(stdin)) {
				
		
		buffer[offset] = c;	
		if (++offset == capacity) {
			buffer = realloc(buffer, (capacity *= 2) * sizeof (char));
		}
			

	}

	// Trim and pad
	buffer = realloc(buffer, (offset + 1) * sizeof (char));
	buffer[offset] = '\0';

	return buffer;
}

// Loads text from file into char string
static char* load_file(char const* path)
{
    	char* buffer = 0;
    	long length;
    	FILE * f = fopen (path, "rb"); 

	if(f) {

		fseek (f, 0, SEEK_END);
      		length = ftell (f);
      		fseek (f, 0, SEEK_SET);
      		buffer = (char*)malloc ((length+5)*sizeof(char));

      		if (buffer) {
        		fread (buffer, sizeof(char), length, f);
      		}

		fclose (f);

	}
	
	// Pad will null terminator
	buffer[length] = ' ';
	buffer[length + 1] = 'E';
	buffer[length + 2] = 'O';
	buffer[length + 3] = 'F';
	buffer[length + 4] = '\0';

    	return buffer;
}
