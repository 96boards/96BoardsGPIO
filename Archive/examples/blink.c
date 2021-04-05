#include <stdio.h>

/* 96BoardsGPIO header file */
#include <gpio.h>

/* Just make the pin numbers easier to remember */

int main(int argc, char * argv[])
{
	int x;
	unsigned int gpio_a = gpio_id("GPIO_A");
	/*
	 * GPIO_A can also be looked up with:
  	 *  gpio_a = gpio_by_letter('A');
	 *  gpio_a = gpio_by_pin(23);
	 */

	// Open the GPIO for use.  Do so by pin number on the
	// Low Speed Expansion Connector.
	if (!gpio_open(gpio_a, "out")) {
		for (x=0; x<10; x++) {
			digitalWrite(gpio_a, HIGH);
			usleep(500000);
			digitalWrite(gpio_a, LOW);
			usleep(500000);
		}
	} else {
		fprintf(stderr, "Unable to open GPIO_A\n");
		return -1;
	}

	return 0;
}
