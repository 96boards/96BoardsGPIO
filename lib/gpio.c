/*
 * gpio.c:
 *      Copyright (c) 2015 David Mandala <david.mandala@linaro.org
 *
 ***********************************************************************
 * This file is part of 96BoardsGPIO:
 * https://
 *
 * This library (96BoardsGPIO) is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of the
 * License.
 * 
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 ***********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "gpio.h"

#define GPIO_OFFSET 23
#define NUM_PINS 12

struct GPIO_INFO {
    int SoC_number;
    int direction_fd;
    int value_fd;
};

struct GPIO_VALUES {
    int SoC_number;
    int Board_pin_number;
    char * Board_text_name;
    char * SoC_text_name;
};

static struct GPIO_VALUES * current_board;

/*    {   4, 28, "GPIO_F", "GPIO_4", "  4" },*/
static struct GPIO_VALUES dragon[] = {
    {  36, 23, "GPIO_A", "GPIO_36" },
    {  12, 24, "GPIO_B", "GPIO_12" },
    {  13, 25, "GPIO_C", "GPIO_13" },
    {  69, 26, "GPIO_D", "GPIO_69" },
    { 115, 27, "GPIO_E", "GPIO_115" },
    { 507, 28, "GPIO_F", "GPIO_4" },
    {  24, 29, "GPIO_G", "GPIO_24" },
    {  25, 30, "GPIO_H", "GPIO_25" },
    {  35, 31, "GPIO_I", "GPIO_35" },
    {  34, 32, "GPIO_J", "GPIO_34" },
    {  28, 33, "GPIO_K", "GPIO_28" },
    {  33, 34, "GPIO_L", "GPIO_33" },
};

static struct GPIO_VALUES hikey[] = {
    {  488, 23, "GPIO_A", "GPIO2_0" },
    {  489, 24, "GPIO_B", "GPIO2_1" },
    {  490, 25, "GPIO_C", "GPIO2_2" },
    {  491, 26, "GPIO_D", "GPIO3_3" },
    {  492, 27, "GPIO_E", "GPIO2_4" },
    {  415, 28, "GPIO_F", "BL_PWDN0GPIO9_1" },
    {  463, 29, "GPIO_G", "GPIO6_7_DSI_TE0" },
    {  495, 30, "GPIO_H", "GPIO2_7" },
    {  426, 31, "GPIO_I", "ISP_RSTB0_GPIO10_2" },
    {  433, 32, "GPIO_J", "ISP_RSTB0_GPIO9_1" },
    {  427, 33, "GPIO_K", "GSP_RSTB1_GPIO10_37" },
    {  434, 34, "GPIO_L", "ISP_RSTB0_GPIO9_2" },
};

static struct GPIO_VALUES bubblegum[] = {
    { 19, 23, "GPIO_A", "GPIOD19" },
    { 18, 24, "GPIO_B", "GPIOD18" },
    { 17, 25, "GPIO_C", "GPIOD17" },
    { 16, 26, "GPIO_D", "GPIOD16" },
    { 15, 27, "GPIO_E", "GPIOD15" },
    { 14, 28, "GPIO_F", "GPIOD14" },
    { 13, 29, "GPIO_G", "GPIOD13" },
    { 12, 30, "GPIO_H", "GPIOD12" },
    { 11, 31, "GPIO_I", "GPIOD11" },
    { 10, 32, "GPIO_J", "GPIOD10" },
    { 27, 33, "GPIO_K", "GPIOD27" },
    { 26, 34, "GPIO_L", "GPIOD26" },
};

static struct GPIO_INFO gpio_info[NUM_PINS];
static struct GPIO_INFO * info;

int init_96Boards_GPIO_library(char * board){
    int ret = -1;
    
    /* Set the entire array to 0's */
    memset(gpio_info,0,sizeof(gpio_info));
    if (!strcasecmp(board, "dragon")){
        current_board = dragon;
        ret = 0;
    } else if (!strcasecmp(board, "hikey")){
        current_board = hikey;
        ret = 0;
    } else if (!strcasecmp(board, "bubblegum")){
        current_board = bubblegum;
        ret = 0;
    }
    return(ret);
}

