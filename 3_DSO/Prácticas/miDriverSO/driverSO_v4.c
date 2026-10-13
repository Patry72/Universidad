
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
#include <linux/sched.h>        // TASK_INTERRUMPIBLE y uso de current
#include <linux/interrupt.h>	// para interrupciones
#include <linux/gpio.h>         // para acceso y uso de GPIOs
#include <linux/timer.h>		// para usar timers
#include <linux/semaphore.h>    // para semáforos
#include <linux/spinlock.h>		// para spinlocks
#include <linux/moduleparam.h>	// para enviar parámetros al módulo
#include <linux/jiffies.h>		// para leer jiffies

/********************************
 ---- DEFINITIONS / VARIABLES----
 ********************************/

#define DRIVER_AUTHOR "Patricia Cardenas Fernandez"
#define DRIVER_DESC "Driver para uso de placa auxiliar de Paspberry PI"

/* GPIOS numbers as in BCM RPi */

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
static char *button_dev_desc = "Placa lab. DAC";

static short int irq_BUTTON1 = 0;
static short int irq_BUTTON2 = 0;
static char buffer_buttons[100] = "";   	// Buffer para almacenar el contador de interrupciones
static short int bloqueo = 1;   			// Condición de bloqueo para WAIT QUEUE (lectura)
static int pos_caracter = 0;

static unsigned long ticks_unsegundo;
// static unsigned long long ticks_unsegundo;   // *** para 64 bits ***
static unsigned long t_param = 5;   		// Temporizador por defecto para rebotes
// static unsigned long long t_param = 5;   	// *** para 64 bits ***
static unsigned long ticks_param = 0;		// Ticks para el parámetro t_param
// static unsigned long long ticks_param = 0;	// *** para 64 bits ***

static int f_busy = 0;   					// Controla si el fichero está ocupado (abierto) o no al hacer cat /dev/buttons
static int owner;							// pid del proceso poseedor del fichero

/* IMPORTANTE declarar las funciones antes de definir el timer */
static void timer_handler1 (struct timer_list *);  
static void timer_handler2 (struct timer_list *);  

static void tasklet_handler1(struct tasklet_struct *);
static void tasklet_handler2(struct tasklet_struct *);

DECLARE_WAIT_QUEUE_HEAD(espera);
DEFINE_TIMER(mytimer1, timer_handler1);		
DEFINE_TIMER(mytimer2, timer_handler2);
DECLARE_TASKLET(mytasklet1, tasklet_handler1);
DECLARE_TASKLET(mytasklet2, tasklet_handler2);
DEFINE_SEMAPHORE(sem_busy);   				// Semáforo para variable f_busy
DEFINE_SPINLOCK(bloqueo_lock);				// Spinlock para variable bloqueo
// spinlock_t bloqueo_lock = SPIN_LOCK_UNLOCKED;	// DESFASADO
module_param(t_param, ulong, 0);   

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
 
