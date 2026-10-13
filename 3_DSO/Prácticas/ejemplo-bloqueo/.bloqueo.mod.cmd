cmd_/home/pi/DSO/Clase/ejemplo-bloqueo/bloqueo.mod := printf '%s\n'   bloqueo.o | awk '!x[$$0]++ { print("/home/pi/DSO/Clase/ejemplo-bloqueo/"$$0) }' > /home/pi/DSO/Clase/ejemplo-bloqueo/bloqueo.mod
