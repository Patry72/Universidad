#!/usr/bin/env bash

export LANG=C

pin=27

# Mostrar la numeracion de gpio del chip (BCM) y de la placa (wPI)

cat <<EOF
Shell script using /sys/class/gpio/ 
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

Using gpio${pin}
EOF


# create gpio27 dir and the corresponding direction and value access files
#  in the gpio dir (/sys/...)
echo -n "${pin}" > /sys/class/gpio/unexport
echo -n "${pin}" > /sys/class/gpio/export

# now wait until directory (gpio27) and files (direction and value) are created
sleep 0.1

#configure green led as output
echo -n "out" > /sys/class/gpio/gpio${pin}/direction

#set led, wait and unset it
echo 1 > /sys/class/gpio/gpio${pin}/value
sleep 1
echo 0 > /sys/class/gpio/gpio${pin}/value

echo "Done."

