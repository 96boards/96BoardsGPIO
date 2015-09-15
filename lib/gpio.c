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

#include "__gpio.h"

static struct GPIO_INFO gpio_info[NUM_PINS];
static struct GPIO_INFO * info;

static struct GPIO_VALUES* get_from_config() {
    int i=0, rc;
    FILE *fp;
    char line[256];
    struct GPIO_VALUES *board = NULL;

    fp = fopen("/etc/96boards_gpio.conf", "r");
    if (fp) {
        board = malloc(sizeof(struct GPIO_VALUES) * NUM_PINS);
        while(fgets(line, sizeof line, fp)) {
            if (*line == '#' || *line == '\0') continue;
            if (i>=NUM_PINS) {
                fprintf(stderr, "Invalid format for /etc/96boards_gpio.conf (too many pins)\n");
                exit(1);
            }
            board[i].Board_text_name = malloc(128);
            board[i].SoC_text_name = malloc(128);
            rc = sscanf(line, "%d %d %s %s",
                        &(board[i].SoC_number), &(board[i].Board_pin_number),
                        board[i].Board_text_name, board[i].SoC_text_name);
            if (rc != 4) {
                fprintf(stderr, "Invalid line in /etc/96boards_gpio.conf:\n%s\n", line);
                exit(1);
            }
            i++;
        }
    }
    return board;
}

void close_96Boards_GPIO_library(void){
    /* Clean up all open GPIO and then exit */
    int x;
    
    for (x=GPIO_OFFSET;x<(GPIO_OFFSET+NUM_PINS);x++){
        close_GPIO(x);
    }
    return;    
}

int init_96Boards_GPIO_library(char * board){
    int ret = -1;
    /* Set the entire array to 0's */
    memset(gpio_info,0,sizeof(gpio_info));
    
    current_board = get_from_config();
    if(current_board){
        ret = 0;
    } else if (!strcasecmp(board, "dragon")){
        current_board = dragon;
        ret = 0;
    } else if (!strcasecmp(board, "hikey")){
        current_board = hikey;
        ret = 0;
    } else if (!strcasecmp(board, "bubblegum")){
        current_board = bubblegum;
        ret = 0;
    }
    atexit(close_96Boards_GPIO_library);
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

