/* Nguyen Thanh Nam - 18020925 */
/* Y tuong: copy toan bo du lieu tu anh goc toi file anh duoc tao moi 
 * 	    md5sum co the duoc chay ben ngoai tren moi truong linux */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	FILE * img_in, * img_out;

	char in_name[100], out_name[100];
	int c;

	printf("Enter the image file name: \n");
	scanf("%s", in_name);

	printf("Enter the copy image name: \n");
	scanf("%s", out_name);
	
	img_in = fopen(in_name, "r");
	img_out = fopen(out_name, "w");

	if (img_in == NULL || img_out == NULL) {
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	while ((c = fgetc(img_in)) != EOF) {
		fputc(c, img_out);
	}

	fclose(img_in);
	fclose(img_out);

	return 0;
}
