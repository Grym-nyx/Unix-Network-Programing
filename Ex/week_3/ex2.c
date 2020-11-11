/* Nguyen Thanh Nam - 18020925 */
/* Y tuong: doc du lieu tu file pkt_hdr.bin vao struct moi tao
 * 	    in ra du lieu tu struct moi tao do */
#include <stdio.h>
#include <stdlib.h>

#define MAX_NAME_LENGTH 100

struct udphdr {
	unsigned short fileSize;
	unsigned char fileType;
	unsigned char fileNameLength;
	char fileName[MAX_NAME_LENGTH];
};

int main(int argc, char *argv) {
	struct udphdr pkt_hdr;

	FILE * file = fopen("pkt_hdr.bin","rb");

	fread(&pkt_hdr,sizeof(struct udphdr), 1, file);
	
	printf(" file_size: %hu\n file_type: %hhx\n file_name: %s\n fileNameLength: %hhx\n", 
			pkt_hdr.fileSize, pkt_hdr.fileType, pkt_hdr.fileName, pkt_hdr.fileNameLength);
}	
