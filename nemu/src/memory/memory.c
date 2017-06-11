#include "common.h"
#include "memory/cache.h"
#include "cpu/reg.h"

#define PAGE_LEN 4096		//4KB per page
#define PAGE_OFFSET_MASK (PAGE_LEN - 1)

uint32_t dram_read(hwaddr_t, size_t);
uint32_t hwaddr_read(hwaddr_t addr, size_t len);
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
	union {
		uint32_t val;
		struct {
			uint32_t offset	: 12;
			uint32_t idx_2	: 10;
			uint32_t idx_1	: 10;
		};
	} temp;
	union {
		uint32_t val;
		struct {
			uint32_t present			: 1;
			uint32_t read_write			: 1;
			uint32_t user_supervisor	: 1;
			uint32_t page_write_through	: 1;
			uint32_t page_cache_disable	: 1;
			uint32_t accessed			: 1;
			uint32_t pad0				: 6;
			uint32_t page_frame			: 20;
		};
	} pde;
	union {
		uint32_t val;
		struct {
			uint32_t present             : 1;
			uint32_t read_write          : 1;
			uint32_t user_supervisor     : 1;
			uint32_t page_write_through  : 1;
			uint32_t page_cache_disable  : 1;
			uint32_t accessed            : 1;
			uint32_t dirty               : 1;
			uint32_t pad0                : 1;
			uint32_t global              : 1;
			uint32_t pad1                : 3;
			uint32_t page_frame          : 20;
		};
	} pte;
	
	temp.val = addr;
	if (temp.idx_1 == 127) {
		Log("eip == %#-8x\n", cpu.eip);
		Log("addr = %#-8x\n", addr);
	}
//	Log("temp.val = %#-8x	idx_1 = %#-8x	idx_2 = %#-8x\n", temp.val, temp.idx_1, temp.idx_2);
//	Log("cpu.cr3 = %#-8x\n", cpu.cr3 & ~PAGE_OFFSET_MASK);

	pde.val = hwaddr_read((cpu.cr3 & ~PAGE_OFFSET_MASK) + temp.idx_1 * 4, 4);
	Assert(pde.present, "this page director entry is not presented!\n\
			addr = %#-8x  %%cr3=%#x  mask = %#x  %%cr3 & mask = %#x idx = %d",\
		   	addr, cpu.cr3, ~PAGE_OFFSET_MASK, cpu.cr3 & ~PAGE_OFFSET_MASK, temp.idx_1);


	pte.val = hwaddr_read((pde.val & ~PAGE_OFFSET_MASK) + temp.idx_2 * 4, 4);
	Assert(pte.present, "this page table entry is not presented!");

	return ((pte.page_frame << 12) + temp.offset);
//	Log("temp.idx_2 = %#-8x\n", temp.idx_2);
//	Log("pte = %#-8x\n", pte.page_frame);
	
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
		uint32_t offset = hwaddr & PAGE_OFFSET_MASK;
		Assert(offset + len <= PAGE_LEN, "Cross the page boundary!\n");
	} else 
		hwaddr = addr;
		
	return hwaddr_read(hwaddr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data)
{
	hwaddr_t hwaddr;
	if (cpu._cr0._pg) {
		hwaddr = page_translate(addr);
		uint32_t offset = hwaddr & PAGE_OFFSET_MASK;
		Assert(offset + len <= PAGE_LEN, "Cross the page boundary!\n");
	} else
		hwaddr = addr;
	hwaddr_write(hwaddr, len, data);
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


