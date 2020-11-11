/* Nguyen Thanh Nam - 18020925 */
/* Y tuong: Tao 2 file: f_json, f_text, ghi du lieu vao 2 file su dung fprintf (text format) va fwrite (binary format) */
#include <stdio.h>

#define MAX_SIZE_LENGTH 100

struct pkt_hdr {
	unsigned short fileSize;
	unsigned char fileType;
	unsigned char fileNameLength;
	char fileName[MAX_SIZE_LENGTH];
};
int main(int argc, char *argv) {
	struct pkt_hdr pkt;
	
	printf("File size: ");	
	scanf("%hu", &pkt.fileSize);

	printf("File type: ");
	scanf("%hhx", &pkt.fileType);

	printf("File name: ");
	scanf("%s", pkt.fileName);

	FILE * f_json = fopen("pkt_hdr_json.txt","w");
	FILE * f_text = fopen("pkt_hdr.txt","wb");

	// write format to json file
	fprintf(f_json, "{\n\t\"fileSize\": %hu,\n\t\"fileType\": %hhx,\n\t\"fileName\": \"%s\"\n}"
			, pkt.fileSize, pkt.fileType, pkt.fileName);

	// write to text file
	fwrite(&pkt, sizeof(struct pkt_hdr), 1, f_text);

	fseek(f_json, 0, SEEK_END);
	int size_json = ftell(f_json);

	fseek(f_text, 0, SEEK_END);
	int size_text = ftell(f_text);

	if (size_json > size_text)
		printf("Json file is bigger text file.\n");
	else 
		printf("Text file is bigger json file.\n");

	fclose(f_json);
	fclose(f_text);
}
