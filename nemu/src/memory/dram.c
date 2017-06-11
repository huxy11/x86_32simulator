#include <stdlib.h>
#include "common.h"
#include "memory/burst.h"
#include "misc.h"
#include "memory/cache.h"
void cache_add(uint32_t addr, uint32_t idx, uint32_t tag, uint8_t i);
void cache_read(hwaddr_t addr, void *data);
void cache_write(hwaddr_t addr, uint32_t len, void *data, uint8_t* mask);

/* Simulate the (main) behavor of DRAM.
 * Although this will lower the performace of NEMU, it makes
 * you clear about how DRAM perform read/write operations.
 * Note that cross addressing is not simulated.
 */

#define COL_WIDTH 10
#define ROW_WIDTH 10
#define BANK_WIDTH 3
#define RANK_WIDTH (27 - COL_WIDTH - ROW_WIDTH - BANK_WIDTH)

typedef union {
	struct {
		uint32_t col	: COL_WIDTH;
		uint32_t row	: ROW_WIDTH;
		uint32_t bank	: BANK_WIDTH;
		uint32_t rank	: RANK_WIDTH;
	};
	uint32_t addr;
} dram_addr;



#define NR_COL (1 << COL_WIDTH)
#define NR_ROW (1 << ROW_WIDTH)
#define NR_BANK (1 << BANK_WIDTH)
#define NR_RANK (1 << RANK_WIDTH)

//#define HW_MEM_SIZE (1 << (COL_WIDTH + ROW_WIDTH + BANK_WIDTH + RANK_WIDTH))
#define HW_MEM_SIZE 0xffffffff
//#define HW_MEM_SIZE 0xfffffffff

uint8_t dram[NR_RANK][NR_BANK][NR_ROW][NR_COL];
uint8_t *hw_mem = (void *)dram;

typedef struct {
	uint8_t buf[NR_COL];
	int32_t row_idx;
	bool valid;
} RB;

RB rowbufs[NR_RANK][NR_BANK];

void init_ddr3() {
	int i, j;
	for(i = 0; i < NR_RANK; i ++) {
		for(j = 0; j < NR_BANK; j ++) {
			rowbufs[i][j].valid = false;
		}
	}
}

void ddr3_read(hwaddr_t addr, void *data) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	dram_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t rank = temp.rank;
	uint32_t bank = temp.bank;
	uint32_t row = temp.row;
	uint32_t col = temp.col;

	if(!(rowbufs[rank][bank].valid && rowbufs[rank][bank].row_idx == row) ) {
		/* read a row into row buffer */
		memcpy(rowbufs[rank][bank].buf, dram[rank][bank][row], NR_COL);
		rowbufs[rank][bank].row_idx = row;
		rowbufs[rank][bank].valid = true;
	}

	/* burst read */
	memcpy(data, rowbufs[rank][bank].buf + col, BURST_LEN);
}

void ddr3_write(hwaddr_t addr, void *data, uint8_t *mask) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	dram_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t rank = temp.rank;
	uint32_t bank = temp.bank;
	uint32_t row = temp.row;
	uint32_t col = temp.col;
	
	if (!(rowbufs[rank][bank].valid && rowbufs[rank][bank].row_idx == row) ) {
		/* read a row into row buffer */
		memcpy(rowbufs[rank][bank].buf, dram[rank][bank][row], NR_COL);
		rowbufs[rank][bank].row_idx = row;
		rowbufs[rank][bank].valid = true;
	}

	/* burst write */
	memcpy_with_mask(rowbufs[rank][bank].buf + col, data, BURST_LEN, mask);

	/* write back to dram */
	memcpy(dram[rank][bank][row], rowbufs[rank][bank].buf, NR_COL);
}

uint32_t dram_read(hwaddr_t addr, size_t len) {
	//Log("read in 0x%x\n", addr);

#if 1
	uint32_t cache_offset = addr & CACHE_BURST_MASK;
	/* cache line = 64B */
	uint8_t cache_temp[2 * CACHE_BURST_LEN];
	cache_read(addr, cache_temp);
	if (cache_offset + len > CACHE_BURST_LEN)
	/* data cross the burst boundary */
		cache_read(addr + CACHE_BURST_LEN, cache_temp + CACHE_BURST_LEN);
	return unalign_rw(cache_temp + cache_offset, 4);
#endif

	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	/* ddr3 means BURST_LEN = 8 */
	ddr3_read(addr, temp);
	if (offset + len > BURST_LEN) {
		/* data cross the burst boundary */
		ddr3_read(addr + BURST_LEN, temp + BURST_LEN);
	}
	return unalign_rw(temp + offset, 4);
}

