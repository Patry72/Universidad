
/* **********************
 * Ejercicio 1: leds    *
 * Ejercicio 2: speaker *
 * Ejercicio 3: buttons *
 * **********************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>   // misc dev
#include <linux/fs.h>           // file operations
#include <linux/uaccess.h>      // copy to/from user space
#include <linux/wait.h>         // waiting queue
#include <linux/sched.h>        // TASK_INTERRUMPIBLE
#include <linux/delay.h>        // udelay
#include <linux/interrupt.h>
#include <linux/gpio.h>         // para acceso y uso de GPIOs

#define DRIVER_AUTHOR "Patricia Cardenas Fernandez"
#define DRIVER_DESC "Driver para placa auxiliar rpi. Usamos leds, altavoz y botones."

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

#define num_leds 6

static int LED_GPIOS[] = {GPIO_GREEN1, GPIO_GREEN2, GPIO_YELLOW1, GPIO_YELLOW2, GPIO_RED1, GPIO_RED2};
static char *led_desc[] = {"GPIO_GREEN1","GPIO_GREEN2","GPIO_YELLOW1","GPIO_YELLOW2","GPIO_RED1","GPIO_RED2"};
static char *speaker_desc = "BUZZER";
static char *button1_desc = "BUTTON1";
static char *button2_desc = "BUTTON2";

static char buffer_buttons[40];   // Buffer para almacenar el contador de interrupciones
static char *button_dev_desc = "Placa lab. DAC";
static short int irq_BUTTON1 = 0;
static short int irq_BUTTON2 = 1;

DECLARE_WAIT_QUEUE_HEAD(espera);

int bloqueo = 0;

/***************************
 ---- LEDS WRITE / READ ---- 
 ***************************/

static void byte2leds(char ch)
{
    int i;
    int val = (int)ch;

    for (i = 0; i < 6; i++) {
		gpio_set_value(LED_GPIOS[i], (val >> i) & 1);
	}
}

static char leds2byte(void)
{
    int val;
    char ch = 0;
    int i;

    for (i = 0; i < 6; i++) {
        val=gpio_get_value(LED_GPIOS[i]);
        ch = ch | (val << i);
    }
    
    return ch;
}

/*********************************
 ---- READ / WRITE OPERATIONS ----
 *********************************/
 
static ssize_t leds_read (struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	char ch;

    if (*ppos == 0) {
		*ppos+=1;
	} else {
		return 0;
	}

    ch = leds2byte();

    printk( KERN_INFO " (read) valor entregado: %d\n", (int)ch);

    if (copy_to_user(buf, &ch, 1)) {
		return -EFAULT;
	}
	
	return 1;
}

static ssize_t leds_write (struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	/*
	 * EJEMPLOS:
	 * 00 101010 -> enciende / apaga cada led según valor del bit
	 * x1 010101 -> sólo enciende bits a 1, deja tal cual la configuración que haya anterior
	 * 1x 010101 -> sólo apaga bits a 0, deja tal cual la configuración que haya anterior
	 * 11 xxxxxx -> Mensaje de Dennis Nedry y apagamos leds
	 */
	 
	/* Obtenemos byte del usuario */
	char ch;
	char leds_antes = leds2byte();   // Necesitamos saber el valor de los leds actual para los modos 01 y 10
	printk(KERN_INFO " Leds_antes: %d\n", (int)leds_antes);
	char mode;

    if (copy_from_user(&ch, buf, 1)) {
        return -EFAULT;
    }
    
    mode = (ch >> 6);   // Nos quedamos sólo con los bits de modo
    
    printk( KERN_INFO " (write) leds - valor mode: %d\n", mode);
    
    printk( KERN_INFO " (write) leds - valor recibido: %d\n", (int)ch);
    
    /* Actuamos según los 2 msb */
    if ((int)mode == 0) {
		printk(KERN_INFO "%s - Modo 00\n", KBUILD_MODNAME);
		byte2leds(ch);
	} else if ((int)mode == 1) {
		printk(KERN_INFO "%s - Modo 01\n", KBUILD_MODNAME);
		ch = (leds_antes | ch);
		byte2leds(ch);
	} else if ((int)mode == 2) {
		printk(KERN_INFO "%s - Modo 10\n", KBUILD_MODNAME);
		ch = (leds_antes & ch);
		byte2leds(ch);
	} else if ((int)mode == 3) {
		printk(KERN_INFO "%s - Modo 11 ... and ... AH! AH! AH! YOU DIDN'T SAY THE MAGIC WORD!\n", KBUILD_MODNAME);
		
		int i;
		for (i = 0; i < 6; i++) {
			gpio_set_value(LED_GPIOS[i], 0);
		}
	}
    
	return 1;
}

