# Ejemplo de uso cola de bloqueo en Kernel

compilar e instalar con:
```bash
MODULE=bloqueo make
```
Probar con:
```bash
cat /dev/bloqueo
```
Hay que pulsar el boton1 para desbloquear la lectura y después se piede comprobar con```dmesg``` los mensajes de las funciones read y la interrupcción
