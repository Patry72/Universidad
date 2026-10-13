
**** DESCRIPCIÓN DEL PROYECTO ****

Montaje de un sistema de ficheros en el espacio del usuario (FUSE), en el que se almacenarán una serie de proverbios.
En este directorio se encuentra el archivo "basicFUSE.c", el cual contiene la implementación de las funciones que podemos realizar en nuestro sistema de ficheros. 

El funcionamiento es el siguiente:
Al compilar el código "basicFUSE.c" se genera un ejecutable del sistema de ficheros y una carpeta, la cual será el directorio raíz donde se montará nuestro FUSE, con sus respectivas carpetas y ficheros. Una vez montado podemos realizar operaciones como lectura de directorio o fichero, búsqueda de una cadena de caracteres en ellos..., operaciones de lectura básicas. Además de estos ficheros, también se encuentra un fichero cabecera "basicFUSE.h" con la estructura principal de datos de nuestro sistema de ficheros y un código de funciones auxiliares "basicFUSE_lib.c".

**** MONTAJE Y DESMONTAJE ***

En el mismo directorio se encuentra un fichero "Makefile" para compilar, montar y desmontar el FUSE.
- Para compilar:
 $ make

- Para montar:
 $ make mount

- Para desmontar:
 $ make umount

- Para eliminar ficheros .o:
 $ make clean

- También se puede hacer un debug del comportamiento (en otro terminal). Este comando también monta el sistema:
 $ make debug

- O sólo hacer un seguimiento de los printf que se encuentran en las distintas funciones a modo de depurar (en otro terminal). Este comando también monta el sistema:
 $ make printf

*** EJEMPLOS DE USO ***

- Mostrar contenido del directorio raíz del punto de montaje:
 $ ls punto_montaje/   -> Se mostrarán todos los capítulos en número romanos y los subdirectorios little y BIG.

- Mostrar atributos de todo el contenido raíz del punto de montaje:
 $ ls -la punto_montaje/   -> Se mostrará lo anterior y los subdirectorios . y .., junto a sus atributos (tamaño, fecha de modificación, permisos...).

- Mostrar contenido de un fichero:
 $ cat punto_montaje/XL   -> Mostrará por la salida estándar el proverbio correspondiente al capítulo XL.

- Buscar una palabra entre todos los ficheros:
 $ grep caminante /punto_montaje/*   -> Mostrará en qué archivo/s se encuentra la cadena "caminante".

- Mostrar información sobre un fichero o directorio:
 $ stat punto_montaje/BIG   -> Mostrará la información del fichero o directorio (enlaces, nº de inodo, fechas, pid, uid...).

*** QUIÉN HA REALIZADO ESTE PROYECTO ***

Patricia Cárdenas Fernández