static ssize_t speaker_write (struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char ch;
	
	if (copy_from_user(&ch, buf, 1)) {
        return -EFAULT;
    }
    
    printk( KERN_INFO " (write) speaker - valor recibido: %d\n", (int)ch);
	
	gpio_set_value(GPIO_SPEAKER, 1);
	
	if (ch == '0') {
		gpio_set_value(GPIO_SPEAKER, 0);
		// printk( KERN_INFO "set 0: %d\n", (int)ch);
	} else {
		gpio_set_value(GPIO_SPEAKER, 1);
		// printk( KERN_INFO "set 1: %d\n", (int)ch);
	}
	
	return 1;
}

static ssize_t buttons_read (struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	/*
	 * Manejar el buffer_buttons con cada interrupción (pulsación)
	 * 1. Copiar buffer_buttons a ch
	 * 2. Enviar ch al usuario
	 * 3. Borrar contenido del buffer
	 */
	
	char *ch;

    if (*ppos == 0) *ppos+=1;
    else return 0;

    strcpy(ch, buffer_buttons);

    printk( KERN_INFO " (read) buffer entregado: %s\n", ch);
	
    if (copy_to_user(buf, &ch, sizeof(ch))) {
		return -EFAULT;
	}
	
	// *** BORRAR BUFFER ***
	
	return 1;
}

static irqreturn_t r_irq_handler (int irq, void *dev_id, struct pt_regs *regs)
{
	return IRQ_HANDLED;
}

/*********************************************
 ---- FILE OPERATIONS / DEVICE STRUCTURES ----
 *********************************************/ 

/* LEDS (lectura y escritura) */
static const struct file_operations leds_fops = {
    .owner	= THIS_MODULE,
    .write	= leds_write,
    .read	= leds_read,
};

static struct miscdevice leds_miscdev = {
    .minor	= MISC_DYNAMIC_MINOR,
    .name	= "leds",				// Es el nombre que aparecerá en /dev -> /dev/leds
    .fops	= &leds_fops,         	// Puntero a operaciones
    .mode   = S_IRUGO | S_IWUGO,   	// IRUGO: Read a User, Group, Others / IWUGO: Write a User, Group y Others
};

/* SPEAKER (sólo escritura) */
static const struct file_operations speaker_fops = {
    .owner	= THIS_MODULE,
    .write	= speaker_write,
};

static struct miscdevice speaker_miscdev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "speaker",			// Es el nombre que aparecerá en /dev -> /dev/speaker
	.fops	= &speaker_fops,		
	.mode	= S_IWUGO,				
};

/* BUTTONS (sólo lectura) */
static const struct file_operations buttons_fops = {
	.owner	= THIS_MODULE,
	.read	= buttons_read,
};

static struct miscdevice buttons_miscdev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "buttons",			// Es el nombre que aparecerá en /dev -> /dev/buttons
	.fops	= &buttons_fops,		
	.mode	= S_IRUGO,
};

/***********************************
 ---- INTERRUPTS CONFIGURATIONS ----
 ***********************************/
 
