
#ifndef __KDEFS_H
#define __KDEFS_H

/* 64-bit page * entry bits */
#define PDE64_PRESENT 1
#define PDE64_RW (1U << 1)
#define PDE64_USER (1U << 2)
#define PDE64_ACCESSED (1U << 5)
#define PDE64_DIRTY (1U << 6)
#define PDE64_PS (1U << 7)
#define PDE64_G (1U << 8)

/*4 level PT idx masks*/
#define PML4_IDX_SHIFT      39
#define PDPT_IDX_SHIFT      30
#define PD_IDX_SHIFT        21
#define PT_IDX_SHIFT        12
#define PGT_IDX_MASK		0x1ffUL
#define PAGE_ADR_MASK		0xfffffffffffff000
#define PTE_TO_PA_MASK		0xfffff000UL
#define LAST_32_BITS		0xffffffffUL

#define PA_TO_PAGE_ADR_MASK 0xfffffffffffff000UL

#define LOG_ON
#ifdef LOG_ON
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(f, ...)
#endif

#endif
