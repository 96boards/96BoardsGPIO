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

#define HIGH 1
#define LOW 0


#ifdef __cplusplus
extern "C" {
#endif

// Look up gpio ID by pin name, eg "GPIO_A"
unsigned int gpio_id(const char *pin_name);
// Look up gpio ID by letter, eq 'A'
unsigned int gpio_by_letter(char alpha);
// Look up gpio ID by pin number, eq 23
unsigned int gpio_by_pin(char pin_number);
// Open a gpio in either "INPUT" or "OUTPUT"
int gpio_open(unsigned int gpio_id, const char *direction);
// Wait for an interrupt on a GPIO in INPUT mode
int digitalRead(unsigned int gpio_id);
// Set a GPIO in OUTPUT mode to HIGH(1) or LOW(0)
int digitalWrite(unsigned int gpio_id, unsigned int value);

#ifdef __cplusplus
}
#endif
