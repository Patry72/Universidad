#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>

#include <linux/interrupt.h>  // para usar tasklets

// static void tasklet_handler(unsigned long);

static void tasklet_handler(struct tasklet_struct *);

// Se define el tasklet con: nombre, función y dato
// el dato es el que recibe la función como parámetro y se utiliza para poder registrar varios tasklets que usen
// la misma función y reciban diferentes datos

// DECLARE_TASKLET(mytasklet /*nombre*/, tasklet_handler /*funcion*/, 0 /*dato*/);

// static void tasklet_handler(unsigned long dato)
// {
    //printk(KERN_NOTICE "%s : Tasklet run at %u\n", KBUILD_MODNAME, (unsigned) jiffies);
// }

// MEJORA
// Cuando mytasklet es planificado se llama a la función tasklet_handler
DECLARE_TASKLET(mytasklet /*nombre*/, tasklet_handler /*funcion*/);

static void tasklet_handler(struct tasklet_struct *ptr)
{
    printk(KERN_NOTICE "%s : Tasklet run at %u\n", KBUILD_MODNAME, (unsigned) jiffies);
}

static int my_init(void)
{

    printk(KERN_NOTICE "Hello, loading %s module!", KBUILD_MODNAME);
    printk(KERN_NOTICE "%s : Tasklet scheduled at %u\n", KBUILD_MODNAME, (unsigned) jiffies);

    tasklet_schedule(&mytasklet);  // se lanza el tasklet en diferido

    return 0;
}

static void my_exit(void)
{
    tasklet_kill(&mytasklet);  // eliminamos el tasklet
    printk(KERN_NOTICE "Bye, exiting %s module!\n", KBUILD_MODNAME);
}

module_init(my_init);
module_exit(my_exit);

MODULE_DESCRIPTION("Timer example");
MODULE_LICENSE("GPL");
