#!/usr/bin/python
import time

from gpio_96boards import GPIO

GPIO_P1 = 23
pins = (
    (GPIO_P1, 'out'),
)


def blink(gpio):
    for i in range(5):
        gpio.digital_write(GPIO_P1, GPIO.HIGH)
        time.sleep(i)
        gpio.digital_write(GPIO_P1, GPIO.LOW)
        time.sleep(1)


if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(
        description='Blink LED on GPIO 1 (pin 23)')
    parser.add_argument('board', help='Name of board. eg dragon')
    args = parser.parse_args()

    with GPIO(args.board, pins) as gpio:
        blink(gpio)
