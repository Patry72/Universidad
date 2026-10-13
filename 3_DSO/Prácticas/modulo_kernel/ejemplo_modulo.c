/*---------------------------------------------
 * Diseño de Sistemas Operativos (DAC) *
	para compilar el módulo: 
	$ MODULE=ejemplo_modulo make
	para instalar el módulo:
	$ sudo insmod ejemplo_modulo.ko 
	para comprobar si el módulo fue cargado:
	$ sudo lsmod
	$ dmesg | tail
	para desinstalar el módulo:
	$ sudo rmmod ejemplo_modulo
----------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> 
#include <linux/fs.h>         // file operations
#include <linux/uaccess.h>      // copy to/from user space
#include <linux/wait.h>       // waiting queue

// For warning: module: loading out-of-tree module taints kernel.
// MODULE_INFO(intree, "Y");

#define DRIVER_AUTHOR "Diseño de Sistemas Operativos - DAC"
#define DRIVER_DESC   "Ejemplo modulo Kernel para Raspi"

/****************************************************************************/
/* Module init / cleanup block.                                             */
/****************************************************************************/
int r_init(void) {

    printk(KERN_NOTICE "Hello, loading %s module!\n",KBUILD_MODNAME);
    return 0;
}



void r_cleanup(void) {
    printk(KERN_NOTICE "DSO module cleaning up...\n");
    printk(KERN_NOTICE "Done. Bye\n");
    return;
}


module_init(r_init);
module_exit(r_cleanup);


/****************************************************************************/
/* Module licensing/description block.                                      */
/****************************************************************************/

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
