#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/kthread.h>
#include <linux/delay.h>
//#include <asm-generic/uaccess.h>

/****************************************************************************/
/* Module licensing/description block.                                      */
/****************************************************************************/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DAC");
MODULE_DESCRIPTION("Simple module featuring proc read with a kthread");

/****************************************************************************/
// Globals
/****************************************************************************/

#define ENTRY_NAME "counter"
#define PERMS 0644
#define PARENT NULL
static struct task_struct *kthread;
static int counter;

/****************************************************************************/
// kthread run function
/****************************************************************************/

int counter_run(void *data) 
{
	while (!kthread_should_stop()) {
		ssleep(1);
		counter++;
	}
	printk(KERN_NOTICE "The counter thread has terminated\n");
	return counter;
}

/****************************************************************************/
// proc file operations (read only)
/****************************************************************************/

ssize_t counter_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset) {
 char message[20];
 int len; 
 
 if(*offset > 0) return 0; // solo lee al principio del fichero, offset==0
 
 printk(KERN_NOTICE "proc called read\n");
 len = sprintf(message,"%d\n",counter);
 len -= copy_to_user(buf, message, len);
 
 *offset+=len;
 return len;
}

//static struct file_operations fops= { read : counter_proc_read };
static struct proc_ops fops = {
	.proc_read = counter_proc_read,
};


/****************************************************************************/
/* Module init / cleanup block.                                             */
/****************************************************************************/
int r_init(void) 
{
    printk(KERN_NOTICE "Hello, loading DSO %s module!\n",KBUILD_MODNAME);
	counter=0;
    printk(KERN_NOTICE "kthread counter created\n");
    kthread = kthread_run(counter_run, NULL, ENTRY_NAME);
    if (IS_ERR(kthread)) 
	{ 
		printk(KERN_ERR "ERROR! kthread_run\n");
		return PTR_ERR(kthread);
	}
   
    printk(KERN_NOTICE "/proc/%s create\n", ENTRY_NAME);
	if (!proc_create(ENTRY_NAME, PERMS, PARENT, &fops)) 
	{
		printk(KERN_ERR "ERROR! proc_create\n");
		remove_proc_entry(ENTRY_NAME, PARENT);
		return -ENOMEM;
	}
    return 0;
}

void r_cleanup(void) 
{
	int ret;
	
    printk(KERN_NOTICE "DSO %s module cleaning up...\n",KBUILD_MODNAME);

    ret = kthread_stop(kthread);
    if (ret != -EINTR) printk(KERN_NOTICE "Counter thread has stopped\n");

    remove_proc_entry(ENTRY_NAME, NULL);
    printk(KERN_NOTICE "Removing /proc/%s.\n", ENTRY_NAME);
    printk(KERN_NOTICE "Done. Bye\n");
    return;
}

module_init(r_init);
module_exit(r_cleanup);
