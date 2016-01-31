#include <stdio.h>

int my_funct(){
	int a = 85;

	return a;
}

int main(int argc, char *argv[]){	
	int a[10];
	for (int i = 0; i < 10; i++){
		a[i] = 0;
	}
	a[0] = my_funct();
	printf("complete!\n");
}
