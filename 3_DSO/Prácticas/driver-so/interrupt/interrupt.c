 #include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h> // misc dev
#include <linux/fs.h>         // file operations
#include <linux/uaccess.h>    // copy to/from user space
#include <linux/wait.h>       // waiting queue
#include <linux/sched.h>      // TASK_INTERRUMPIBLE
#include <linux/delay.h>      // udelay

#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/timer.h>

#define DRIVER_AUTHOR "Andres Rodriguez - DAC"
#define DRIVER_DESC   "Ejemplo interrupción para placa lab. DAC Rpi"

//GPIOS numbers as in BCM RPi

#define GPIO_BUTTON1 2
#define GPIO_BUTTON2 3

#define GPIO_SPEAKER 4

#define GPIO_GREEN1  27
#define GPIO_GREEN2  22
#define GPIO_YELLOW1 17
#define GPIO_YELLOW2 11
#define GPIO_RED1    10
#define GPIO_RED2    9

static int LED_GPIOS[]= {GPIO_GREEN1, GPIO_GREEN2, GPIO_YELLOW1, GPIO_YELLOW2, GPIO_RED1, GPIO_RED2} ;

static char *led_desc[]= {"GPIO_GREEN1","GPIO_GREEN2","GPIO_YELLOW1","GPIO_YELLOW2","GPIO_RED1","GPIO_RED2"} ;

static unsigned long ticks_unsegundo;

static void timer_handler (struct timer_list *timer);   // IMPORTANTE declarar la función antes de definir el timer

DEFINE_TIMER(mytimer, timer_handler);


/****************************************************************************/
/* LEDs write/read using gpio kernel API                                    */
/****************************************************************************/


static void byte2leds(char ch)
{
    int i;
    int val=(int)ch;

    for(i=0; i<6; i++) gpio_set_value(LED_GPIOS[i], (val >> i) & 1);
												// desplaza a la dcha i posiciones y hace AND con  in sólo bit (el lsb)
}


static char leds2byte(void)
{
    int val;
    char ch;
    int i;
    ch=0;

    for(i=0; i<6; i++)
    {
        val=gpio_get_value(LED_GPIOS[i]);
        ch= ch | (val << i);
    }
    return ch;
}

/****************************************************************************/
/* Interrupts variables block                                               */
/****************************************************************************/
static short int irq_BUTTON1    = 0;

// text below will be seen in 'cat /proc/interrupt' command
#define GPIO_BUTTON1_DESC           "Boton 1"

// below is optional, used in more complex code, in our case, this could be
#define GPIO_BUTTON1_DEVICE_DESC    "Placa lab. DAC"

// int ch = 0;   // descomentar y comentar (1) y (2) para probar que tiene "memoria"

/****************************************************************************/
/* IRQ handler - fired on interrupt                                         */
/****************************************************************************/

static void timer_handler (struct timer_list *timer)   
{
	printk(KERN_NOTICE "Se acabó el tiempo! Habilitando IRQs...\n");
	
	enable_irq(irq_BUTTON1);
}

static irqreturn_t r_irq_handler1(int irq, void *dev_id, struct pt_regs *regs) {

	disable_irq_nosync(irq_BUTTON1);  

	ticks_unsegundo = msecs_to_jiffies(1000);  // Calculamos cuántos tick son 1 segundo 
	
	mod_timer(&mytimer, jiffies + ticks_unsegundo * 1 );  // Programamos para despues de 500 microsegundos
	
    int ch;   // (1)
    // we will increment value in leds with button push
    // due to switch bouncing this hadler will be fired few times for every putton push
    
    ch=leds2byte();   // (2)
    ch++;
    if (ch>63) ch=0;
    byte2leds(ch);

    return IRQ_HANDLED;
}



/*****************************************************************************/
/* This functions requests GPIOs and configures interrupts */
/*****************************************************************************/

/*******************************
 *  request and init gpios for leds
 *******************************/

static int r_GPIO_config(void)
{
    int i;
    int res=0;
    for(i=0; i<6; i++)
    {
        if ((res=gpio_request_one(LED_GPIOS[i], GPIOF_INIT_LOW, led_desc[i])))   // Pedimos un gpio y que lo inicialice apagado
        {
            printk(KERN_ERR "GPIO request faiure: led GPIO %d %s\n",LED_GPIOS[i], led_desc[i]);   // Si devuelve algun valor falla
            return res;
        }
        gpio_direction_output(LED_GPIOS[i],0);   // Devuelve 0 si fue correcto. 
												 // El 0 indica el valor de inicio, para evitar glitches
	}
	
	return res;
}

/*******************************
 *  set interrup for button 1
 *******************************/

static int r_int_config(void)
{
	int res=0;
	
	// 1º pedimos GPIO
    if ((res=gpio_request(GPIO_BUTTON1, GPIO_BUTTON1_DESC))) {   
        printk(KERN_ERR "GPIO request faiure: %s\n", GPIO_BUTTON1_DESC);   
        return res;
    }

	// 2º definimos como interrupcion
    if ( (irq_BUTTON1 = gpio_to_irq(GPIO_BUTTON1)) < 0 ) {
        printk(KERN_ERR "GPIO to IRQ mapping faiure %s\n", GPIO_BUTTON1_DESC);
        return irq_BUTTON1;
    }

    printk(KERN_NOTICE "  Mapped int %d for button1 in gpio %d\n", irq_BUTTON1, GPIO_BUTTON1);

    if ((res=request_irq(irq_BUTTON1,
                    (irq_handler_t ) r_irq_handler1,  // Si un mismo evento queremos que esté asociado a más de un
													  // handler tenemos que poner IRQF_SHARED
                    IRQF_TRIGGER_FALLING,
                    GPIO_BUTTON1_DESC,
                    GPIO_BUTTON1_DEVICE_DESC))) {
        printk(KERN_ERR "Irq Request failure\n");
        return res;
    }


    return res;
}



/****************************************************************************/
/* Module init / cleanup block.                                             */
/****************************************************************************/


static void r_cleanup(void) {
    int i;
    printk(KERN_NOTICE "%s module cleaning up...\n", KBUILD_MODNAME);
    for(i=0; i<6; i++)
         gpio_free(LED_GPIOS[i]);	// libera GPIOS, los devuelve al sistema
    if(irq_BUTTON1) free_irq(irq_BUTTON1, GPIO_BUTTON1_DEVICE_DESC);   //libera irq
    gpio_free(GPIO_BUTTON1);  // libera GPIO
    printk(KERN_NOTICE "Done. Bye from %s module\n", KBUILD_MODNAME);
    
    del_timer(&mytimer);
    
    return;
}


static int r_init(void) {
    int res=0;
    printk(KERN_NOTICE "Hello, loading %s module!\n", KBUILD_MODNAME);
    
    printk(KERN_NOTICE "%s - GPIO config...\n", KBUILD_MODNAME);
    if((res = r_GPIO_config()))   // Si res da algo != 0 no se instala el mod al hacer insmod (...)
    {
		r_cleanup();
		return res;
	}
    
    printk(KERN_NOTICE "%s - int config...\n", KBUILD_MODNAME);
    if((res = r_int_config()))
    {
		r_cleanup();
		return res;
	}
    
    return res;
}

module_init(r_init);
module_exit(r_cleanup);

/****************************************************************************/
/* Module licensing/description block.                                      */
/****************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