static int r_INT_config (void)
{
	/*
	 * 1. Se pulsan los botones y se guarda en buffer_buttons las pulsaciones de cada uno.
	 * 2. Si leemos, nos muestra buffer_buttons.
	 * 3. Si no hay nada en buffer_buttons, bloquear lectura hasta siguiente pulsación. Despertar cuando haya y mostrar.
	 * 4. Usar TASKLETS para interrupciones y TIMERS (mod_timer) para rebotes (usar parámetro para establecer tiempo timer).
	 * 5. Controlar acceso a buttons una sóla vez. Mostrar error "dispositivo ocupado" si se accede otra vez.
	 * 6 .Para que la llamada (lectura) a buttons sea bloqueante usar cola de espera.
	 */
	 
	int ret = 0;
	
	// Pedimos BUTTON1  *** PROBAR SI SE PUEDE PONER ESTE SNIPPET EN r_GPIO_config ***
	if ((ret = gpio_request(GPIO_BUTTON1, button1_desc))) {   
        printk(KERN_ERR "GPIO request faiure :( button GPIO %d %s\n", GPIO_BUTTON1, button1_desc);   
        return ret;
    }
    
    // Definimos BUTTON1 como interrupción
    if ((irq_BUTTON1 = gpio_to_irq(GPIO_BUTTON1)) < 0) {   // Le asocia un nº de interrupción
        printk(KERN_ERR "GPIO to IRQ mapping faiure :( %s\n", button1_desc);
        return irq_BUTTON1;
    }
    
    // DUDA: set_debounce
    
    printk(KERN_NOTICE "  Mapped int %d for button1 in gpio %d\n", irq_BUTTON1, GPIO_BUTTON1);
    
    // Pedimos BUTTON2  *** PROBAR SI SE PUEDE PONER ESTE SNIPPET EN r_GPIO_config ***
	if ((ret = gpio_request(GPIO_BUTTON2, button2_desc))) {   
        printk(KERN_ERR "GPIO request faiure :( button GPIO %d %s\n", GPIO_BUTTON2, button2_desc);   
        return ret;
    }
    
    // Definimos BUTTON2 como interrupción
    if ((irq_BUTTON2 = gpio_to_irq(GPIO_BUTTON2)) < 0) {   // Le asocia un nº de interrupción
        printk(KERN_ERR "GPIO to IRQ mapping faiure :( %s\n", button2_desc);
        return irq_BUTTON2;
    }
    
    // DUDA: set_debounce
    
    printk(KERN_NOTICE "  Mapped int %d for button2 in gpio %d\n", irq_BUTTON2, GPIO_BUTTON2);
    
    if ((ret = request_irq(irq_BUTTON1, (irq_handler_t ) r_irq_handler, IRQF_TRIGGER_FALLING, button1_desc, button_dev_desc))) {
        printk(KERN_ERR "Irq Request failure :( %s\n", button1_desc);
        return ret;
    }
    
    if ((ret = request_irq(irq_BUTTON2, (irq_handler_t ) r_irq_handler, IRQF_TRIGGER_FALLING, button2_desc, button_dev_desc))) {
        printk(KERN_ERR "Irq Request failure :( %s\n", button2_desc);
        return ret;
    }
	
	return ret;
}

/*****************************
 ---- GPIO CONFIGURATIONS ----
 *****************************/

static int r_GPIO_config (void)
{
    int i;
    int ret = 0;
    
    /* LEDS */
    for(i = 0; i < 6; i++)
    {
		// Pedimos un led en cada iteración
        if ((ret = gpio_request_one(LED_GPIOS[i], GPIOF_INIT_LOW, led_desc[i]))) {
            printk(KERN_ERR "GPIO request failure :( led GPIO %d %s\n",LED_GPIOS[i], led_desc[i]);
            return ret;
        }
        // DUDA A QUITAR
        /*
        // Configuramos cada GPIO como salida
        if ((ret = gpio_direction_output(LED_GPIOS[i],0))) {
			printk(KERN_ERR "GPIO direction failure :( led GPIO %d %s\n", LED_GPIOS[i], led_desc[i]);
			return ret;
		}
		*/
	}
	
	/* SPEAKER */
	if ((ret = gpio_request_one(GPIO_SPEAKER, GPIOF_INIT_LOW, speaker_desc))) {
		printk(KERN_ERR "GPIO request failure :( speaker GPIO %d %s\n", GPIO_SPEAKER, speaker_desc);
        return ret;
    }
    // DUDA A QUITAR
    /*
    if ((ret = gpio_direction_output(GPIO_SPEAKER, 0))) {
		printk(KERN_ERR "GPIO direction failure :( speaker GPIO %d %s\n", GPIO_SPEAKER, speaker_desc);
		return ret;
	}
	*/
	printk(KERN_NOTICE "%s - GPIO_config OK...:D \n", KBUILD_MODNAME);
	
	return ret;
}

/*******************************
 ---- DEVICE CONFIGURATIONS ----
 *******************************/