static ssize_t leds_read (struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	char ch;

    if (*offset == 0) {
		*offset+=1;
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

static ssize_t leds_write (struct file *file, const char __user *buf, size_t count, loff_t *offset)
{ 
	/* Obtenemos byte del usuario */
	char ch;
	char mode;
	char leds_antes = leds2byte();   // Necesitamos saber el valor de los leds actual para los modos 01 y 10
	printk(KERN_INFO " Leds_antes: %d\n", (int)leds_antes);
	
	// char *mensaje;
	// strcpy(mensaje, "AH! AH! AH! YOU DIDN'T SAY THE MAGIC WORD!");
	// int len = strlen(mensaje);
	// char buffer[len];

    if (copy_from_user(&ch, buf, 1)) {
        return -EFAULT;
    }
    
    /* Obtenemos sólo bits de modo */
    mode = (ch >> 6);   
    
    printk( KERN_INFO " (write) leds - valor mode: %d", mode);
    printk( KERN_INFO " (write) leds - valor recibido: %d", (int)ch);
    
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
		
		/*
		if (copy_to_user(buffer, &mensaje, sizeof(mensaje))) {
			return -EFAULT;
		}
		*/
		
		for (int i = 0; i < 6; i++) {
			gpio_set_value(LED_GPIOS[i], 0);
	    }
	}
    
	return 1;
}

static ssize_t speaker_write (struct file *file, const char __user *buf, size_t count, loff_t *offset)   // *** DUDA FUNCIONAMIENTO ***
{
	char ch;
	
	if (copy_from_user(&ch, buf, 1)) {
        return -EFAULT;
    }
    
    printk( KERN_INFO " (write) speaker - valor recibido: %d\n", (int)ch);
	
	gpio_set_value(GPIO_SPEAKER, 1);
	
	if (ch == '0') {
		gpio_set_value(GPIO_SPEAKER, 0);
	} else {
		gpio_set_value(GPIO_SPEAKER, 1);
	}
	
	return 1;
}	

static ssize_t buttons_read (struct file *file, char __user *buf, size_t count, loff_t *offset)   // *** DUDA NO COPIA BUFFER ENTERO ***
{
	/*
	 * Control del spinlock: lock al consultar variable y unlock para liberar
	 * 1. Bloquear (wait event) proceso si no hay datos en el buffer.
	 * 2. Copiar buffer_buttons a ch.
	 * 3. Enviar ch al usuario.
	 * 4. Borrar contenido del buffer.
	 * 5. Mostrar "Dispositivo ocupado" si se accede una segunda vez y no está listo -> explicación en CV 
	 */
	
	char buffer_user[100];   
	int len;

    if (*offset == 0) {
		*offset+=1;   
    } else {
		return 0;
	}
    
    /* Capturamos lock */
    spin_lock(&bloqueo_lock);
    
    while (bloqueo) {
		printk(KERN_INFO " (read) comienza bloqueo\n");
		
		/* Liberamos lock */
		spin_unlock(&bloqueo_lock);
		
		/* Como estamos bloqueados se añade proceso a cola de espera */
		if (wait_event_interruptible(espera, !bloqueo)) {
			return -ERESTARTSYS;
		}
		
		/* Capturamos lock */
		spin_lock(&bloqueo_lock);
		
		printk(KERN_INFO " (read) acaba bloqueo\n");
	}
	
	bloqueo = 1;
	
	/* Liberamos lock */
	spin_unlock(&bloqueo_lock);
	
    strcpy(buffer_user, buffer_buttons);
    len = strlen(buffer_user);
    
    printk(KERN_INFO "buffer_buttons: %s", buffer_buttons);
	printk(KERN_INFO "buffer_user: %s", buffer_user);
	
    printk(KERN_INFO " (read) buffer entregado: %s con longitud: %d", buffer_user, len);
	
    if (copy_to_user(buf, buffer_user, len)) {   
		return -EFAULT;
	}
	
	/* Volvemos a inicializar buffer */
	for (int i = 0; i < len; i++) {
		buffer_buttons[i] = '\0';
	}
	
	pos_caracter = 0;
	
	return len;
}

static int buttons_open (struct inode *inode, struct file* filp)   // *** DUDA *** //
{
	/*
	 * Cuando se hace cat -> owner   Cuando otro proceso (terminal) hace cat mientras espera owner -> ocupado
	 * 1. Consultamos variable f_busy.
	 * 2. Si es la 1ª vez abierto, se pone ocuapdo. Si no, "Device or ressource busy".
	 */
	int actual = current->pid;   // pid del proceso actual
	printk(KERN_INFO "Open  actual pid: %d", actual);
	 
	if (down_interruptible(&sem_busy)) {
		return -ERESTARTSYS;
	}
	
	if (f_busy == 0) {
		f_busy = 1;
		up(&sem_busy);
		//owner = (long*)current;
		owner = current->pid;   // pid del proceso actual
		printk(KERN_INFO "Open - owner pid: %d", owner);
	} else {
		up(&sem_busy);
	    return -EBUSY;	
	}
	
	/*
	int actual = get_current()->pid;
	
	printk(KERN_INFO "Fichero abierto por proceso con pid: %d", owner);
	
	if (owner != actual) {
		printk(KERN_INFO "Ocupado por proceso con pid: %d", owner);
		return -EBUSY;
	}
	*/
	return 0;
}

static int buttons_release (struct inode *inode, struct file* filp)   // *** DUDA *** //
{
	// if (owner == (long*)current) f_busy=0;
	
	if (owner == current->pid) {
		f_busy = 0;
		printk(KERN_INFO "Release - owner pid: %d", owner);
	}
	/*
	printk(KERN_INFO "Fichero cerrado por proceso con pid: %d", owner);
	
	owner = get_current()->pid;   // Pasamos el dueño al proceso actual
*/
	return 0;
}

/****************************************
 ---- TIMER / IRQ / TASKLET HANDLERS ----
 ****************************************/
 
static void timer_handler1(struct timer_list *timer)
{
	/*
	 * Cuando se llame a este manejador, sólo tendremos que habilitar las 
	 * dos interrupciones de los botones.
	 * Lo llamamos cuando expira el mod_timer.
	 */
	printk(KERN_NOTICE "%lld Game over!  :o\n",get_jiffies_64());
	
	enable_irq(irq_BUTTON1);
}
static void timer_handler2(struct timer_list *timer)
{
	/*
	 * Cuando se llame a este manejador, sólo tendremos que habilitar las 
	 * dos interrupciones de los botones.
	 * Lo llamamos cuando expira el mod_timer.
	 */
	printk(KERN_NOTICE "%lld Game over!  :o\n",get_jiffies_64());
	
	enable_irq(irq_BUTTON2);
} 
/* BOTTOM HALF PARA BUTTON 1 */
static void tasklet_handler1 (struct tasklet_struct *task)   
{
	/*
	 * Usamos spinlocks porque este códido no se puede dormir
	 * Control del semáforo: lock al consultar variable y unlock para liberar
	 * 1. Escribimos 1 o 2 en el buffer, según la interrupción que se haya dado
	 * 2. Actualizamos condición de bloqueo
	 * 3. Cuando hay un carácter hacemos wake_up para despertar lectura
	 */
	 
	printk(KERN_NOTICE "Tasklet run at %llu\n", get_jiffies_64());
	
	spin_lock(&bloqueo_lock);
	
	buffer_buttons[pos_caracter++] = '1';
	bloqueo = 0;
	
	spin_unlock(&bloqueo_lock); 
	
	if (pos_caracter > 0) {
		wake_up(&espera);
	}
}

/* BOTTOM HALF PARA BUTTON 2 */
static void tasklet_handler2 (struct tasklet_struct *task)  
{
	/*
	 * Control del semáforo: lock al consultar variable y unlock para liberar
	 * 1. Escribimos 1 o 2 en el buffer, según la interrupción que se haya dado
	 * 2. Actualizamos condición de bloqueo
	 * 3. Hacemos wake_up para despertar lectura
	 */
	 
	printk(KERN_NOTICE "Tasklet run at %llu\n", get_jiffies_64());
	
	spin_lock(&bloqueo_lock);

	buffer_buttons[pos_caracter++] = '2';
	bloqueo = 0;
	
	spin_unlock(&bloqueo_lock); 
	
	if (pos_caracter > 0) {
		wake_up(&espera);
	}
}

	      /*****************************************************/
	/***************************************************************/
   /* DUDA : ¿Funcionaría mod_timer para unsigned long long (64)? */
  /***************************************************************/ 
	   /*****************************************************/ 

/* TOP HALF */
static irqreturn_t irq_handler (int irq, void *dev_id)    
{
	/*
	 * 1. Deshabilitamos interrucpiones de los botones
	 * 2. Activamos timer para controlar los rebotes
	 * 3. Planificamos para modificar buffer
	 */ 
	
	if (irq == irq_BUTTON1) {   // Añadimos '1' al buffer       
		disable_irq_nosync(irq_BUTTON1);
		
		printk(KERN_INFO "Interrupción por %s at %lu (32)", button1_desc, jiffies);
		// printk(KERN_INFO "Interrupción por %s at %llu (64)", button1_desc, get_jiffies_64());
		
		mod_timer(&mytimer1, jiffies + ticks_param);
		//mod_timer(&mytimer1, get_jiffies_64() + ticks_param);
		
		printk(KERN_INFO "Timer will expire after %lu (32)\n", jiffies + ticks_param);
		// printk(KERN_INFO "Timer will expire after %llu (64)\n", get_jiffies_64() + ticks_param);
		
		tasklet_schedule(&mytasklet1);
	} else if (irq == irq_BUTTON2) {   // Añadimos '2' al buffer     
		disable_irq_nosync(irq_BUTTON2);
		
		printk(KERN_INFO "Interrupción por %s at %lu (32)", button2_desc, jiffies);
		// printk(KERN_INFO "Interrupción por %s at %llu (64)", button2_desc, get_jiffies_64());
		
		mod_timer(&mytimer2, jiffies + ticks_param);
		// mod_timer(&mytimer2, get_jiffies_64() + ticks_param);
		
		printk(KERN_INFO "Timer will expire after %lu (32)\n", jiffies + ticks_param);
		// printk(KERN_INFO "Timer will expire after %llu\n", get_jiffies_64() + ticks_param);
		
		tasklet_schedule(&mytasklet2);
	}
	
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
	.mode	= S_IRUGO | S_IWUGO,				
};

/* BUTTONS (sólo lectura) */
static const struct file_operations buttons_fops = {
	.owner		= THIS_MODULE,
	.read		= buttons_read,
	.open 		= buttons_open,
	.release	= buttons_release,
};

static struct miscdevice buttons_miscdev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "buttons",			// Es el nombre que aparecerá en /dev -> /dev/buttons
	.fops	= &buttons_fops,		
	.mode	= S_IRUGO | S_IWUGO,
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
	 * 5. Controlar acceso a buttons una sóla vez. Mostrar error "dispositivo ocupado" si se accede otra vez -> en CV *** DUDA ***
	 * 6. Para que la llamada (lectura) a buttons sea bloqueante usar cola de espera.
	 */
	 
	int ret = 0;
	
	// Pedimos BUTTON1
	if ((ret = gpio_request(GPIO_BUTTON1, button1_desc))) {   
        printk(KERN_ERR "GPIO request faiure... :[ button GPIO %d %s\n", GPIO_BUTTON1, button1_desc);   
        return ret;
    }
    
    // Definimos BUTTON1 como interrupción
    if ((irq_BUTTON1 = gpio_to_irq(GPIO_BUTTON1)) < 0) {   // Le asocia un nº de interrupción
        printk(KERN_ERR "GPIO to IRQ mapping faiure... :[ %s\n", button1_desc);
        return irq_BUTTON1;
    }
    
    // DUDA: set_debounce
    
    printk(KERN_NOTICE "  Mapped INT %d for %s in gpio %d\n", irq_BUTTON1, button1_desc, GPIO_BUTTON1);
    
    // Pedimos BUTTON2
	if ((ret = gpio_request(GPIO_BUTTON2, button2_desc))) {   
        printk(KERN_ERR "GPIO request faiure... :[ button GPIO %d %s\n", GPIO_BUTTON2, button2_desc);   
        return ret;
    }
    
    // Definimos BUTTON2 como interrupción
    if ((irq_BUTTON2 = gpio_to_irq(GPIO_BUTTON2)) < 0) {   // Le asocia un nº de interrupción
        printk(KERN_ERR "GPIO to IRQ mapping faiure... :[ %s\n", button2_desc);
        return irq_BUTTON2;
    }
    
    // DUDA: set_debounce
    
    printk(KERN_NOTICE "  Mapped INT %d for %s in gpio %d\n", irq_BUTTON2, button2_desc, GPIO_BUTTON2);
    
    /* Registramos IRQs */
    if ((request_irq(irq_BUTTON1, irq_handler, IRQF_TRIGGER_FALLING, button1_desc, button_dev_desc))) {
        printk(KERN_ERR "Irq Request failure... :[ %s\n", button1_desc);
        return -EIO;
    }
    
    if ((request_irq(irq_BUTTON2, irq_handler, IRQF_TRIGGER_FALLING, button2_desc, button_dev_desc))) {
        printk(KERN_ERR "Irq Request failure... :[ %s\n", button2_desc);
        return -EIO;
    }
	
	return ret;
}

