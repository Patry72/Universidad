#!/usr/bin/env bash

pin=2

cat <<EOF
shell script using wiringPi (gpio command)
+-----+-----+---------+----------+
| BCM | wPi |   Name  | Physical |
+-----+-----+---------+----------+
|   2 |   8 |   SDA.1 |  3       | Button
|   3 |   9 |   SCL.1 |  5       | Button
|   4 |   7 | GPIO. 7 |  7       | Buzzer
|     |     |      0v |  9       |
|  17 |   0 | GPIO. 0 | 11       | Yellow Led
|  27 |   2 | GPIO. 2 | 13       | Green Led
|  22 |   3 | GPIO. 3 | 15       | Green Led
|     |     |    3.3v | 17       |
|  10 |  12 |    MOSI | 19       | Red Led
|   9 |  13 |    MISO | 21       | Red Led
|  11 |  14 |    SCLK | 23       | Yellow Led
+-----+-----+---------+------+---+

Using wPI pin ${pin} (gpio27)
EOF

#configure green led as output
gpio mode ${pin} out


#set led, wait and unset it
gpio write ${pin} 1
sleep 1
gpio write ${pin} 0

echo "Done."