int open_GPIO( struct GPIO_INFO * info, char * gpio_path ) {
    int ret = -1;
    int fd;
    char buf[50];

    if (info->SoC_number){
        if ((ret = fd = open("/sys/class/gpio/export", O_WRONLY))!= -1){
            ret = snprintf(buf, sizeof(buf)-1, "%d", info->SoC_number);
            if ((ret = write(fd, buf, ret))!= -1){
                if (!(ret = close(fd))){
                    strcpy(buf, gpio_path);
                    strcat(buf, "direction");
                    if ((ret = info->direction_fd = open(buf, O_RDWR))!= -1){
                        strcpy(buf, gpio_path);
                        strcat(buf, "value");
                        if ((ret = info->value_fd = open(buf, O_RDWR))!= -1){
                            ret = 0;
                        }
                    }
                }
            }
        }
    }
    return(ret);
}

int open_GPIO_Board_pin_number( int pin ) {
    int ret = -1;
    int x;
    char gpio_path[50];
    struct GPIO_VALUES * board;
    
    if (pin >=GPIO_OFFSET && pin < GPIO_OFFSET + NUM_PINS ){
        info = &gpio_info[pin - GPIO_OFFSET];
        for (x=0, board = current_board; x<NUM_PINS ;x++, board++){
            if (board->Board_pin_number == pin){
                info->SoC_number = board->SoC_number;
		snprintf(gpio_path, sizeof(gpio_path)-1, "/sys/class/gpio/gpio%d/", info->SoC_number);
                ret = open_GPIO( info, gpio_path );
                break;
            }
        }
    }
    return(ret);
}

int close_GPIO( int pin ) {
    int ret = -1;
    int fd;
    char buf[8];

    if (pin >= GPIO_OFFSET && pin < GPIO_OFFSET + NUM_PINS ){
        info = &gpio_info[pin - GPIO_OFFSET];
        if (info->SoC_number ){
            if ((ret = fd = open("/sys/class/gpio/unexport", O_WRONLY))!= -1){
                close(info->direction_fd);
                close(info->value_fd);
		ret = snprintf(buf, sizeof(buf)-1, "%d", info->SoC_number);
                if ((ret = write(fd, buf, ret))!= -1){
                    ret = close(fd);
                }
            }
        }
    }
    return(ret);
}

int setup_GPIO( int pin, char * direction ) {
    int ret = -1;
    
    if (pin >=23 && pin <= 34 ){
        info = &gpio_info[pin - GPIO_OFFSET];
        if (info->SoC_number){
            if (!strcmp(direction, "out") || !strcmp(direction, "in")){
                lseek(info->direction_fd, 0, SEEK_SET);
                if (write(info->direction_fd, direction, strlen(direction))>0){
                    ret = 0;
                }
            }
        }
    }
    return(ret);
}

int digitalRead(int pin) {
    int ret = -1;
    char value;
    
    
    if (pin >= GPIO_OFFSET && pin < GPIO_OFFSET + NUM_PINS ){
        info = &gpio_info[pin - GPIO_OFFSET];
        if (info->SoC_number){
            lseek(info->value_fd, 0, SEEK_SET);
            if (read(info->value_fd, &value, 1)==1){
                printf("value = %c\n",value);
                if (value == '1'){
                    ret = HIGH;
                }else{
                    ret = LOW;
                }
            } else {
                ret = -1;
            }
        }
    }
    return(ret);
}

int digitalWrite( int pin, int value ) {
    int ret = -1;
    
    if (pin >=GPIO_OFFSET && pin < GPIO_OFFSET + NUM_PINS ){
        info = &gpio_info[pin - GPIO_OFFSET];
        if (info->SoC_number){
            lseek(info->value_fd, 0, SEEK_SET);
            if (value == HIGH ){
                if (write(info->value_fd, "1", 1 )==1){
                    ret = 0;
                } else {
                    ret = -1;
                }
            }else if (value == LOW){
                if (write(info->value_fd, "0", 1 )==-1){
                    ret = 0;
                } else {
                   ret = -1;
                }
            }
        }
    }
    return(ret);
}

