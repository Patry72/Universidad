
**** DESCRIPCIÓN DEL PROYECTO ****

Driver para usar placa auxiliar DAC para Raspberry Pi.
En este directorio se encuentra el archivo "driverSO.c" con el que poder probar los seis leds, el altavoz y los dos pulsadores. Al instalar este módulo se crearán los nodos /leds, /speaker y /buttons en el directorio /dev. Además de estos ficheros, también se encuentran dos scripts para facilitar la comunicación con los leds:

- "bin2char.sh": envía al dispositivo el número binario enviado como parámetro.
- "dev2bin.sh": lee el estado de los leds (0 apagado y 1 encendido) del dispositivo enviado como parámetro.

Cabe destacar un par de características de este módulo. Son el uso de una variable "t_param" para la configuración de un timer que se lo pasamos como parámetro al instalarlo, y una función de apertura única del fichero /buttons, de manera que se bloquee un proceso que quiere leer si ya había otro esperando en la lectura.

El funcionamiento del driver es el siguiente:

* LEDS: según los dos bit más significativos que enviemos al nodo /leds actuarán de la siguiente manera:
 - 00xxxxxx: Se encenderán los leds cuyo bit esté a 1, y se apagará los leds cuyo bit esté a 0.
 - 01xxxxxx: Únicamente se encenderán (o quedarán encendidos) los leds cuyo bit esté a 1.
 - 10xxxxxx: Únicamente se apagarán (o quedarán apagados) los leds cuyo bit esté a 0.
 - 11xxxxxx: Se apagarán los seis leds y mostrará en el kernel el mensaje de Denis Nedry de Jurassic Park.

* SPEAKER: hará sonar la música que enviemos al nodo /speaker. Como ejemplos se encuentran "prueba_speaker2.c" y "sonidoTetris" en el directorio.

* BUTTONS: según los pulsadores que se accionen se irán añadiendo dichas pulsaciones a un buffer que será mostrado al usuario al leer del nodo /buttons.
  Si el buffer se encuentra vacío al leer, bloquear la lectura hasta próxima pulsación, que mostrará automáticamente al usuario su contenido.

¡¡ IMPORTANTE !!
Hay líneas de código (las que usan jiffies para 32 bits y get_jiffies_64() para 64 bits) que afectan según la arquitectura de rpi. Decomentar aquellas que que coincidan con la arquitectura.


**** COMPILACIÓN, INSTALACIÓN Y DESINSTALACIÓN ****

En el mismo directorio se encuentra un fichero "Makefile" para compilar, instalar y desinstalar el módulo. Si queremos instalar el módulo sin el parámetro t_param, hacemos lo siguiente:

- Para compilar e instalar:
 $ make MODULE=driverSO

- Para desinstalar:
 $ make uninstall MODULE=driverSO
 
- Para comprobar si se ha instalado el módulo:
 $ lsmod

- También se puede hacer un seguimiento continuado de los mensajes enviados al kernel:
 $ dmesg -wH 

 
Si queremos instalar el módulo asignando un valor al parámetro t_param, hacemos lo siguiente:

- Para compilar:
 $ make compile MODULE=driverSO
 
- Para instalar:
 $ sudo insmod driverSO.ko t_param=valor   ->   donde valor es la cantidad de segundos para el timer

- Para desinstalar:
 $ make uninstall MODULE=driverSO
 
- Para comprobar si se ha instalado el módulo:
 $ lsmod

- También se puede hacer un seguimiento continuado de los mensajes enviados al kernel:
 $ dmesg -wH


**** EJEMPLOS DE USO ****

* LEDS:
 $ ./bin2char 01110011 > /dev/leds
 $ ./dev2bin /dev/leds

* SPEAKER:
 $ ./sonidoTetris > /dev/speaker    ->    Debemos haber compilado el fichero ejecutando:  
								          $ gcc sonidoTetris.c -o sonidoTetris

* BUTTONS:
 (accionamos pulsadores varias veces)
 $ cat /dev/buttons
 
 Si un proceso está bloqueado esperando lectura, podemos ejecutar otro proceso en otra terminal para demostrar la apertura única:
 $ cat /dev/buttons
 
 Esto mostrará en el segundo proceso el mensaje "Device or resource busy".


**** QUIÉN HA REALIZADO ESTE PROYECTO ****

Patricia Cárdenas Fernández (patry72@uma.es)


**** FALLOS SIN RESOLVER ****

- Control de rebotes de pulsadores usando las funciones irq_disable e irq_enable.
  SOLUCIÓN OPTADA: usar variable global para permitir o denegar la interrupción de cada pulsador.
