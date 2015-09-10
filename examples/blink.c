#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* 96BoardsGPIO header file */
#include <gpio.h>

/* Just make the pin numbers easier to remember */
#define GPIO_P1 23

int main(int argc, char * argv[])
{
    int x;
    char board[25];

    if (argc == 2 && (!strcasecmp(argv[1], "dragon") || !strcasecmp(argv[1], "hikey")|| !strcasecmp(argv[1], "bubblegum")) ) {
        strcpy(board,argv[1]);
    }else{
        strcpy(board,"dragon");
    }
    if (!init_96Boards_GPIO_library(board)){
         // Open the GPIO for use.  Do so by pin number on the 
         // Low Speed Expansion Connector.               
         if (!open_GPIO_Board_pin_number(GPIO_P1)){
             // Set the GPIO to output
             if (!setup_GPIO(GPIO_P1, "out")){
                 // Now blink the pin.
                 for (x=0;x<10;x++){
                     digitalWrite(GPIO_P1, HIGH);
                     usleep(500000);
                     digitalWrite(GPIO_P1, LOW);
                     usleep(500000);
                 }
             } else {
                 /* Could not set direction */
                 return(-1);
             }
             // Now close the GPIO used
             close_GPIO(GPIO_P1);
         } else {
             /* Could not open GPIO pin */
             return(-1);
         }
     }else{
         /* Could not initi library */
         return(-1);
     }
     // Exit the example program
     return 0;
}
