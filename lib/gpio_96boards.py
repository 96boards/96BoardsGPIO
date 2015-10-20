import ctypes


class GPIO(object):
    DIRECTIONS = ('in', 'out')
    HIGH = 1
    LOW = 0

    _lib = ctypes.CDLL('lib96BoardsGPIO.so')

    def __init__(self, pins):
        for pin in pins:
            if type(pin[0]) != int:
                raise ValueError('Pin number must be an integer')
            if pin[1] not in GPIO.DIRECTIONS:
                raise ValueError('Invalid direction for pin(%d): %s' % pin)
        self.pins = pins
        self.in_ctx = False

    def __enter__(self):
        for pin, direction in self.pins:
            if GPIO._lib.gpio_open(pin, direction):
                raise RuntimeError(
                    'Unable to set direction for pin(%d) to: %s' % (
                        pin, direction))
            self.in_ctx = True
        return self

    def __exit__(self, type, value, traceback):
        self.in_ctx = False

    @staticmethod
    def gpio_id(pin_name):
        return GPIO._lib.gpio_id(pin_name)

    def digital_read(self, pin):
        assert type(pin) == int
        if not self.in_ctx:
            raise RuntimeError('Method must be called inside context manager')
        return GPIO._lib.digitalRead(pin)

    def digital_write(self, pin, val):
        assert type(pin) == int and type(val) == int
        if not self.in_ctx:
            raise RuntimeError('Method must be called inside context manager')
        return GPIO._lib.digitalWrite(pin, val)
