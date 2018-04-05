/*
 * gpio.h:
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
#include <stdlib.h>
#include <stdio.h>

#include <libsoc_board.h>
#include <libsoc_gpio.h>


typedef struct _gpio_list{
	gpio *g;
	struct _gpio_list *next;
} gpio_list;

static gpio_list *gpios = NULL;
static board_config *config = NULL;

__attribute__((destructor)) static void _cleanup()
{
	gpio_list *tmp, *ptr = gpios;
	while (ptr) {
		tmp = ptr;
		libsoc_gpio_free(ptr->g);
		ptr = ptr->next;
		free(tmp);
	}
	if (config)
		libsoc_board_free(config);
}


unsigned int gpio_id(const char *pin_name)
{
	if (!config)
		config = libsoc_board_init();
	/* Some original documents refer to pins as GPIO_A, but our conf files
	 * use GPIO-A. Try to fix this for the user if needed.
	 */
	if (pin_name[4] == '_') {
		unsigned int gpio_id;
		char *fixed = strdup(pin_name);
		fixed[4] = '-';
		gpio_id = libsoc_board_gpio_id(config, fixed);
		free(fixed);
		return gpio_id;
	}
	return libsoc_board_gpio_id(config, pin_name);
}

unsigned int gpio_by_letter(char alpha)
{
	char pin_name[] = {'G', 'P', 'I', 'O', '_', 'X'};
	if (alpha < 'A' || alpha > 'A' + 12) {
		fprintf(stderr, "ERROR: Invalid pin letter: %c\n", alpha);
		exit(1);
	}
	pin_name[5] = alpha;
	return gpio_id(pin_name);
}

unsigned int gpio_by_pin(char pin_number)
{
	char pin_name[] = {'G', 'P', 'I', 'O', '_', 'X'};
	if (pin_number < 23 || pin_number > 23 + 12) {
		fprintf(stderr, "ERROR: Invalid pin number: %d\n", pin_number);
		exit(1);
	}
	pin_name[5] = pin_number - 23 + 'A';
	return gpio_id(pin_name);
}

int gpio_open(unsigned int gpio_id, const char *direction)
{
	int rc = -1;
	gpio_list *ptr;
	gpio *g = libsoc_gpio_request(gpio_id, LS_GPIO_SHARED);
	if (!g)
		return rc;
	if (!strcmp(direction, "in"))
		rc = libsoc_gpio_set_direction(g, INPUT);
	else
		rc = libsoc_gpio_set_direction(g, OUTPUT);

	ptr = calloc(sizeof(gpio_list), 1);
	ptr->g = g;
	ptr->next = gpios;
	gpios = ptr;
	return rc;
}

int digitalRead(unsigned int gpio_id)
{
	gpio_list *ptr = gpios;
	while(ptr) {
		if (ptr->g->gpio == gpio_id) {
			return libsoc_gpio_get_level(ptr->g);
		}
		ptr = ptr->next;
	}
	return -1;
}

int digitalWrite(unsigned int gpio_id, unsigned int value)
{
	gpio_list *ptr = gpios;
	while(ptr) {
		if (ptr->g->gpio == gpio_id) {
			return libsoc_gpio_set_level(ptr->g, value);
		}
		ptr = ptr->next;
	}
	return -1;
}
