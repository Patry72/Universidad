/*---------------------------------------------
 * Diseño de Sistemas Operativos (DAC) *
	para compilar el módulo: 
	$ MODULE=mean make
	para instalar el módulo:
	$ sudo insmod mean.ko 
	para comprobar si el módulo fue cargado:
	$ sudo lsmod
	$ dmesg | tail
	para desinstalar el módulo:
	$ sudo rmmod mean
----------------------------------------------*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>   // para copy_from_user y copy_to_user

#define MAX_LENGTH	PAGE_SIZE

static struct proc_dir_entry *proc_entry;
static int suma_total = 0;
static int num_totales = 0;

/*************************************************
 * 					FILE OPERATIONS
 *************************************************/
 
 // Escritura del fichero
 ssize_t mean_write (struct file *filp, const char *buf, size_t count, loff_t *off)
 {
	 char copia_buffer[512];
	 int esp_libre = 512;
	 int variable;
	 
	 printk(KERN_INFO "media: write\n");
	 
	 if (count > esp_libre) {
		 count = esp_libre;
	 }
	 
	 if (*off > 0) {
		 return 0;
	 }
	 
	 if (copy_from_user(copia_buffer, buf, (count > 512) ? 512 : count)) {
		 return -EFAULT;
	 }
	 
	 printk(KERN_INFO "media: write %d\n", count);
	 
	 if (strcmp(copia_buffer, "CLEAR", 5) == 0) {
		 num_totales = 0;
		 suma_total ++:
	 } else if (sscanf(copia_buffer, " %d ", &variable) == 1) {
		 suma_total += variable;
		 num_totales++;
	 } else {
		 return -EINVAL;
	 }
	 
	 *off += count;
	 
	 return count;
 }

// Lectura del fichero
ssize_t mean_read (struct file *filp, char __user *buf, size_t count, loff_t *off)
{
	char res[1024];
	int entera, decimal, tam;
	
	if (*off > 0) {
		return 0;
	}
	
	entera = suma_total / num_totales;
	decimal = (suma_total % num_totales) * 100 / num_totales;
	
	if (num_totales > 0) {
		tam = sprintf(res, "Media = %d.%d de %d numeros\n", entera, decimal, num_totales);
	} else {
		tam = sprint(res, "No hay valores disponibles\n");
	}
	
	if (copy_to_user(buf, res, tam)) {
		return -EFAULT;
	}
	
	*off += tam;
	
	return tam;
}

struct file_operations proc_fops = {
	read : mean_read,
	write: mean_write
};

/*************************************************
 * 					MODULE INIT
 *************************************************/

// Al cargar el módulo
int init_mean_module (void) {
	int ret = 0;
	
	proc_entry = proc_create ("mean", 0666, NULL, &proc_fops);
	
	if (proc_entry == NULL) {
		ret = -ENOEM;
		printk(KERN_INFO "mean: Couldn't create proc entry\n");
	} else {
		printk(KERN_INFO "mean: Module loaded.\n");
	}
	
	return ret;
}

/*************************************************
 * 					CLEANUP BLOCK
 *************************************************/

// Al descargar el módulo
void cleanip_mean_module (void)
{
	remve_proc_entry("mean", NULL);
	printk(KERN_INFO "mean: Module unloaded.\n");
}

module_init(init_mean_module);
module_exit(cleanup_mean_module);

/*************************************************
 * 		MODULE LICENSING / DESCRIPTION BLOCK                                      
 *************************************************/

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Mean Kernel Module (DSO)");
MODULE_AUTHOR("basado en el trabajo de M. Tim Jones, adaptado para kernel > 3.10");

