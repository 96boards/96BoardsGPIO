/*
 *
 * demo.c
 *
 * Copyright (c) David Mandala david.mandala@linaro.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 * This demo uses the rudimentary beginnings of the 96BoardGPIO library
 * to control real world hardware.
 * 
 * One of the cool things about the 96Boards CE project is that all of the
 * boards us the same pins for the Low Speed Expansion Connector so you can
 * plug any expansion board into any 96Board and it will connect
 * electrically BUT there is a issue where GPIO is concerned.  Different
 * SoC's have different GPIO pins.  So even though electrically the pins are
 * the same it takes differnt code to enable and use the GPIO on pins 23 -
 * 34.  Not so fun.
 *
 * The 96BoardGPIO library trys to abstract the info so that you can just
 * tell it what board you are using and what pins you want to use and the
 * library does the rest.
 *
 * Early days, there are lots of improvements that can and I expect will
 * be made.  In the mean time here is a demo of running a motorized 
 * 1" 12 vdc ball valve via a 5 vdc set of relays and turning on and off
 * said relays just becasue we can. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"
#include <unistd.h>

void relay_on( int pin ){
    digitalWrite(pin, HIGH);
    printf("Relay On\n");
    usleep(800000);
}

void relay_off(int pin){
    digitalWrite(pin, LOW);
    printf("Relay Off\n");
    usleep(800000);
}

void open_valve(int pin1, int pin2){
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
    printf("Valve Open\n");
    //sleep(4);
}

void close_valve(int pin1, int pin2){
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    printf("Valve Closed\n");
    //sleep(4);
}


#define GPIO_P1 23
#define GPIO_P2 24
#define GPIO_P3 25
#define GPIO_P4 26
#define GPIO_P5 27
#define GPIO_P6 29

int main(){
    int loop1, loop2;

    if (!init_96Boards_GPIO_library("dragon")){
        // Open the GPIO for use.  Do so by pin number on the 
        // Low Speed Expansion Connector. This could have been
        // done in other ways too.
        open_GPIO_Board_pin_number(GPIO_P1);
        open_GPIO_Board_pin_number(GPIO_P2);
        open_GPIO_Board_pin_number(GPIO_P3);
        open_GPIO_Board_pin_number(GPIO_P4);
        open_GPIO_Board_pin_number(GPIO_P5);
        open_GPIO_Board_pin_number(GPIO_P6);
        
        // Now set all 4 ports that we are using to out
        setup_GPIO(GPIO_P1, "out");
        setup_GPIO(GPIO_P2, "out");
        setup_GPIO(GPIO_P3, "out");
        setup_GPIO(GPIO_P4, "out");
        setup_GPIO(GPIO_P5, "out");
        setup_GPIO(GPIO_P6, "out");
        
        
        for (loop1 = 500;loop1;loop1--){
            open_valve(GPIO_P1, GPIO_P2);
            for( loop2 = 5;loop2;loop2--){
                relay_on(GPIO_P3);
                relay_on(GPIO_P4);
                digitalWrite(GPIO_P5, HIGH);
                relay_off(GPIO_P4);
                relay_off(GPIO_P3);
                digitalWrite(GPIO_P5, LOW);
            }
            for( loop2 = 5;loop2;loop2--){
                relay_on(GPIO_P3);
                relay_on(GPIO_P4);
                digitalWrite(GPIO_P6, HIGH);
                relay_off(GPIO_P3);
                relay_off(GPIO_P4);
                digitalWrite(GPIO_P6, LOW);
            }
            close_valve(GPIO_P1, GPIO_P2);
            for( loop2 = 5;loop2;loop2--){
                relay_on(GPIO_P4);
                relay_on(GPIO_P3);
                digitalWrite(GPIO_P6, HIGH);
                relay_off(GPIO_P4);
                relay_off(GPIO_P3);
                digitalWrite(GPIO_P6, LOW);
            }
            for( loop2 = 5;loop2;loop2--){
                relay_on(GPIO_P4);
                relay_on(GPIO_P3);
                digitalWrite(GPIO_P5, HIGH);
                relay_off(GPIO_P3);
                relay_off(GPIO_P4);
                digitalWrite(GPIO_P5, LOW);
            }
            
        }
        sleep(5);
        close_GPIO(GPIO_P1);
        close_GPIO(GPIO_P2);
        close_GPIO(GPIO_P3);
        close_GPIO(GPIO_P4);
        close_GPIO(GPIO_P5);
        close_GPIO(GPIO_P6);
    }
    return (0);
}
