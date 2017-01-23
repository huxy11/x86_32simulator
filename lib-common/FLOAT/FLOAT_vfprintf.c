#include <stdio.h>
#include <stdint.h>
#include "FLOAT.h"
#include <sys/mman.h>

extern char _vfprintf_internal;
extern char _ppfs_setargs;
extern char _fpmaxtostr;
extern int __stdio_fwrite(char *buf, int len, FILE *stream);

__attribute__((used)) static int format_FLOAT(FILE *stream, FLOAT f) {
	//nemu_assert(f == 0x00001111);
	uint32_t sign = f & 0x80000000;
	if (sign)
		f = ~f + 1;
	int16_t integer = (f & 0xffff0000) >> 16;
	uint64_t decimal = f;
	//decimal = (decimal & 0xffff) * 1000000 / 65536;
	decimal &= 0xffff;
	decimal *= 1000000;
	//nemu_assert(decimal == 4369000000);
	uint32_t *d1 = (int*)&decimal;
	uint32_t *d2 = d1 + 1;
	uint32_t d3 = 0x10000;
	asm volatile ("idiv %2" : "=a"(*d1), "=d"(d3): "r"(d3), "a"(*d1), "d"(*d2));
	*d2 = 0;
	//nemu_assert(decimal == 66665);
	char buf[80];
	int len;
	if (sign) 
		len = sprintf(buf, "-%d.%-06d", integer, decimal);
	else 
		len = sprintf(buf, "%d.%-06d", integer, decimal);

	/* TODO: improve this */
	/* replace ' ' with '0' */
	int j = 2;
	for (;j < len; j++)
		if (buf[j] == 0x20)
			buf[j] = 0x30;
		
	return __stdio_fwrite(buf, len, stream);
}

static void modify_vfprintf() {
	uint8_t *vfp = &_vfprintf_internal;
	uint8_t *fts = &_fpmaxtostr;
	/* Calculate the address of call(_fpmaxtostr) */
	int32_t offset = (int)fts - (int)format_FLOAT;
	vfp += 0x307;
	/* Modify the destiantion address*/
	//mprotect((void *)((uint32_t)(vfp - 0x64) & 0xFFFFF000), 4096 * 2, \
						PROT_READ | PROT_WRITE | PROT_EXEC);
	*(int32_t*)vfp -= offset;
	/* Rectify the stack size */
	vfp -= 12;
	vfp[0] = 0x8;
	/* Change the arguments */
	vfp[1] = 0xff;
	vfp[2] = 0x32;
	vfp[3] = 0x90;
	/* Clean the float instrucion */
	vfp[-18] = 0x90;
	vfp[-19] = 0x90;
	vfp[-22] = 0x90;
	vfp[-23] = 0x90;

	return;
	

	
	/* 
	 * Below is the code section in _vfprintf_internal() relative to the
	 * hijack.
	 */

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = _fpmaxtostr(stream,
				(__fpmax_t)
				(PRINT_INFO_FLAG_VAL(&(ppfs->info),is_long_double)
				 ? *(long double *) *argptr
				 : (long double) (* (double *) *argptr)),
				&ppfs->info, FP_OUT );
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

	/* You should modify the run-time binary to let the code above
	 * call `format_FLOAT' defined in this source file, instead of
	 * `_fpmaxtostr'. When this function returns, the action of the
	 * code above should do the following:
	 */

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = format_FLOAT(stream, *(FLOAT *) *argptr);
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

}

static void modify_ppfs_setargs() {
	uint8_t *mps = &_ppfs_setargs;
	mps[111] = 0x74;
	mps[112] = 0x32;
	//mprotect((void *)((uint32_t)mps), 4096 * 2, \
		PROT_READ | PROT_WRITE | PROT_EXEC);
	

	/*
	 * Below is the code section in _vfprintf_internal() relative to
	 * the modification.
	 */

#if 0
	enum {                          /* C type: */
		PA_INT,                       /* int */
		PA_CHAR,                      /* int, cast to char */
		PA_WCHAR,                     /* wide char */
		PA_STRING,                    /* const char *, a '\0'-terminated string */
		PA_WSTRING,                   /* const wchar_t *, wide character string */
		PA_POINTER,                   /* void * */
		PA_FLOAT,                     /* float */
		PA_DOUBLE,                    /* double */
		__PA_NOARG,                   /* non-glibc -- signals non-arg width or prec */
		PA_LAST
	};

	/* Flag bits that can be set in a type returned by `parse_printf_format'.  */
	/* WARNING -- These differ in value from what glibc uses. */
#define PA_FLAG_MASK		(0xff00)
#define __PA_FLAG_CHAR		(0x0100) /* non-gnu -- to deal with hh */
#define PA_FLAG_SHORT		(0x0200)
#define PA_FLAG_LONG		(0x0400)
#define PA_FLAG_LONG_LONG	(0x0800)
#define PA_FLAG_LONG_DOUBLE	PA_FLAG_LONG_LONG
#define PA_FLAG_PTR		(0x1000) /* TODO -- make dynamic??? */

	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			case (PA_INT|PA_FLAG_LONG):
				GET_VA_ARG(p,ul,unsigned long,ppfs->arg);
				break;
			case PA_CHAR:	/* TODO - be careful */
				/* ... users could use above and really want below!! */
			case (PA_INT|__PA_FLAG_CHAR):/* TODO -- translate this!!! */
			case (PA_INT|PA_FLAG_SHORT):
			case PA_INT:
				GET_VA_ARG(p,u,unsigned int,ppfs->arg);
				break;
			case PA_WCHAR:	/* TODO -- assume int? */
				/* we're assuming wchar_t is at least an int */
				GET_VA_ARG(p,wc,wchar_t,ppfs->arg);
				break;
				/* PA_FLOAT */
			case PA_DOUBLE:
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			case (PA_DOUBLE|PA_FLAG_LONG_DOUBLE):
				GET_VA_ARG(p,ld,long double,ppfs->arg);
				break;
			default:
				/* TODO -- really need to ensure this can't happen */
				assert(ppfs->argtype[i-1] & PA_FLAG_PTR);
			case PA_POINTER:
			case PA_STRING:
			case PA_WSTRING:
				GET_VA_ARG(p,p,void *,ppfs->arg);
				break;
			case __PA_NOARG:
				continue;
		}
		++p;
	}
#endif

	/* You should modify the run-time binary to let the `PA_DOUBLE'
	 * branch execute the code in the `(PA_INT|PA_FLAG_LONG_LONG)'
	 * branch. Comparing to the original `PA_DOUBLE' branch, the
	 * target branch will also prepare a 64-bit argument, without
	 * introducing floating point instructions. When this function
	 * returns, the action of the code above should do the following:
	 */

#if 0
	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
			here:
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			// ......
				/* PA_FLOAT */
			case PA_DOUBLE:
				goto here;
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			// ......
		}
		++p;
	}
#endif

}

void init_FLOAT_vfprintf() {
	modify_vfprintf();
	modify_ppfs_setargs();
}
