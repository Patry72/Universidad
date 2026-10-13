cmd_/home/pi/DSO/Clase/miDriverSO/driverSO_v4.mod := printf '%s\n'   driverSO_v4.o | awk '!x[$$0]++ { print("/home/pi/DSO/Clase/miDriverSO/"$$0) }' > /home/pi/DSO/Clase/miDriverSO/driverSO_v4.mod
