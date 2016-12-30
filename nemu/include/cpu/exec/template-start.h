#include "cpu/exec/helper.h"

#if DATA_BYTE == 1

#define SUFFIX b
#define DATA_TYPE uint8_t
#define DATA_TYPE_S int8_t
#define MOST_SIGNIFICANCE 0x80
#define MAX_UVAL 0xff

#elif DATA_BYTE == 2

#define SUFFIX w
#define DATA_TYPE uint16_t
#define DATA_TYPE_S int16_t
#define MOST_SIGNIFICANCE 0x8000
#define MAX_UVAL 0xffff

#elif DATA_BYTE == 4

#define SUFFIX l
#define DATA_TYPE uint32_t
#define DATA_TYPE_S int32_t
#define MOST_SIGNIFICANCE 0x80000000
#define MAX_UVAL 0xffffffff

#else

#error unknown DATA_BYTE

#endif

#define REG(index) concat(reg_, SUFFIX) (index)
#define REG_NAME(index) concat(regs, SUFFIX) [index]

#define MEM_R(addr) swaddr_read(addr, DATA_BYTE)
#define MEM_W(addr, data) swaddr_write(addr, DATA_BYTE, data)

#define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)

#define MSB(n) ((DATA_TYPE)(n) >> ((DATA_BYTE << 3) - 1))
	/* EFLAGS */
#define check_sf(x) do {													\
							if((x & MOST_SIGNIFICANCE) == MOST_SIGNIFICANCE) 	\
								cpu._sf = 1;									\
							else												\
								cpu._sf = 0;									\
						} while (0)

#define check_zf(x) do {													\
							if(x == 0)											\
								cpu._zf = 1;									\
							else 												\
								cpu._zf = 0;									\
						}while (0)
						
#define check_pf(x) do {													\
							uint32_t tmp = 1, iii;								\
							cpu._pf = 1;										\
							for (iii = 0; iii < 8; iii++) {						\
								if (tmp & x) {									\
									cpu._pf = ~cpu._pf;							\
								}												\
							tmp <<= 1;											\
							}													\
						} while(0)

#define check_add_cf(src, dest) do {											\
									if ((MAX_UVAL - src) < dest)				\
										cpu._cf = 1;							\
									else 										\
										cpu._cf = 0;							\
								} while(0)

#define check_sub_cf(src, dest) do {											\
									if (src > dest)								\
										cpu._cf = 1;							\
									else										\
										cpu._cf = 0;							\
								} while(0)		


#define check_add_of(src, dest, re) do {										\
										cpu._of = 0;							\
										DATA_TYPE src_h, dest_h;				\
										src_h = src & MOST_SIGNIFICANCE;		\
										dest_h = dest & MOST_SIGNIFICANCE;		\
										if (src_h == dest_h) {					\
											DATA_TYPE re_h = 					\
												re &MOST_SIGNIFICANCE;			\
											if(re_h != dest_h)					\
												cpu._of = 1;					\
										}										\
									} while(0)

#define check_sub_of(src, dest, re) do {										\
										cpu._of = 0;							\
										DATA_TYPE src_h, dest_h;				\
										src_h =	src & MOST_SIGNIFICANCE;		\
										dest_h = dest & MOST_SIGNIFICANCE;		\
										if (src_h != dest_h) {					\
											DATA_TYPE re_h = 					\
												re & MOST_SIGNIFICANCE;			\
											if (re_h != dest_h)					\
												cpu._of = 1;					\
										}										\
									} while(0)
									
#define check_add_af(src, dest) do {											\
									if ((0xff - (src & 0xff)) < (dest & 0xff))	\
										cpu._af = 1;							\
									else 										\
										cpu._af = 0;							\
								} while(0)

#define check_sub_af(src, dest) do {											\
									if ((dest & 0xff) < (src & 0xff)) 			\
										cpu._af = 1;							\
									else										\
										cpu._af = 0;							\
								} while(0)
