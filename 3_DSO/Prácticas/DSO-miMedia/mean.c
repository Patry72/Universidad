/* Ejercicio módulo para calcular la media de /proc/mean */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/init.h> 
#include <linux/fs.h>       // file operations
#include <linux/uaccess.h> 	// copy to/from user space
#include <linux/vmalloc.h>	
#include <linux/string.h>

/*********************************************
 ---- MODULE LICENSING / DESCRPTION BLOCK ----
 *********************************************/

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Módulo para calcular medias (DSO)");
MODULE_AUTHOR("Patricia Cardenas Fernandez");

/********************************
 ---- DEFINITIONS / VARIABLES----
 ********************************/

#define MESSAGE_LENGTH 256

struct proc_dir_entry *proc_entry;   // Directorio para la media en /proc
static int num_totales;					 // Cuenta de números totales
static int suma_total;					 	 // Suma acumulada

/*****************************
 ---- TRANSLATE OPERATION ----
 *****************************/

/* Comprueba si str es un nº. Si lo es, lo devuelve por valor en forma de entero */
int number (char *str, int *valor)
{
	int num = 0;
	int len = strlen(str);
	int i = 0;
	
	/* Comenzamos con el primer dígito */
	int is_number = str[i] && (str[i] <= '9' && str[i] >= '0');
	
	/* Continuamos hasta que un dígito no sea un número */
	while (is_number && i < len) {
		num = num * 10 + (str[i] - '0'); 
		i++;
		is_number = str[i] && (str[i] <= '9' && str[i] >= '0');
	}
	
	if (is_number) {
		*valor = num;
		
	/* Si no es un número comprobamos si es porque está leyendo el carácter final \0 ó \n, entonces será un número */
	} else if ((!is_number && str[i] == '\n' && i > 0) || (!is_number && str[i] == '\0' && i > 0)) {
		is_number = 1;
		*valor = num;
	}
	
	printk(KERN_INFO "número: %d\n", num);
	printk(KERN_INFO "is_number: %d\n", is_number);
	
	return is_number;
}

/*********************************
 ---- READ / WRITE OPERATIONS ----
 *********************************/
 
/* filp: ptr a fichero, buf: ptr a buffer de user, count: tam del dato a escribir, off: ptr a posición donde está accediendo el user */
ssize_t mean_write (struct file *filp, const char __user *buf, size_t count, loff_t *off)
{
	char numero[MESSAGE_LENGTH];
	int valor;
	
	printk(KERN_INFO "Entrando a mean_write...\n");	
	
	/* Obtenemos dato */
	if (copy_from_user(numero, buf, count )) {
		return -EFAULT;
	}
	
	printk(KERN_INFO "Número introducido: %s\n", numero);
	
	/* Comprobamos si es un número */
	if (strcmp(numero, "CLEAR") == 0) {
		num_totales = 0;
		suma_total = 0;
	} else {
		if (!number(numero, &valor)) {
			printk(KERN_INFO "No es un número\n");
			return -EINVAL;
		} else {
			num_totales++;
			suma_total += valor;
		}
	}
	
	*off += count;   // desplazamos offset
	
	return count;
}

/* filp: ptr a fichero, buf: ptr a buffer vacío de user, count: tam del dato a leer, off: ptr a posición donde está accediendo el user */
ssize_t mean_read (struct file *filp, char __user *buf, size_t count, loff_t *off )
{
	char mensaje[MESSAGE_LENGTH];
	int len;
	int media;
	int p_entera;
	int p_decimal;
	
	printk(KERN_INFO "Entrando a mean_read...\n");

	if (*off > 0) {
		return 0;
	}
	
	if (num_totales == 0) {
		len = sprintf(mensaje, "No hay números para la media\n");
	} else {
		media = (suma_total * 100) / num_totales;
		p_entera = media / 100;    // Nos quedamos con la parte entera
		p_decimal = media % 100;   // Nos quedamos con la parte decimal
		len = sprintf(mensaje, "media: %d.%02d, de %d numeros\n", p_entera, p_decimal, num_totales);
	}
	
	if (copy_to_user(buf, mensaje, len)) {
		return -EFAULT;
	}
	
	*off += len;
	
	return len;
}
 
 /************************
 ---- FILE OPERATIONS ----
 *************************/

static struct file_operations proc_fops = {
	.read 	= mean_read,
	.write  = mean_write,
};
 
 /**********************************
 ---- INITIALIZATION / CLEANUP ----
 **********************************/

void mean_cleanup (void)
{
	/* Eliminamos entrada de /proc */
	printk(KERN_NOTICE "Module cleaning up...\n");
	remove_proc_entry("mean", NULL);
	
	printk(KERN_INFO "mean: Module unloaded! :D\n");
}

int mean_init (void)
{
	int ret = 0;
	
	printk(KERN_NOTICE "*************************************");
	printk(KERN_NOTICE "Hello, loading %s module! :D\n", KBUILD_MODNAME);
	printk(KERN_NOTICE "*************************************");
	
	/* Creamos entrada en /proc */
	proc_entry = proc_create("mean", 0666, NULL, &proc_fops);   // NULL -> estará en el propio /proc
	// proc_entry = create_proc_entry("mean", 0666, NULL);  // Antes se usaba create_proc_entry
	
	if (!proc_entry) {
		ret = -ENOMEM;
		printk(KERN_INFO "mean: Couldn't create proc entry... :[\n");
		remove_proc_entry("mean", NULL);
	} else {
		num_totales = 0;
		suma_total = 0;
		printk(KERN_INFO "mean: proc entry created! :D\n");
	}
	
	return ret;
}
 
module_init(mean_init);
module_exit(mean_cleanup);
