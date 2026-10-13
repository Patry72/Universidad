#include <linux/module.h>
#include <linux/kernel.h>

#define DRIVER_AUTHOR "DAC"
#define DRIVER_DESC   "Ejemplo modulo cargable linux (LKM)"


/****************************************************************************/
/* Module init / cleanup block.                                             */
/****************************************************************************/
static int r_init(void) {

    printk(KERN_NOTICE "Hi, loading %s module!\n",KBUILD_MODNAME);
    
    return 0;
}



static void r_cleanup(void) {
    
    printk(KERN_NOTICE "Done. Bye from %s module\n",KBUILD_MODNAME);

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
