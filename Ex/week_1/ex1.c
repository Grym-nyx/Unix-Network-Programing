/* Nguyen Thanh Nam - 18020925 */
/* Y tuong: Su dung bang ASCII de xac dinh so, ky tu, ki hieu */
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char str[1000];
	int nu[3] = { 0 };
	printf("Enter the string: ");
	scanf("%s", str);
	int len = strlen(str);	

	for(int i = 0; i < len; i++){
		if (str[i] >= 48 && str[i] <= 57) {
			nu[0]++;	
		}
		else if ((str[i] >=65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122)) {
			nu[1]++;
		}
		else {
			nu[2]++;
		}
	}
	
	printf("the number: %d\n", nu[0]);
	printf("the letter: %d\n", nu[1]);
	printf("the symbol: %d\n", nu[2]);
	
	return 0;
}	