static int r_dev_config (void)
{
	int ret;
	
	/* LEDS */
    ret = misc_register(&leds_miscdev); 
    
    if (ret < 0) {
        printk(KERN_ERR "leds_misc_register failed :(\n");
    } else {
		printk(KERN_NOTICE "%s - misc_register OK...:D leds_miscdev.minor=%d\n", KBUILD_MODNAME, leds_miscdev.minor);
	}
	
	/* SPEAKER */
	ret = misc_register(&speaker_miscdev);   
	
	if (ret < 0) {
        printk(KERN_ERR "speaker_misc_register failed :(\n");
    } else {
		printk(KERN_NOTICE "%s - misc_register OK...:D speaker_miscdev.minor=%d\n", KBUILD_MODNAME, speaker_miscdev.minor);
	}
	
	/* BUTTONS */
	ret = misc_register(&buttons_miscdev);   
	
	if (ret < 0) {
        printk(KERN_ERR "buttons_misc_register failed :(\n");
    } else {
		printk(KERN_NOTICE "%s - misc_register OK...:D buttoons_miscdev.minor=%d\n", KBUILD_MODNAME, buttons_miscdev.minor);
	}
	
	return ret;
}

/**********************************
 ---- INITIALIZATION / CLEANUP ----
 **********************************/
 
static void r_cleanup (void)
{
	printk(KERN_NOTICE "%s - module cleaning up...\n", KBUILD_MODNAME);
	
	/* Liberamos GPIOs */
	
	/* LEDS */
	int i;
	for (i = 0; i < num_leds; i++) {
		gpio_free(LED_GPIOS[i]);
	}
	
	/* SPEAKER */
	gpio_free(GPIO_SPEAKER);
	 
	/* BUTTONS */
	if (irq_BUTTON1) {
		free_irq(irq_BUTTON1, button_dev_desc);   // Liberamos interrupción
	}
	gpio_free(GPIO_BUTTON1);
	
	if (irq_BUTTON2) {
		free_irq(irq_BUTTON2, button_dev_desc);   // Liberamos interrupción
	}
	gpio_free(GPIO_BUTTON2);
	 
	// Desactivamos timer
	// ...
	
	/* Eliminamos los dispositivos */
	printk(KERN_NOTICE "%s - devices exiting...\n", KBUILD_MODNAME);
	
	/* LEDS */
	if (leds_miscdev.this_device) {
		misc_deregister(&leds_miscdev);
	}
	
	/* SPEAKER */
	if (speaker_miscdev.this_device) {
		misc_deregister(&speaker_miscdev);
	}
	
	/* BUTTONS */
	if (buttons_miscdev.this_device) {
		misc_deregister(&buttons_miscdev);
	}
	
	printk(KERN_NOTICE "Done. Bye from %s module :D!\n", KBUILD_MODNAME);
	
	return;
}
 
static int r_init (void)
{
	int res = 0;
	printk(KERN_NOTICE "Hello, loading %s module :D!\n", KBUILD_MODNAME);
	
	/* Configuramos dispositivo */
	printk(KERN_NOTICE "%s - devices config...\n", KBUILD_MODNAME);
	
	if ((res = r_dev_config())) {   //IMPORTANTE mejor poner doble paréntesis 
		printk(KERN_ERR "%s - devices config failed...:(\n", KBUILD_MODNAME);
		r_cleanup();
		return res;
	}
	printk(KERN_NOTICE "%s - devices config done :D!\n", KBUILD_MODNAME);
	
	/* Configuramos GPIOs */
	printk(KERN_NOTICE "%s - GPIO config...\n", KBUILD_MODNAME);
	
	if ((res = r_GPIO_config())) {
		printk(KERN_ERR "%s - GPIO config failed :(\n", KBUILD_MODNAME);
		r_cleanup();
		return res;
	}
	
	/* Configuramos interrupciones */
	printk(KERN_NOTICE "%s - int config...\n", KBUILD_MODNAME);
	
    if ((res = r_INT_config())) {
		printk(KERN_ERR "%s - Iterrupts config failed :(\n", KBUILD_MODNAME);
		r_cleanup();
		return res;
	}
	
	return res;
}

module_init(r_init);
module_exit(r_cleanup);

/*********************************************
 ---- MODULE LICENSING / DESCRPTION BLOCK ----
 *********************************************/
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
