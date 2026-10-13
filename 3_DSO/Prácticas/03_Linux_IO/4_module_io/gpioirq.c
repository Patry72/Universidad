#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h> // misc dev
#include <linux/fs.h>         // file operations
#include <linux/uaccess.h>      // copy to/from user space
#include <linux/wait.h>       // waiting queue
#include <linux/sched.h>      // TASK_INTERRUMPIBLE
#include <linux/delay.h>      // udelay

#include <linux/interrupt.h>
#include <linux/gpio.h>


/**********************************************************************/
/* BerryClip board elements
 * Upper row:	Two buttons placed on the left (s2) & center (s1)
 * 				One buzzer at right side
 * BCM 2835 GPIO numbers corresponding to pins 3, 5 and 4
 */
#define BERRYCLIP_DESCRP  "BerryClip board"
#define GPIO_S_1 2
#define GPIO_S_2 3
#define GPIO_BUZ 4
/* Lower row: 6 leds, defined below from right to left */
/* BCM 2835 GPIO no. corresponding to pins 21, 19, 23, 15, 11 and 13 */
#define GPIO_RED1	 9
#define GPIO_RED2	10
#define GPIO_YLW1	11
#define GPIO_YLW2	17
#define GPIO_GRN1	22
#define GPIO_GRN2	27
/* Array for loop manipulation on BerryClip elements */
struct Element {
	int pin;
	int gpio;
	char *descrp;
};

static const struct Element BerryClip[] = {
	{  3, GPIO_S_1,  "Button_s1"  },
	{  5, GPIO_S_2,  "Button_s2"  },
	{  4, GPIO_BUZ,  "Buzzer"     },
	{ 21, GPIO_RED1, "RedLed1"    },
	{ 19, GPIO_RED2, "RedLed2"    },
	{ 23, GPIO_YLW1, "YellowLed1" },
	{ 15, GPIO_YLW2, "YellowLed2" },
	{ 11, GPIO_GRN1, "GreenLed1"  },
	{ 13, GPIO_GRN2, "GreenLed2"  },
};
/**********************************************************************/


/**********************************************************************/
/* BerryClip LEDs manipulation through linux/pgio API                 */
/* 	Each led represents a bit in a six-digit number:                  */
/* 		least signif. bit == LED_GPIOS[0] == outter red == rightmost  */
/* 		most signif. bit == LED_GPIOS[5] == outter green == leftmost  */
/**********************************************************************/
static void writeLeds(char value)
{
    int i; // leds iterator

    for(i=3; i<=8; i++) {
		 gpio_set_value(BerryClip[i].gpio, (value & 0x1));
		 value >>= 1;
	}
}

static char readLeds(void)
{
    char retval=0;
    int i=8; // leds iterator

	retval += gpio_get_value(BerryClip[i--].gpio)?1:0;
    while(i>=3) {
		retval <<= 1;
		retval += gpio_get_value(BerryClip[i--].gpio)?1:0;
    }
    return retval;
}


/**********************************************************************/
/* IRQ handler - fired on interrupt                                   */
/**********************************************************************/
static irqreturn_t irq_handler_s1(int irq, void *dev_id,
									struct pt_regs *regs)
{
    writeLeds((readLeds() + 1) & 0x3F);		// Do the work.

    return IRQ_HANDLED;	// Notify to kernel
}


/**********************************************************************/
/* static variables                                                   */
/**********************************************************************/
static int initstep=0;
static short int irqButton_s1;


/**********************************************************************/
/* init/cleanup functions                                             */
/**********************************************************************/
static void cleanup(void) {
    int i;
    printk(KERN_NOTICE "%s: starting cleaning up (%d)\n",
						KBUILD_MODNAME, initstep);
    
    switch (initstep) {	// PASS THROUGH ALL STEPS
	tomany:
	case 11:	// All
		printk(KERN_NOTICE "%s: Freeing irq %d\n", KBUILD_MODNAME,
							irqButton_s1);
		free_irq(irqButton_s1, BERRYCLIP_DESCRP); //libera irq
	case 10:
		initstep=9;	
	default:
		if (initstep>11) goto tomany;
		printk(KERN_NOTICE "%s: Freeing gpio:", KBUILD_MODNAME);
		for(i=0; i<initstep; i++) {
			 gpio_free(BerryClip[i].gpio);	// libera GPIO
			 printk(" %d", BerryClip[i].gpio);
		}
		printk("\n");
	}
    printk(KERN_NOTICE "Removing %s module. DONE.\n", KBUILD_MODNAME);
    return;
}

