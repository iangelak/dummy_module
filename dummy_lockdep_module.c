/* dummy_lockdep_module.c -- a simple kernel module to create a Lockdep splat */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/lockdep.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name Here <you@mail.srv>");
MODULE_DESCRIPTION("A simple module to trigger lockdep");
MODULE_VERSION("1.0");

struct mutex mutexA;
struct mutex mutexB;


void function_A(void)
{
    mutex_lock(&mutexA);
    mutex_lock(&mutexB);

    /* Critical section of function_A*/
    printk(KERN_INFO "%s: Locked mutexA and then mutexB\n", __func__);
    
    mutex_unlock(&mutexB);
    mutex_unlock(&mutexA);
}
/* EXPORT_SYMBOL(function_A); */

void function_B(void)
{
    mutex_lock(&mutexB);
    mutex_lock(&mutexA);

    /* Critical section of function_B*/
    printk(KERN_INFO "%s: Locked mutexB and then mutexA\n", __func__);
    
    mutex_unlock(&mutexA);
    mutex_unlock(&mutexB);
}

/* EXPORT_SYMBOL(function_B); */

static int __init dummy_init(void)
{
    mutex_init(&mutexA);
    mutex_init(&mutexB);
    
    printk(KERN_INFO "Initialized both mutexes!\n");

    /* Run function_A*/
    function_A();

    /*Run function_B*/
    function_B();
    
    return 0;
}

static void __exit dummy_exit(void)
{
    printk(KERN_INFO "Goodbye, unloading the dummy module!\n");
}

module_init(dummy_init);
module_exit(dummy_exit);
