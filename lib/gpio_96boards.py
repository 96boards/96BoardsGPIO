import ctypes


class GPIO(object):
    DIRECTIONS = ('in', 'out')
    HIGH = 1
    LOW = 0

    _lib = None

    def __init__(self, board_name, pins):
        assert type(board_name) == str
        if not GPIO._lib:
            GPIO._lib = ctypes.CDLL('lib96BoardsGPIO.so')

        if GPIO._lib.init_96Boards_GPIO_library(board_name):
            raise ValueError('Unknown board name: ' + board_name)

        for pin in pins:
            if type(pin[0]) != int:
                raise ValueError('Pin number must be an integer')
            if pin[1] not in GPIO.DIRECTIONS:
                raise ValueError('Invalid direction for pin(%d): %s' % pin)
        self.pins = pins
        self.in_ctx = False

    def __enter__(self):
        for pin, direction in self.pins:
            if GPIO._lib.open_GPIO_Board_pin_number(pin):
                raise RuntimeError('Unable to open pin: %d' % pin)
            if GPIO._lib.setup_GPIO(pin, direction):
                raise RuntimeError(
                    'Unable to set direction for pin(%d) to: %s' % (
                        pin, direction))
            self.in_ctx = True
        return self

    def __exit__(self, type, value, traceback):
        self.in_ctx = False
        for pin, _ in self.pins:
            GPIO._lib.close_GPIO(pin)

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
