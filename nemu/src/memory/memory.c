#include "common.h"
#include "memory/cache.h"
#include "cpu/reg.h"

#define PAGE_LEN 4096		//4KB per page
#define PAGE_MASK (PAGE_LEN - 1)

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

static lnaddr_t seg_translate(swaddr_t addr, uint8_t sreg)
{
	Assert((addr <= cpu.sreg_lmt[sreg] + 1) * 4096 - 1,				\
		   "addr = %#x > seg_base + seg_limit\n!", addr);
	addr += cpu.sreg_base[sreg];
	return addr;
}

static hwaddr_t page_translate(lnaddr_t addr)
{
	panic("To be completed");
}

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len)
{
	/* if (len = 1) return dram_read(addr, len) & 1111b */
	/* if (len = 2)....*/
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data)
{
	dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len)
{
	hwaddr_t hwaddr;
	if (cpu._cr0._pg) {
		hwaddr = page_translate(addr);
		uint32_t offset = hwaddr & PAGE_MASK;
		Assert (offset + len <= PAGE_LEN, "Cross the page boundary!\n");
	} else 
		hwaddr = addr;
		
	return hwaddr_read(hwaddr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data)
{
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg)
{
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr;
	if (cpu._cr0._pg)
		lnaddr = seg_translate(addr, sreg); 
	else
		lnaddr = addr;
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data)
{
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}


