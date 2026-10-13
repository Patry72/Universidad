#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>

#include <linux/timer.h>   // para usar timers

static unsigned long ticks_unsegundo;

// static void timer_handler(unsigned long);

static void timer_handler(struct timer_list *);

// Se define el timer con: nombre, función, tiempo de ejecución y dato
// si el tiempo de ejecución es 0, no se ejecuta inicialmente
// el dato es el que recibe la función como parámetro y se utiliza para poder registrar varios timers que usen
// la misma función y reciban diferentes datos

//DEFINE_TIMER(mytimer /*nombre*/, timer_handler /*funcion*/, 0 /*tiempo*/, 0 /*dato*/);
 
/*
static void timer_handler(unsigned long dato)
{
    printk(KERN_NOTICE "%s : Timer run at %u\n", KBUILD_MODNAME, (unsigned) jiffies);

    mod_timer(&mytimer, jiffies + ticks_unsegundo * 5);  // reprogramar para después de 5 segundos
}
*/

// MEJORA

DEFINE_TIMER(mytimer /*nombre*/, timer_handler /*funcion*/);

static void timer_handler(struct timer_list *ptr)
{
    printk(KERN_NOTICE "%s : Timer run at %u\n", KBUILD_MODNAME, (unsigned) jiffies);

    mod_timer(&mytimer, jiffies + ticks_unsegundo * 5);  // reprogramar para después de 5 segundos
}

//

static int my_init(void)
{
    ticks_unsegundo = msecs_to_jiffies(1000);  // para calcular cuantos ticks son un segundo

    printk(KERN_NOTICE "Hello, loading %s module! ticks para un segundo: %u\n", KBUILD_MODNAME, (unsigned)ticks_unsegundo);

    mod_timer(&mytimer, jiffies + ticks_unsegundo * 5 );  // programar para despues de 5 segundos. mytimer es el timer descriptor
					// No estamos modificando jiffies, estamos consultando su valor y a partir de ahí evaluamos ticks_unsegundo*5

    printk(KERN_NOTICE "%s : loaded at %u\n", KBUILD_MODNAME, (unsigned) jiffies);

    return 0;
}

static void my_exit(void)
{
    del_timer(&mytimer); // eliminamos el timer
    printk(KERN_NOTICE "Bye, exiting %s module!\n", KBUILD_MODNAME);
}

module_init(my_init);
module_exit(my_exit);

MODULE_DESCRIPTION("Timer example");
MODULE_LICENSE("GPL");
