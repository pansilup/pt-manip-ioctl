#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/kconfig.h>

#include <asm/cpufeature.h>  // For boot_cpu_has()
#include <linux/sched.h>
#include <linux/mm.h>        // For `pgd_t` and related definitions
#include <asm/pgtable.h>     // For page table manipulation macros
#include <linux/highmem.h>   //for kmap_atomic()

#include "kernel_agent.h"
#include "kdefs.h"
#include "../simTDX/seamManager/krover_manager.h"

#define CR3_KERNEL_TO_USER_MASK 0x1000UL /*If the 12th bit is clear, kernel CR3. Add the mask to get user CR3*/
#define PDE64_PRESENT 1UL

#define LOG(...) printk("kernel_agent: " __VA_ARGS__)

typedef unsigned long ulong;

static struct s_adr seam_adr_k[TDXMODULE_MAPPED_PAGE_COUNT];

unsigned long read_int3(void){
    unsigned long cr3_val = 0;

    asm volatile("movq %%cr3, %0; \n\t"
                :"=r"(cr3_val)::);
    return cr3_val;
}

static ulong *pa_to_va(ulong pa){

    unsigned pfn;
    struct page *pg;
    ulong *ptr;

    pfn = pa >> 12;
    pg = pfn_to_page(pfn);
    ptr = (ulong *)kmap_atomic(pg);

    return ptr;
}

static void pa_to_va_free(void *va){
    kunmap_atomic(va);
}

// /*given a kernel va, translate and find the pa*/
// static ulong translate_to_pa(ulong va, ulong *page_size, ulong *pml4){

// 	uint64_t pdpt_pa, pd_pa, pt_pa;
// 	uint64_t *pdpt, *pd, *pt;
// 	uint32_t pml4_idx, pdpt_idx, pd_idx, pt_idx;



// }

static int update_page_tables(ulong arg){
    unsigned long cr3_kernel;
    ulong *pml4_kernel_va, *pml4_user_va, pml4_kernel_pa, pml4_user_pa;
    ulong page_size, user_data_size;
    // int idx;
    // unsigned long *tbl;
    // ulong *tmp;
    LOG("arg: %lx\n", arg);
    LOG("task name: %s\n", current->comm);
    LOG("active_mm: %lx\n", (unsigned long)current->active_mm);

    pml4_kernel_pa = read_int3() & PA_TO_PAGE_ADR_MASK;
    pml4_user_pa = pml4_kernel_pa | CR3_KERNEL_TO_USER_MASK;
    LOG("cr3_kernel: %lx cr3 user:%lx\n", pml4_kernel_pa, pml4_user_pa);

    user_data_size = sizeof(seam_adr_k);
    LOG("user data size: 0x%lx\n", user_data_size);

    /*copy_from_user returns the number of bytes it could not copy. So at success : return 0*/
    if(copy_from_user((void *)seam_adr_k, (const void __user *)arg, user_data_size)){
        LOG("copy_from_user ERR\n");
        return -1;
    }
    LOG("copy_from_user SUCCESS\n");

    // pml4_kernel_va  = pa_to_va(pml4_kernel_pa);
    pml4_user_va = pa_to_va(pml4_user_pa);
    LOG("pml4_user_va: %lx\n", pml4_user_va);
    pa_to_va_free((void *)pml4_user_va);
    // tmp = (ulong *)current->active_mm->pgd;
    // pml4_kernel_pa = translate_to_pa(pml4_kernel_va, &page_size, pml4_kernel_pa);
    // pgd_user_va    = (ulong *)((unsigned long)pgd_kernel | CR3_KERNEL_TO_USER_MASK);
    // LOG("pgd kernel: %lx pa: %lx\n", pgd_kernel, (unsigned long)virt_to_phys((void *)pgd_kernel));
    // LOG("pgd user  : %lx pa: %lx\n", (unsigned long)pgd_user, (unsigned long)virt_to_phys((void *)pgd_user));

    // LOG("Printing kernel pml4 entries present...");
    // idx = 0;
    // tbl = (unsigned long *)pml4_kernel_va;
    // while(idx < 512){
    //     if(tbl[idx] & PDE64_PRESENT){
    //         LOG("idx: %03d value: %lx\n", idx, tbl[idx]);
    //     }
    //     idx++;
    // }
    
    // LOG("Printing user pml4 entries present...");
    // idx = 0;
    // tbl = tmp;
    // while(idx < 512){
    //     if(tbl[idx] & PDE64_PRESENT){
    //         LOG("idx: %03d value: %lx\n", idx, tbl[idx]);
    //     }
    //     idx++;
    // }

    return 0;
}

static long ioctl_handler(struct file *file, unsigned int ioctl_command, unsigned long arg){

    switch (ioctl_command) {
        case IOCTL_UPDATE_PT:{
            if(update_page_tables(arg) != 0){
                return -EINVAL;
            }
            LOG("update_page_tables: SUCCESS\n");
        } break;
        default: {
            return -EINVAL;
        }
    }
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = ioctl_handler,
};

static struct miscdevice kernel_agent_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name   = "kernel_agent_device",
    .fops   = &fops,
};

static int __init agent_init(void){

    int status;

    /*check if PTI is enabled for the kernel dung compilation*/
    if(IS_ENABLED(CONFIG_PAGE_TABLE_ISOLATION)){
        LOG("CONFIG_PAGE_TABLE_ISOLATION is enabled.\n");

        /*Check if the Page Table Isolation is turned on at boot time and is effective*/
        if (boot_cpu_has(X86_FEATURE_PTI)) {
            LOG("PTI is turned on.\n");
        } else {
            LOG("PTI is not turned on. set command line boot parameter pti=on\n");
            return -1;
        }
    }
    else{
        LOG("CONFIG_PAGE_TABLE_ISOLATION is not enabled, recompile the kernel with the config enabled.\n");
        return -1;
    }

    /*register a new character device*/
    status = misc_register(&kernel_agent_device);
    if(status){
        LOG("unable to register kernel_agent_device");
        return status;
    }

    LOG("kernel_agent loaded\n");
    return 0;
}

static void __exit agent_exit(void){

    misc_deregister(&kernel_agent_device);
    LOG("kernel agent unloaded\n");
}

module_init(agent_init);
module_exit(agent_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("pansilu pitigalaarachchi");
MODULE_DESCRIPTION("Upon the request of KRover+ process, agent will update PTs");
