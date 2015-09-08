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

struct GPIO_INFO {
    int SoC_number;
    int direction_fd;;
    int value_fd;
    char * gpio;
    char * path;
};

struct GPIO_VALUES {
    int SoC_number;
    int Board_pin_number;
    char * Board_text_name;
    char * SoC_text_name;
    char * SoC_text_number;
    };

static struct GPIO_VALUES * current_board;

static struct GPIO_VALUES dragon[] = { 
    {  36, 23, "GPIO_A", "GPIO_36", "36" },
    {  12, 24, "GPIO_B", "GPIO_12", "12" },
    {  13, 25, "GPIO_C", "GPIO_13", "13" },
    {  69, 26, "GPIO_D", "GPIO_69", "69" },
    { 115, 27, "GPIO_E", "GPIO_115", "115" },
/*    {   4, 28, "GPIO_F", "GPIO_4", "  4" },*/
    { 507, 28, "GPIO_F", "GPIO_4", "507" },
    {  24, 29, "GPIO_G", "GPIO_24", "24" },
    {  25, 30, "GPIO_H", "GPIO_25", "25" },
    {  35, 31, "GPIO_I", "GPIO_35", "35" },
    {  34, 32, "GPIO_J", "GPIO_34", "34" },
    {  28, 33, "GPIO_K", "GPIO_28", "28" },
    {  33, 34, "GPIO_L", "GPIO_33", "33" },
};

static struct GPIO_VALUES hikey[] = { 
    {  488, 23, "GPIO_A", "GPIO2_0",		 "488" },
    {  489, 24, "GPIO_B", "GPIO2_1",		 "489" },
    {  490, 25, "GPIO_C", "GPIO2_2",		 "490" },
    {  491, 26, "GPIO_D", "GPIO3_3",		 "491" },
    {  492, 27, "GPIO_E", "GPIO2_4",		 "492" },
    {  415, 28, "GPIO_F", "BL_PWDN0GPIO9_1",	 "415" },
    {  463, 29, "GPIO_G", "GPIO6_7_DSI_TE0",	 "463" },
    {  495, 30, "GPIO_H", "GPIO2_7",		 "495" },
    {  426, 31, "GPIO_I", "ISP_RSTB0_GPIO10_2",	 "426" },
    {  433, 32, "GPIO_J", "ISP_RSTB0_GPIO9_1",	 "433" },
    {  427, 33, "GPIO_K", "GSP_RSTB1_GPIO10_3",	 "427" },
    {  434, 34, "GPIO_L", "ISP_RSTB0_GPIO9_2",	 "434" },
};

static struct GPIO_VALUES bubblegum[] = {
    { 19, 23, "GPIO_A", "GPIOD19", "19" },
    { 18, 24, "GPIO_B", "GPIOD18", "18" },
    { 17, 25, "GPIO_C", "GPIOD17", "17" },
    { 16, 26, "GPIO_D", "GPIOD16", "16" },
    { 15, 27, "GPIO_E", "GPIOD15", "15" },
    { 14, 28, "GPIO_F", "GPIOD14", "14" },
    { 13, 29, "GPIO_G", "GPIOD13", "13" },
    { 12, 30, "GPIO_H", "GPIOD12", "12" },
    { 11, 31, "GPIO_I", "GPIOD11", "11" },
    { 10, 32, "GPIO_J", "GPIOD10", "10" },
    { 27, 33, "GPIO_K", "GPIOD27", "27" },
    { 26, 34, "GPIO_L", "GPIOD26", "26" },
};

static struct GPIO_INFO gpio_info[12];
static struct GPIO_INFO * info;

int init_96Boards_GPIO_library(char * board){
    int ret = -1;
    
    memset(gpio_info,0,sizeof(gpio_info));
    if (!strcmp(board, "dragon")){
        current_board = dragon;
        ret = 0;
    } else if (!strcmp(board, "hikey")){
        current_board = hikey;
        ret = 0;
    } else if (!strcmp(board, "bubblegum")){
        current_board = bubblegum;
        ret = 0;
    }
    return(ret);
}

int open_GPIO( struct GPIO_INFO * info, char * gpio_path ) {
    int ret = -1;
    int fd;
    char gpio_files[50];
    
    if (info->SoC_number){
        if ((ret = fd = open("/sys/class/gpio/export", O_WRONLY))!= -1){
            if ((ret = write(fd, info->gpio, strlen(info->gpio)))!= -1){
                if (!(ret = close(fd))){
                    strcpy(gpio_files, gpio_path);
                    strcat(gpio_files, "direction");
                    if ((ret = info->direction_fd = open(gpio_files, O_RDWR))!= -1){
                        strcpy(gpio_files, gpio_path);
                        strcat(gpio_files, "value");
                        if ((ret = info->value_fd = open(gpio_files, O_RDWR))!= -1){
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
    
    if (pin >=23 && pin <= 34 ){
        info = &gpio_info[pin - GPIO_OFFSET];
        for (x=0, info->SoC_number = 0, board = current_board; x<12 ;x++, board++){
            if (board->Board_pin_number == pin){
                info->SoC_number = board->SoC_number;
                info->gpio = board->SoC_text_number;
                strcpy(gpio_path,"/sys/class/gpio/gpio");
                strcat(gpio_path,info->gpio);
                strcat(gpio_path,"/");
                ret = open_GPIO( info, gpio_path );
                break;
            }
        }
    }
    return(ret);
}

int open_GPIO_SoC_number( int gpio ) {
    int ret = -1;
    int x;
    char gpio_path[50];
    struct GPIO_VALUES * board;
    
    for (x=0, info->SoC_number = 0, board  = current_board; x<12 ;x++, board++){
        if (board->SoC_number == gpio){
            info = &gpio_info[x];
            info->SoC_number = board->SoC_number;
            info->gpio = board->SoC_text_number;
            strcpy(gpio_path,"/sys/class/gpio/gpio");
            strcat(gpio_path,info->gpio);
            strcat(gpio_path,"/");
            ret = open_GPIO( info, gpio_path );
            break;
        }
    }
    return(ret);
}

int close_GPIO( int pin ) {
    int ret = -1;
    int fd;
    
    if (pin >=23 && pin <= 34 ){
        info = &gpio_info[pin - GPIO_OFFSET];
        if (info->SoC_number ){
            if ((ret = fd = open("/sys/class/gpio/unexport", O_WRONLY))!= -1){
                close(info->direction_fd);
                close(info->value_fd);
                if ((ret = write(fd, info->gpio, strlen(info->gpio)))!= -1){
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
    
    
    if (pin >=23 && pin <= 34 ){
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
    
    if (pin >=23 && pin <= 34 ){
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

