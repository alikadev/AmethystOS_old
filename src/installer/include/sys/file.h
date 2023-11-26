#ifndef __SYS__FILE_H_
#define __SYS__FILE_H_

#include <sys/disk.h>

void *file_read(const disk_t *disk, const char *filename);

#endif