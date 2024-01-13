#ifndef __ERROR_H_
#define __ERROR_H_

enum err_type
{
	_ERR_NO = 0,
	_ERR_INTERNAL,
	_ERR_INVARG,
	_ERR_HEAPFULL,
	_ERR_NOFONT,
	_ERR_IO,
	_ERR_UNKNFS,
	_ERR_NOTFOUND,
	_ERR_NOTADIR,
	_ERR_NOTAFILE,
};

extern enum err_type _err;
const char *_errstr(void);
#define _errreset {_err = _ERR_NO;}

#endif