void dram_write(hwaddr_t addr, size_t len, uint32_t data) {
	//Log("write 0x%x into 0x%x\n", data, addr);
#if 1
	uint32_t cache_offset = addr &CACHE_BURST_MASK;
	uint8_t cache_temp[2 * CACHE_BURST_LEN];
	uint8_t cache_mask[2 * CACHE_BURST_LEN];
	memset(cache_temp, 0 ,2 * CACHE_BURST_LEN);
	memset(cache_mask, 0, 2 * CACHE_BURST_LEN);
	memset(cache_mask + cache_offset, 1, len);
	*(uint32_t *)(cache_temp + cache_offset) = data;
	
	int k;
	for (k = 0; k < 128; k++)
		//Log("%d:0x%x 0x%x", k, cache_temp[k], cache_mask[k]);
	
	cache_write(addr, CACHE_BURST_LEN, cache_temp, cache_mask);
	if (cache_offset + len > CACHE_BURST_LEN) {
		cache_write(addr +CACHE_BURST_LEN, CACHE_BURST_LEN, cache_temp + CACHE_BURST_LEN, cache_mask + CACHE_BURST_LEN);
	}
#endif

	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	ddr3_write(addr, temp, mask);

	if(offset + len > BURST_LEN) {
		/* data cross the burst boundary */
		ddr3_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}
}

void cache_read(hwaddr_t addr, void *data)
{
	cache_addr temp;
	temp.addr = addr & ~CACHE_BURST_MASK;
	uint32_t idx = temp.idx;
	uint32_t tag = temp.tag;

	int i;
	for (i = 0; i < 8; i++)
		if (cache.blk[idx].way[i].valid && cache.blk[idx].way[i].tag == tag)
		{
			/*cache hit*/
			memcpy(data, cache.blk[idx].way[i].data, CACHE_BURST_LEN); 
			return;
		}
	/*cache miss*/
	int j = rand() % 8;
	for (i = 0; i < 8; i++)
		if (!cache.blk[idx].way[i].valid)
			j = i;
	cache_add(temp.addr, idx, tag, j);
	memcpy(data, cache.blk[idx].way[j].data, CACHE_BURST_LEN); 
	return;

	for (i = 0; i <8; i++) 
		ddr3_read(temp.addr + BURST_LEN * i, data + BURST_LEN * i);
	/*write into cache*/
	uint8_t cache_mask[2 * CACHE_BURST_LEN];
	memset(cache_mask, 1, 2 * CACHE_BURST_LEN);
	cache_write(temp.addr, CACHE_BURST_LEN, data, cache_mask);
}

void cache_write(hwaddr_t addr, uint32_t len, void *data, uint8_t* mask)
{
	cache_addr temp;
	temp.addr = addr;
	uint32_t idx = temp.idx;
	uint32_t tag = temp.tag;
	//uint32_t ofs = temp.ofs;

	int i;
	for (i = 0; i < 8; i++)
		if (cache.blk[idx].way[i].valid && cache.blk[idx].way[i].tag == tag)
		{
			memcpy_with_mask(cache.blk[idx].way[i].data, data, len, mask);
			return;
		}
	/* addr is not in the cache */
	//Log("addr is not in the cache, add i\n");
	int j = rand() % 8;
	for (i = 0; i < 8; i++)
		if (!cache.blk[idx].way[i].valid)
			j = i;
	cache_add(addr & ~CACHE_BURST_MASK, idx, tag, j);
	//Log("ofs = %d, len = %d\n", ofs, len);
	memcpy_with_mask(cache.blk[idx].way[j].data, data, len, mask);
}

void cache_add(uint32_t addr, uint32_t idx, uint32_t tag, uint8_t i)
{	
	uint8_t temp[CACHE_BURST_LEN];
	int j;
	for (j = 0; j < 8; j++)
		ddr3_read(addr + BURST_LEN * j, temp + BURST_LEN * j);
	memcpy(cache.blk[idx].way[i].data, temp, CACHE_BURST_LEN);
	cache.blk[idx].way[i].valid = true;
	cache.blk[idx].way[i].tag = tag;
}
