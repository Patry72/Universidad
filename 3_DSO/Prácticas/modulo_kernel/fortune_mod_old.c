/*---------------------------------------------
 * Diseño de Sistemas Operativos (DAC) *
	para compilar el módulo: 
	$ MODULE=fortune_mod make
	para instalar el módulo:
	$ sudo insmod fortune_mod.ko 
	para comprobar si el módulo fue cargado:
	$ sudo lsmod
	$ dmesg | tail
	para desinstalar el módulo:
	$ sudo rmmod fortune_mod.ko
----------------------------------------------*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

#define MAX_COOKIE_LENGTH       PAGE_SIZE

static struct proc_dir_entry *proc_entry;
static char *cookie_pot;  // Space for fortune strings
static int cookie_index;  // Index to write next fortune
static int next_fortune;  // Index to read next fortune


/****************************************************************************/
/* file operations                                                          */
/****************************************************************************/
// escritura del fichero
ssize_t fortune_write (struct file *filp,const char *buf, size_t count, loff_t *off)
{
  int space_available = (MAX_COOKIE_LENGTH-cookie_index)+1;
  if (count > space_available) {
    printk(KERN_INFO "fortune: cookie pot is full!\n");
    return -ENOSPC;
  }
  if (copy_from_user( &cookie_pot[cookie_index], buf, count )) {
    return -EFAULT;
  }
  cookie_index += count;
  *off+=count;
  cookie_pot[cookie_index-1] = 0;
  return count;
}

// lectura del fichero
ssize_t fortune_read (struct file *filp, char __user *buf, size_t count, loff_t *off )
{
  int len;
  if (*off > 0)  return 0;
 
  /* Wrap-around */
  if (next_fortune >= cookie_index) next_fortune = 0;
  len = sprintf(buf, "%s\n", &cookie_pot[next_fortune]);
  next_fortune += len;
  *off+=len;
  return len;
}


struct file_operations proc_fops = {
	read : fortune_read,
	write: fortune_write
};


/****************************************************************************/
/* Module init / cleanup block.                                             */
/****************************************************************************/

// al cargar el modulo
int init_fortune_module( void )
{
  int ret = 0;
  cookie_pot = (char *)vmalloc( MAX_COOKIE_LENGTH );
  if (!cookie_pot) {
    ret = -ENOMEM;
  } else {
    memset( cookie_pot, 0, MAX_COOKIE_LENGTH );
    
    proc_entry = proc_create("fortune",0666,NULL,&proc_fops);

    if (proc_entry == NULL) {
      ret = -ENOMEM;
      vfree(cookie_pot);
      printk(KERN_INFO "fortune: Couldn't create proc entry\n");
    } else {
      cookie_index = 0;
      next_fortune = 0;
      printk(KERN_INFO "fortune: Module loaded.\n");
    }
  }
  return ret;
}

// al descargar el modulo
void cleanup_fortune_module( void )
{
  remove_proc_entry("fortune", NULL);
  vfree(cookie_pot);
  printk(KERN_INFO "fortune: Module unloaded.\n");
}

module_init( init_fortune_module );
module_exit( cleanup_fortune_module );



/****************************************************************************/
/* Module licensing/description block.                                      */
/****************************************************************************/


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Fortune Cookie Kernel Module (DSO)");
MODULE_AUTHOR("basado en el trabajo de M. Tim Jones, adaptado para kernel > 3.10");
