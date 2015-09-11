#96BoardsGPIO C API
##Overview
The C API is very straightforward to use.  There are currently only 6 calls available. They are:  
 1. int init_96Boards_GPIO_library(char * );  
 2. int open_GPIO_Board_pin_number( int );  
 3. int setup_GPIO( int, char * );  
 4. int digitalRead( int );  
 5. int digitalWrite( int, int );  
 6. int close_GPIO( int );  


If you have programmed an Arduino or other embedded board the GPIO the calls should look familiar to you.  
##init_96Boards_GPIO_library
This call can take the name of the board it’s running on, and returns 0 on a good init or -1 if it fails for any reason.  Or if you have put a config file in /etc called “96boards_gpio.conf” and it contains the proper board information, you don’t need to supply a board name at all.  If a proper config file is in /etc/96boards_gpio.conf it will override the named call anyway.  
Example of DragonBoard 410c config file:  

```
        36  23 GPIO_A GPIO_36  
        12  24 GPIO_B GPIO_12  
        13  25 GPIO_C GPIO_13  
        69  26 GPIO_D GPIO_69  
        115 27 GPIO_E GPIO_115  
        507 28 GPIO_F GPIO_4  
        24  29 GPIO_G GPIO_24  
        25  30 GPIO_H GPIO_25  
        35  31 GPIO_I GPIO_35  
        34  32 GPIO_J GPIO_34  
        28  33 GPIO_K GPIO_28  
        33  34 GPIO_L GPIO_33
```

Example of a HiKey board config file:  

```
        488  23 GPIO_A GPIO_488  
        289  24 GPIO_B GPIO_489  
        490  25 GPIO_C GPIO_490  
        491  26 GPIO_D GPIO_491  
        492 27 GPIO_E GPIO_492  
        415 28 GPIO_F GPIO_415  
        463  29 GPIO_G GPIO_463  
        495  30 GPIO_H GPIO_495  
        426  31 GPIO_I GPIO_426  
        433  32 GPIO_J GPIO_433  
        427  33 GPIO_K GPIO_427  
        434  34 GPIO_L GPIO_434  
```

The file format is: int, int, string, string which is: GPIO calling number, Low Speed Pin number, 96Boards GPIO name, SoC’s sys/class/gpio name.  If it’s in place, the init call uses this data to setup the library and does not need the name of the board to init the library.  
A call where the config file is in /etc/96boards_gpio.conf  


```C  
    if (!init_96Boards_GPIO_library()){  
	do something init was good  
    } else {  
        exit init was bad  
    }  
```


A call for a dragon board would look like:  


```C  
    if (!init_96Boards_GPIO_library(“dragon”)){  
        do something init was good  
    } else {  
	exit init was bad  
    }  
```


A call for a hikey board would look like:  


```C  
    if (!init_96Boards_GPIO_library(“hikey”)){  
        do something init was good  
    } else {  
	exit init was bad  
    }  
```


This call is expected to change in the near future, once the kernel has a clear place to identify the board that we are running on the call will no longer need a config file or take a string name but will simply look in the proper kernel interface (/sys or /proc) and deal with it.  
##open_GPIO_Board_pin_number
Once the init call is successful you can open a GPIO pin for use.  Simply call the open_GPIO_Board_pin_number with the pin number from the low speed connector that you are using.  It will be in the range of pin 23 to pin 34.  The call takes one pin number at a time, to open more than one GPIO pin call the function several times with successive pin numbers.  This call will return 0 on a good call or -1 if it fails for any reason.   
A call to open the first GPIO pin would look like:  


```C  
    if (!open_GPIO_Board_pin_number(23)){  
        do something, GPIO pin is open  
    } else {  
        pin did not open exit  
    }  
```


##setup_GPIO  


Once you open a GPIO pin you need to tell the system if you are using it for input or output.  This is done using the setup_GPIO call.  This call will return 0 on a good call or -1 if it fails for any reason.  
To use the GPIO for input:  


```C  
    if (!setup_GPIO(23, “in”)){  
        do something call was succesfull, GPIO is setup for input  
    } else {  
        setup call failed pin is in unknown state  
    }  
```


To use the GPIO for output:  


```C  
    if (!setup_GPIO(23, “out”)){  
        do something call was succesfull, GPIO is setup for output  
    } else {  
        setup call failed pin is in unknown state  
    }  
```


##digitalRead  
Once you open a GPIO pin and set it to input you can use this call to see if the pin is HIGH, or LOW or -1 if some sort of error happened.  This call will return HIGH or LOW on a good call or -1 if it fails for any reason.  


```C  
    int x;  
  
    if (x = digitalRead(23) != -1){  
        if (x == HIGH){  
                do something based on the pin being high  
        } else {  
                do something based on the pin being low  
        }  
    } else {  
        something is wrong, call returned -1 error  
    }  
```


##digitalWrite  
Once you open a GPIO pin and set it to output you can use this call to set the pin HIGH or LOW.  This call will return 0 on a good call or -1 if it fails for any reason.  


```C  
    if (!digitalWrite(23, HIGH)){  
        do something based on setting the pin high.  Call was successful  
    } else {  
        some kind of error happened, call failed  
    }  
  
    if (!digitalWrite(23, LOW)){  
        do something based on setting the pin low.  Call was successful  
    } else {  
        some kind of error happened, call failed  
    }  
```

##close_GPIO  
Before exiting your application you need to close down the GPIO that you used. Simply call close_GPIO with the pin number(s) you opened.   The call takes one pin number at a time, to open more than one GPIO pin call the function several times with successive pin numbers. This call will return 0 on a good call or -1 if it fails for any reason, although generally since you are exiting your application you may not care to check the exit code.  


```C  
    close_GPIO(23);  
```