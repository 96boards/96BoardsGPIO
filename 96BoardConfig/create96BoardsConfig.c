#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "../lib/__gpio.h"


void print_config(void)
{
    int x;
    for (x=0;x<12;x++){
        printf("%3d %3d %s %s\r\n",current_board->SoC_number, current_board->Board_pin_number, current_board->Board_text_name, current_board->SoC_text_name);
        current_board++;
    }
}

int main(int argc, char * argv[])
{
    int ret = -1;
    /* Set the entire array to 0's */
    
    if (argc == 2){
        if (!strcasecmp(argv[1], "dragon")){
            printf("# DragonBoard Config data\r\n");
            current_board = dragon;
            print_config();
            ret = 0;
        } else if (!strcasecmp(argv[1], "hikey")){
            printf("# HiKey Board Config data\r\n");
            current_board = hikey;
            print_config();
            ret = 0;
        } else if (!strcasecmp(argv[1], "bubblegum")){
            printf("# Bubblegum Board Config data\r\n");
            current_board = bubblegum;
            print_config();
            ret = 0;
        } else {
            printf("Board name did not match \"dragon\", \"hikey\", or \"bubblegum\". Try again with correct board name\r\n");
        }
    } else {
            printf("Board name did not match \"dragon\", \"hikey\", or \"bubblegum\" or no name was supplied. Try again with correct board name\r\n");
    }
    return(ret);
}
