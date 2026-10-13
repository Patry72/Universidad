#!/usr/bin/env python

import RPi.GPIO as GPIO
import time

BUZZER = 7

GPIO.setmode(GPIO.BOARD)
GPIO.setup(BUZZER, GPIO.OUT)

while True:
   GPIO.output(BUZZER, True)
   time.sleep(0.001136)
   GPIO.output(BUZZER, False)
   time.sleep(0.001136)
