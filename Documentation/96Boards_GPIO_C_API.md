#96BoardsGPIO C API
##Overview
The C API is very straightforward to use.  There are currently only 6 calls available. They are:

 1. unsigned int gpio_id(const char *pin_name);
 2. unsigned int gpio_by_letter(char alpha);
 3. unsigned int gpio_by_pin(char pin_number);
 4. int gpio_open(unsigned int gpio_id, const char *direction);
 5. int digitalRead(unsigned int gpio_id);
 6. int digitalWrite(unsigned int gpio_id, unsigned int value);

If you have programmed an Arduino or other embedded board the GPIO the calls should look familiar to you.

##gpio_id
GPIO's on a given board are identified as GPIO_A ... GPIO_L. Each SoC has its
own internal GPIO ID that corresponds to this. The gpio_id will tranform this
name into the SoC's expected value. eg:
```C
 gpio_id("GPIO_A") == 488 // hikey
 gpio_id("GPIO_A") == 36  // dragonboard410c
 gpio_id("BAD VALUE") == -1
```

##gpio_by_letter
This is a helper function that can be used instead of gpio_id.
```C
 gpio_by_letter('A') == gpio_id("GPIO_A")
```

##gpio_by_pin
This is a helper function that can be used instead of gpio_id.
```C
 gpio_by_pin(23) == gpio_id("GPIO_A")
```

##gpio_open
Open a given GPIO in a given direction ("in" or "out")
```C
    if (!gpio_open(gpio_id("GPIO_A", "out")){
        GPIO pin is avalible for use
    } else {
        open GPIO call failed
    }
```

##digitalRead
Once you open a GPIO pin and set it to input you can use this call to see if the pin is HIGH, or LOW or -1 if some sort of error happened.  This call will return HIGH or LOW on a good call or -1 if it fails for any reason.


```C
    int x;

    if (x = digitalRead(23) != -1){
        if (x == HIGH){
            pin read, it's HIGH
        } else {
            pin read, it's LOW
        }
    } else {
        digitialRead call failed, returned -1 error
    }
```


##digitalWrite
Once you open a GPIO pin and set it to output you can use this call to set the pin HIGH or LOW.  This call will return 0 on a good call or -1 if it fails for any reason.


```C
    if (!digitalWrite(23, HIGH)){
        digitialWrite call was successful, output is HIGH
    } else {
        digitalWrite call failed, some kind of error happened
    }

    if (!digitalWrite(23, LOW)){
        digitialWrite call was successful, output is LOW
    } else {
        digitalWrite call failed, some kind of error happened
    }
```
