#ifndef __SYS__PAGING_INTERNAL_H_
#define __SYS__PAGING_INTERNAL_H_

#include <stdint.h>

struct page_s
{
    uint32_t present: 1;
    uint32_t read_write: 1;
    uint32_t user_supervisor: 1;
    uint32_t write_through: 1;
    uint32_t cache_disable: 1;
    uint32_t accessed: 1;
    uint32_t dirty: 1;
    uint32_t page_attribute_table: 1;
    uint32_t global: 1;
    uint32_t available: 3;
    uint32_t address: 20;
} __attribute__((packed));

typedef struct page_s page_directory_t;
typedef struct page_s page_entry_t;

#define PAGE_TABLE_SIZE 1024
#define PAGE_SIZE 4096

void load_page_directory(page_directory_t *directory);
void enable_paging(void);
#endif