/*****************************
 ---- GPIO CONFIGURATIONS ----
 *****************************/

static int r_GPIO_config (void)   // *** DUDA ***
{
    int i;
    int ret = 0;
    
    /* LEDS */
    for(i = 0; i < 6; i++)
    {
		// Pedimos un led en cada iteración
        if ((ret = gpio_request_one(LED_GPIOS[i], GPIOF_INIT_LOW, led_desc[i]))) {
            printk(KERN_ERR "GPIO request failure... :[ led GPIO %d %s\n",LED_GPIOS[i], led_desc[i]);
            return ret;
        }
        
        printk(KERN_INFO "  Configured %s in gpio %d\n", led_desc[i], LED_GPIOS[i]);
        
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
		printk(KERN_ERR "GPIO request failure... :[ speaker GPIO %d %s\n", GPIO_SPEAKER, speaker_desc);
        return ret;
    }
    
    printk(KERN_INFO "  Configured %s in gpio %d\n", speaker_desc, GPIO_SPEAKER);
    
    // DUDA A QUITAR
    /*
    if ((ret = gpio_direction_output(GPIO_SPEAKER, 0))) {
		printk(KERN_ERR "GPIO direction failure :( speaker GPIO %d %s\n", GPIO_SPEAKER, speaker_desc);
		return ret;
	}
	*/
	
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
        printk(KERN_ERR "leds_misc_register failed... :[\n");
    } else {
		printk(KERN_NOTICE "misc_register OK... :D leds_miscdev.minor=%d\n", leds_miscdev.minor);
	}
	
	/* SPEAKER */
	ret = misc_register(&speaker_miscdev);   
	
	if (ret < 0) {
        printk(KERN_ERR "speaker_misc_register failed... :[\n");
    } else {
		printk(KERN_NOTICE "misc_register OK... :D speaker_miscdev.minor=%d\n", speaker_miscdev.minor);
	}
	
	/* BUTTONS */
	ret = misc_register(&buttons_miscdev);   
	
	if (ret < 0) {
        printk(KERN_ERR "buttons_misc_register failed... :[\n");
    } else {
		printk(KERN_NOTICE "misc_register OK... :D buttons_miscdev.minor=%d\n", buttons_miscdev.minor);
	}
	
	return ret;
}

