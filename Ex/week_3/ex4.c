/* Nguyen Thanh Nam - 18020925 */
/* Y tuong: Su dung thu vien json-c 
 * 		sudo apt install libjson-c-dev
 * 		gcc ex4.c -ljson-c -o ex4 */
#include <stdio.h>
#include <json-c/json.h>

int main(int argc, char *argv) {
	FILE * fp;
	char buffer[1024];

	struct json_object *parsed_json;
	struct json_object *fileSize;
	struct json_object *fileType;
	struct json_object *fileName;

	fp = fopen("pkt_hdr_json.txt","r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	// convert file data to json object
	parsed_json = json_tokener_parse(buffer);	

	// get value of key in json object
	json_object_object_get_ex(parsed_json, "fileSize", &fileSize);
	json_object_object_get_ex(parsed_json, "fileType", &fileType);
	json_object_object_get_ex(parsed_json, "fileName", &fileName);

	printf("File size: %hu\n", json_object_get_int(fileSize));
	printf("File type: %hhx\n", json_object_get_int(fileType));
	printf("File name: %s\n", json_object_get_string(fileName));

}
