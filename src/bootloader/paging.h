#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGE_PRESENT 0x1
#define PAGE_RW      0x2
#define PAGE_PS      0x80
#define PAGE_FLAGS   (PAGE_PRESENT | PAGE_RW | PAGE_PS)

#define ENTRIES_PER_TABLE 512
#define GIB (1024ULL * 1024ULL * 1024ULL)

// Page tables must be 4K aligned
__attribute__((aligned(0x1000))) static volatile uint64_t pml4[ENTRIES_PER_TABLE];
__attribute__((aligned(0x1000))) static volatile uint64_t pdpt[ENTRIES_PER_TABLE];

static inline void init_identity_paging(void) {
    // Zero PDPT
    for (uint64_t i = 0; i < ENTRIES_PER_TABLE; i++) {
        pdpt[i] = 0;
    }

    // Fill PDPT with 1GiB identity-mapped pages (512 GiB total)
    for (uint64_t i = 0; i < ENTRIES_PER_TABLE; i++) {
        uint64_t phys_addr = i * GIB;
        pdpt[i] = phys_addr | PAGE_FLAGS;  // 1 GiB pages
    }

    // Zero PML4
    for (uint64_t i = 0; i < ENTRIES_PER_TABLE; i++) {
        pml4[i] = 0;
    }

    // Set PML4[0] to point to PDPT physical address
    pml4[0] = ((uintptr_t)pdpt) | PAGE_PRESENT | PAGE_RW;
}

static inline void enable_paging(void) {
    uint64_t cr4, cr0;
    uint32_t lo, hi;

    // Enable PAE and PSE in CR4 before loading CR3
    asm volatile("mov %%cr4, %0" : "=r"(cr4));
    cr4 |= (1 << 5) | (1 << 4);  // PAE=bit5, PSE=bit4
    asm volatile("mov %0, %%cr4" : : "r"(cr4));

    // Load CR3 with physical address of PML4 (must be physical!)
    asm volatile("mov %0, %%cr3" : : "r"((uintptr_t)pml4));

    // Enable LME (long mode enable) in EFER MSR
    asm volatile("mov $0xC0000080, %%ecx\n\trdmsr" : "=a"(lo), "=d"(hi) : : "ecx");
    lo |= (1 << 8); // LME bit
    asm volatile("mov $0xC0000080, %%ecx\n\twrmsr" : : "a"(lo), "d"(hi) : "ecx");

    // Enable paging and protected mode in CR0
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= (1UL << 31) | (1UL << 0);  // PG and PE bits
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}

#endif // PAGING_H