/**********************************
 ---- INITIALIZATION / CLEANUP ----
 **********************************/
 
static void r_cleanup (void)
{
	/* Liberamos GPIOs */
	
	printk(KERN_NOTICE "Module cleaning up...");
	
	/* LEDS */
	for (int i = 0; i < num_leds; i++) {
		gpio_free(LED_GPIOS[i]);
	}
	
	/* SPEAKER */
	gpio_free(GPIO_SPEAKER);
	 
	/* BUTTONS & IRQs*/
	if (irq_BUTTON1) {
		free_irq(irq_BUTTON1, button_dev_desc);  
	}
	
	gpio_free(GPIO_BUTTON1);
	
	if (irq_BUTTON2) {
		free_irq(irq_BUTTON2, button_dev_desc);   
	}
	
	gpio_free(GPIO_BUTTON2);
	
	/* Eliminamos TIMERS */
	del_timer(&mytimer1);
	del_timer(&mytimer2);
	
	/* Eliminamos TASKLETS */
	tasklet_kill(&mytasklet1);
	tasklet_kill(&mytasklet2);
	
	/* Eliminamos los dispositivos de /dev */
	
	printk(KERN_NOTICE "Devices exiting...\n");
	
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
	
	printk(KERN_NOTICE "*************************************");
	printk(KERN_NOTICE "Done. Bye from %s module! :D\n", KBUILD_MODNAME);
	printk(KERN_NOTICE "*************************************");
	
	return;
}
 
