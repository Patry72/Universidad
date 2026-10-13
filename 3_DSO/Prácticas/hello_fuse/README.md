*** CÓMO USAR ***
1º Compilamos haciendo  $: make
   Esto crea un ejecutable hello_fuse y una carpeta llamada hola (vacía).

2º Montamos el sistema haciendo  $: make mount ó $: ./hello_fuse hola
   Esto crea un fichero hello dentro de la carpeta hola. Este será nuestro punto de montaje.

3º Sólo podemos acceder al punto de montaje haciendo  $: cat hola/hello
   Esto nos mostrará el mensaje guardado Hello World!

4º Desmontamos el sistema haciendo  $: make umount ó $: fusermount -u hola
   Esto elimina el fichero hello de la carpeta hola.

También podemos debuguear. Para ello montamos  haciendo  $: ./hello_huse -d hola
ESto nos permite mostrar mensajes con printf(). (?)
