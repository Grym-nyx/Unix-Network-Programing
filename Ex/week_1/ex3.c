/* Nguyen Thanh Nam - 18020925 */
/* Y tuong: open FILE, check character, put to newFile */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	char ch, file_read[50], file_write[50];
	
	FILE * fpr;
	FILE * fpw;

	printf("Enter the file name you want to read: \n");
	gets(file_read);
	// scanf("%s", file_read);

	printf("Enter the file name you want to write to: \n");
	gets(file_write);
	// scanf("%s", file_write);

	fpr = fopen(file_read, "r");
	fpw = fopen(file_write, "w");

	/* FILE reading handling error */
	if (fpr == NULL || fpw == NULL) {
		perror("Error while reading the file: ");
		exit(EXIT_FAILURE);
	}

	while ((ch = fgetc(fpr)) != EOF) {
		if ((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122)) {
			fputc(ch, fpw);
		}
	}

	printf("Copied all the alphabetical character.");

	fclose(fpr);
	fclose(fpw);

	return 0;
}
