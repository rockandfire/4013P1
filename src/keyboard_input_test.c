// test for keyboard input

#include <stdio.h>

int main(){
	
	char input;
	
	// We need continuious input reading.
	printf("Enter a character: ");
	scanf("%c", &input);
	if( input == ' ' ){
		printf("Ha! A space was entered! \n");
	}
	else{
		printf("Ah! Can't tell! You entered: %c \n", input);
	}
	
	return 0;
	
}
	
