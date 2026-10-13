#!/usr/bin/env python2

import RPi.GPIO as GPIO
import time

#pyhton script using RPi.GPIO library
#+-----+-----+---------+----------+
#| BCM | wPi |   Name  | Physical |
#+-----+-----+---------+----------+
#|   2 |   8 |   SDA.1 |  3       | Button
#|   3 |   9 |   SCL.1 |  5       | Button
#|   4 |   7 | GPIO. 7 |  7       | Buzzer
#|     |     |      0v |  9       |
#|  17 |   0 | GPIO. 0 | 11       | Yellow Led
#|  27 |   2 | GPIO. 2 | 13       | Green Led
#|  22 |   3 | GPIO. 3 | 15       | Green Led
#|     |     |    3.3v | 17       |
#|  10 |  12 |    MOSI | 19       | Red Led
#|   9 |  13 |    MISO | 21       | Red Led
#|  11 |  14 |    SCLK | 23       | Yellow Led
#+-----+-----+---------+----------+


isRPI=1

#BCM gpio 27 -> RPI Physical pin 13 -> Green Led 1
if isRPI == 1:
    #GPIO.BOARD to use Physical Raspberry Pi board pin numbers
    gpiomode = GPIO.BOARD
    LED_G1 = 13
else:
    #GPIO.BCM to use Broadcom's SoC references
    gpiomode = GPIO.BCM
    LED_G1 = 27

print 'Using pin ',LED_G1

#configure GPIO mode and pin LED_G1 as output
GPIO.setmode(gpiomode)
GPIO.setup(LED_G1, GPIO.OUT)

while True:
    GPIO.output(LED_G1, True)
    time.sleep(1)
    GPIO.output(LED_G1, False)
    time.sleep(1)
