/* Nguyen Thanh Nam - 18020925 */
/* Y tuong: create FILE, create contents, add contents to FILE,
 * 	    print size using fseek, ftell */
#include <stdio.h>
#include <stdlib.h>

#define DATA_SIZE 1000
int main(int argc, char *argv[])
{
	/* variable to store user content */
	char data[100];
	char newLine[2];
	int size;

	char name[DATA_SIZE];
	FILE * fp;
	
	printf("Create a file name: ");
	scanf("%s", name);
	fp = fopen(name, "w");

	if(fp == NULL) {
		/* FILE not created hence exit */
		printf("Unable to create file.\n");
		exit(EXIT_FAILURE);
	}

	/* Input contents from user to store in file */
	printf("Enter the contents to store in file: \n");

	/* First fgets gets the new line when input name */
	fgets(newLine, 2, stdin);
	fgets(data, DATA_SIZE, stdin);

	/* Write to file */
	fputs(data, fp);

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	/* Close file */
	fclose(fp);

	/* Successful message */
	printf("File is created and saved successfully.\n");
	printf("The size of file: %d bytes", size);

	return 0;
}

