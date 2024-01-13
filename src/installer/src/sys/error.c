#include <sys/error.h>

enum err_type _err = _ERR_NO;

const char *errstrs[] =
{
  [_ERR_NO] = "No error",
  [_ERR_INVARG] = "Invalid argument",
  [_ERR_INTERNAL] = "Internal error",
  [_ERR_HEAPFULL] = "Heap is full",
  [_ERR_NOFONT] = "No font is set up",
  [_ERR_IO] = "IO error",
  [_ERR_UNKNFS] = "Unknown FS",
  [_ERR_NOTFOUND] = "Not found",
  [_ERR_NOTADIR] = "Not a directory",
  [_ERR_NOTAFILE] = "Not a file",
};

const char *_errstr(void)
{
  return errstrs[_err];
}
