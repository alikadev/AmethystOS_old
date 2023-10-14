#include <sys/io.h>

inline void io_wait(void)
{
    outb(0x80, 0);
}