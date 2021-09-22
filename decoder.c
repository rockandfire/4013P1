#include <pigpio.h>
#include <stdbool.h>
#include <stdio.h>

char binToHex(int a, int b, int c, int d) //super scuffed 'binary' to hex conversion
{
    int binConv = 0;

    if (a == 1)
    {
        binConv += 1000;
    }

    if (b == 1)
    {
        binConv += 100;
    }

    if (c == 1)
    {
        binConv += 10;
    }

    if (d == 1)
    {
        binConv += 1;
    }

    switch (binConv) //returns a hex number based on sum
    {
        case 0:
        {
            return '0';
        }
        case 1:
        {
            return '1';
        }
        case 10:
        {
            return '2';
        }
        case 11:
        {
            return '3';
        }
        case 100:
        {
            return '4';
        }
        case 101:
        {
            return '5';
        }
        case 110:
        {
            return '6';
        }
        case 111:
        {
            return '7';
        }
        case 1000:
        {
            return '8';
        }
        case 1001:
        {
            return '9';
        }
        case 1010:
        {
            return 'A';
        }
        case 1011:
        {
            return 'B';
        }
        case 1100:
        {
            return 'C';
        }
        case 1101:
        {
            return 'D';
        }
        case 1110:
        {
            return 'E';
        }
        case 1111:
        {
            return 'F';
        }
    }
}

int getCounterVal(int pin) //converts state of GPIO pin as int to pass to binToHex
{

    int val = 0;

    if(gpioRead(pin) == PI_ON)
    {
        val = 1;
    }

    return val;
}

int main ()
{
    if (gpioInitialise() < 0)
    {
        return 0;
    }
    else
    {
        gpioSetMode(12, PI_INPUT);
        gpioSetMode(16, PI_INPUT);
        gpioSetMode(20, PI_INPUT);
        gpioSetMode(21, PI_INPUT);
        int a, b, c, d;

        while (1)
        {
            a = getCounterVal(12);
            b = getCounterVal(16);
            c = getCounterVal(20);
            d = getCounterVal(21);
            char result[] = {'0', 'x', binToHex(a, b, c, d), '\0'};
            printf(result);
        }
    }

    

    gpioTerminate();
    return 0;
}