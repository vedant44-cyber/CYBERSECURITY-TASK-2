#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <asm/unistd.h>

#define __NR_execve 221  

static unsigned long *sys_call_table;

asmlinkage long (*orig_execve)(const char __user *filename,
                                const char __user *const __user *argv,
                                const char __user *const __user *envp);

asmlinkage long hooked_execve(const char __user *filename,
                               const char __user *const __user *argv,
                               const char __user *const __user *envp)
{
    printk(KERN_INFO "execve() syscall hooked: filename=%s\n", filename);
   
    return orig_execve(filename, argv, envp);
}

static inline unsigned long read_cr0(void)
{
    unsigned long val;
    asm volatile("mrs %0, S3_0_C15_C0_0" : "=r"(val));
    return val;
}

static inline void write_cr0(unsigned long val)
{
    asm volatile("msr S3_0_C15_C0_0, %0" : : "r"(val));
}

static void unprotect_memory(void)
{
    unsigned long cr0 = read_cr0();
    write_cr0(cr0 & ~0x10000);
}

static void protect_memory(void)
{
    unsigned long cr0 = read_cr0();
    write_cr0(cr0 | 0x10000);
}

static int __init my_module_init(void)
{
    sys_call_table = (unsigned long *)kallsyms_lookup_name("sys_call_table");
    if (!sys_call_table)
    {
        printk(KERN_ERR "Failed to locate sys_call_table\n");
        return -1;
    }

   
    orig_execve = (void *)sys_call_table[__NR_execve];
    printk(KERN_INFO "Original execve syscall address: %lx\n", (unsigned long)orig_execve);

  
    unprotect_memory();
    sys_call_table[__NR_execve] = (unsigned long)hooked_execve;
    protect_memory();

    printk(KERN_INFO "Successfully hooked execve syscall\n");
    return 0;
}

static void __exit my_module_exit(void)
{
    
    unprotect_memory();
    sys_call_table[__NR_execve] = (unsigned long)orig_execve;
    protect_memory();

    printk(KERN_INFO "Restored original execve syscall\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");