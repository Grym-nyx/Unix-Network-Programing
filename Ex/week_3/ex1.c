/* Nguyen Thanh Nam - 18020925
 * Y tuong: Nhap du lieu su dung ham scanf ung voi tung kieu du lieu,
 * 	    ghi vao file su dung ham fwrite, coi pkt nhu mot doi tuong 
 * 	    */

#include <stdio.h>
#define MAX_NAME_LENGTH 100

struct udphdr {
	unsigned short fileSize;
	unsigned char fileType;
	unsigned char fileNameLength;
	char fileName[MAX_NAME_LENGTH];
};

int main(int argc, char* args) {
	struct udphdr pkt;

	printf("File size: ");
	scanf("%hu", &pkt.fileSize);

	printf("File name: ");
	scanf("%s", pkt.fileName);

	printf("File type (0x00 or 0x01): ");
	scanf("%hhx", &pkt.fileType);

	FILE * file = fopen("pkt_hdr.bin", "wb");
	fwrite(&pkt, sizeof(pkt), 1, file);
	
	fseek(file,0,SEEK_END);
	int size = ftell(file);

	printf("Size of file created: %d bytes", size);

	fclose(file);
}