static int r_init (void)
{
	int res = 0;
	
	owner = get_current()->pid;
	printk(KERN_INFO "Init Owner pid %d\n", owner);
	
	printk(KERN_NOTICE "*************************************");
	printk(KERN_NOTICE "Hello, loading %s module! :D\n", KBUILD_MODNAME);
	printk(KERN_NOTICE "*************************************");
	
	printk(KERN_INFO "Seconds configured to timer: %lu", t_param);
	
	printk(KERN_INFO "len: %d", strlen("hola"));
	
	/* Calculamos temporizador de rebotes */
	ticks_unsegundo = msecs_to_jiffies(1000);   // Cuántos jiffies son un segundo
	ticks_param = t_param * ticks_unsegundo;  	// Cuántos ticks son t_param segundos para las irqs
	
	printk(KERN_NOTICE "1 segundo en ticks: %lu\n", ticks_unsegundo);
	printk(KERN_NOTICE "Size of ticks_unsegundo: %d\n", sizeof(ticks_unsegundo));
	
	/* Configuramos dispositivo */
	printk(KERN_NOTICE "Devices config...\n");
	
	if ((res = r_dev_config())) {   //IMPORTANTE mejor poner doble paréntesis 
		printk(KERN_ERR "Devices config failed... :(\n");
		r_cleanup();
		return res;
	}
	printk(KERN_NOTICE "Devices config done! :D\n");
	
	/* Configuramos GPIOs */
	printk(KERN_NOTICE "GPIO config...\n");
	
	if ((res = r_GPIO_config())) {
		printk(KERN_ERR "GPIO config failed... :[\n");
		r_cleanup();
		return res;
	}
	printk(KERN_NOTICE "GPIO config done! :D\n");
	
	/* Configuramos interrupciones */
	printk(KERN_NOTICE "INT config...\n");
	
    if ((res = r_INT_config())) {
		printk(KERN_ERR "INT config failed... :[\n");
		r_cleanup();
		return res;
	}
	printk(KERN_NOTICE "INT config done! :D");
	
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
