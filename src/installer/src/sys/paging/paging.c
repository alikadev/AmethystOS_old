#include <sys/paging.h>
#include "paging_internal.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static page_directory_t directory_table[PAGE_TABLE_SIZE] __attribute__((aligned(PAGE_SIZE))) = {0};
static page_entry_t kernel_table[PAGE_TABLE_SIZE] __attribute__((aligned(PAGE_SIZE))) = {0};

void paging_init(void)
{
    // Init the kernel page with default value
    for (size_t i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        // Fill the kernel page table
        kernel_table[i].present = 1;
        kernel_table[i].read_write = 1;
        kernel_table[i].address = i;
    }

    // Init the page directory
    for (size_t i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        // Fill the kernel page table
        directory_table[i].address = (uint32_t)kernel_table >> 12;
        directory_table[i].read_write = 1;
        directory_table[i].present = 1;
    }

    load_page_directory(directory_table);

    enable_paging();
}