static int init(void) {
    int retval;		// return value in case of error
    int i;			// leds iterator
    
    printk(KERN_NOTICE "Loading %s module.\n", KBUILD_MODNAME);
    
    printk(KERN_NOTICE "%s: configuring gpio:", KBUILD_MODNAME);

	initstep = 0;
	/* Button s2 */
    if ((retval=gpio_request(BerryClip[0].gpio,BerryClip[0].descrp))) {
        printk(KERN_ERR "\tRequest faiure on GPIO %d (%s)\n",
						BerryClip[0].gpio,BerryClip[0].descrp);
        cleanup();
        return retval;
    }
    printk(" %d", BerryClip[0].gpio);
    initstep++;			/* 1 */

	/* Button s1 */
    if ((retval=gpio_request(BerryClip[1].gpio,BerryClip[1].descrp))) {
        printk(KERN_ERR "\tRequest faiure on GPIO %d (%s)\n",
						BerryClip[1].gpio,BerryClip[1].descrp);
        cleanup();
        return retval;
    }
    printk(" %d", BerryClip[1].gpio);
    initstep++;			/* 2 */

	/* Buzzer */
	if ((retval=gpio_request_one(BerryClip[2].gpio, GPIOF_INIT_LOW,
									BerryClip[2].descrp)))
	{
		printk(KERN_ERR "\tRequest faiure on GPIO %d (%s)\n",
						BerryClip[2].gpio, BerryClip[2].descrp);
		cleanup();
		return retval;
	}
	printk(" %d", BerryClip[2].gpio);
	initstep++;			/* 3 */

    for(i=initstep; i<=8; i++)
    {
        if ((retval=gpio_request_one(BerryClip[i].gpio, GPIOF_INIT_LOW,
										BerryClip[i].descrp)))
        {
            printk(KERN_ERR "\tRequest faiure on GPIO %d (%s)\n",
							BerryClip[i].gpio, BerryClip[i].descrp);
            cleanup();
            return retval;
        }
        printk(" %d", BerryClip[i].gpio);
        initstep++;		/* [4..9] */
        gpio_direction_output(BerryClip[i].gpio,0);
	}
    printk( ". DONE\n");
    
	/* Map the button gpio to irq */
    printk(KERN_NOTICE "%s: Mapping gpio %d", KBUILD_MODNAME, GPIO_S_1);
    if ( (retval = gpio_to_irq(BerryClip[0].gpio)) < 0 ) {
        printk(KERN_ERR "\tGPIO to IRQ mapping faiure %s\n",
						BerryClip[0].descrp);
        return retval;
    }
    irqButton_s1 = retval;
    initstep++;			/* 10 */
    printk( " into IRQ %d. DONE.\n", irqButton_s1);

	/* Set the function handler for the irq */
    printk(KERN_NOTICE "%s: Request IRQ...", KBUILD_MODNAME);
    if ((retval = request_irq(irqButton_s1,
					(irq_handler_t)irq_handler_s1, IRQF_TRIGGER_FALLING,
                    BerryClip[0].descrp, BERRYCLIP_DESCRP))) {
        printk(KERN_ERR "Irq %d Request failure\n",irqButton_s1);
        return retval;
    }
    initstep++;			/* 11 */
    printk( "Handled. DONE\n");
        
    printk(KERN_NOTICE "%s: configuration successful\n",KBUILD_MODNAME);
    return 0; // Success
}

module_init(init);
module_exit(cleanup);

/**********************************************************************/
/* Module licensing/description block.                                */
/**********************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arquitectura de Computadores. UMA");
MODULE_DESCRIPTION("BeryClip device driver");
