// gcc -Wall -pthread -o testGPIO testGPIO.c -lpigpio -lrt
#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>

#define TEST_PIN 23

void setPinStates(){
	gpioSetMode(TEST_PIN, PI_OUTPUT);
}

void initialize(){
    if( gpioInitialise() > 0){
        printf("GPIO Library initialized!");
        setPinStates();
    }
    else {
        printf("GPIO initialization failed");
    }
}

void terminate(){
    gpioWrite(TEST_PIN, 0);
    gpioTerminate();
}

int main(){
	initialize();
	char keyboard_input;
	printf("Select a new V_IN then press ENTER to exit. /n");
	gpioWrite(TEST_PIN, 1);
	scanf("%c", &keyboard_input);
	gpioWrite(TEST_PIN, 0);
	terminate();
}
