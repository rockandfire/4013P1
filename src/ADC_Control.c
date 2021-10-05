// Slayter Teal ECEN 4013
// To Compile use: 
// gcc -Wall -pthread -o ADC_Control ADC_Control.c -lpigpio -lrt
#include <stdio.h>
#include <unistd.h>
#include <pigpio.h> // our GPIO lib for testing

/**
 * These constants are the pin numbers for the various
 * portions of the pi.
 */ 
#define OVERFLOW 14 // read the output of the overflow bit
#define VOLTAGE_CONTROL 15 // voltage input control pin
#define COMPARATOR_INPUT 18 // read the output of the comparator
#define CLK_CONTROL 23 // output signal to stop the clock, freezing the output
#define COUNTER_CLR 24 // output signal to clear the counter
#define CAPACITOR_CONTROL 25 // shorts the capacitor at the end of the integration cycle
// the output pins
#define A 8
#define B 7
#define C 1
#define D 12

/**
 * These globals will store the state of our
 * various states. 
 */
int v_state = 0;
int reset = 0;
int captureOutput = 0;
char output[] = {'0', '0', '0', '0'};

void setPinStates(){
    gpioSetMode(OVERFLOW, PI_INPUT);
    gpioSetMode(VOLTAGE_CONTROL, PI_OUTPUT);
    // gpioSetMode(VOLTAGE_REF, PI_OUTPUT);
    gpioSetMode(COMPARATOR_INPUT, PI_INPUT);
    gpioSetMode(CLK_CONTROL, PI_OUTPUT);
    gpioSetMode(CAPACITOR_CONTROL, PI_OUTPUT);
    gpioSetMode(COUNTER_CLR, PI_OUTPUT);
    
}

void initialize(){
    if( gpioInitialise() > 0){
        printf("GPIO Library initialized! \n");
        setPinStates();
    }
    else {
        printf("GPIO initialization failed \n");
    }
}

void terminate(){
    gpioWrite(VOLTAGE_CONTROL, 0);
    gpioWrite(CLK_CONTROL, 0);
    gpioWrite(CAPACITOR_CONTROL, 0);
    gpioWrite(COUNTER_CLR, 0);
    gpioTerminate();
}

void toStringOutput(){
    if(gpioRead(A) == 1) output[3] = '1';
    if(gpioRead(B) == 1) output[2] = '1';
    if(gpioRead(C) == 1) output[1] = '1';
    if(gpioRead(D) == 1) output[0] = '1';
}

void stopTest(){ // a "test" method if you would...
    printf("End of Test -- Press Enter to Terminate \n");
    char keyboard_input;
    scanf("%c", &keyboard_input);
    terminate();
    printf("Press Ctrl+C to exit the program \n");
    scanf("%c", &keyboard_input);
}

int ADC_Control(){
    while(1){
        /**
         * Second State: The counter has overflowed, time to switch from v_in
         * to v_ref. Set captureOutput to 1 to print a output when
         * the comparator out is 0 and we transition to the final state. 
         */
        if(gpioRead(OVERFLOW) == 0){
            gpioWrite(CLK_CONTROL, 0);
            gpioWrite(VOLTAGE_CONTROL, 1);
            gpioWrite(COUNTER_CLR, 1);
            gpioWrite(CAPACITOR_CONTROL, 0);
            gpioWrite(COUNTER_CLR, 0);
            if(v_state == 0){
                v_state = 1;
                printf("We have overflow! Switching States! \n");
                printf("v_state is: %d \n", v_state);
            }
            if(captureOutput == 0){
                captureOutput = 1;
                printf("captureOutput is: %d \n", captureOutput);
            }
            // stopTest();
        }
        /**
         * v_ref has integrated to 0, comparator output should be 0.
         * The output should be ready to print.
         * RESET should be set high.
         */
        if(v_state == 1 && gpioRead(COMPARATOR_INPUT) == 0){
            printf("Comparator is 0 \n");
            gpioWrite(CLK_CONTROL, 1);
            gpioWrite(VOLTAGE_CONTROL, 0);
            if(v_state == 1) v_state = 0;
            if(reset == 0) reset = 1;
            if(captureOutput == 1){ // the final state, before reset.
                captureOutput = 0;
                printf("\n");
                
                /**
                 * At this point an output is shown on the LEDs,
                 * we need to short the capacitor, then wait for a
                 * new V_in to continue.
                 */ 
                char keyboard_input;
                printf("Select a new V_IN then press ENTER to continue. \n");
                printf("Or Press `Spacebar + Enter` to exit the program. \n");
                gpioWrite(CAPACITOR_CONTROL, 1);
                scanf("%c", &keyboard_input);
                gpioWrite(CLK_CONTROL, 1);
                gpioWrite(COUNTER_CLR, 1);
                gpioWrite(COUNTER_CLR, 0);
                if(reset == 1) reset = 0; 
                if( keyboard_input == ' ' ) return 0;
            }
        }
        
        /**
         * First state: integrator input should be v_in and
         * the counter should be counting up until OVERFLOW
         * goes high thus causing the state to change as shown
         * above.  
         */
         if(v_state == 0){
             gpioWrite(VOLTAGE_CONTROL, 0);
             gpioWrite(CLK_CONTROL, 0);
             gpioWrite(CAPACITOR_CONTROL, 0);
             if( captureOutput == 1 ) captureOutput = 0; // don't capture the output of the counter.
             if( v_state == 1 ) v_state = 0; // v_state should be v_in
        }
            
    }
}

int main(){
    initialize();
    
    ADC_Control();
    
    terminate();
    return 0;
}
