#include <stdlib.h>
#include "common.h"
#include "memory/cache.h"
cache_s cache;

uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);
int ddr3_read(hwaddr_t, void*);
static void cache_read_internel(hwaddr_t, void*);

void init_cache()
{
	cache.read = cache_read;
	cache.write = cache_write;
	int i, j;
	for (i = 0; i < 1024; i++)
		for (j = 0; j < 8; j++)
			cache.blk[i].way[j].valid = 0;
}

uint32_t cache_read(hwaddr_t addr, size_t len)
{
	uint32_t offset = addr & BLK_MASK;
	uint8_t temp[2 * BLK_LEN];
	cache_read_internel(addr, temp);
	if (offset + len > BLK_LEN) { 
		cache_read_internel(addr + BLK_LEN, temp + BLK_LEN);
	}
	return unalign_rw(temp + offset, 4);
}

static void cache_read_internel(hwaddr_t addr, void *data)
{
	addr &= ~BLK_MASK;
	uint32_t blk_index = (addr & BLK_INDEX_MASK) >> 3;
	uint32_t tag = (addr & TAG_MASK) >> 13;

	int i;
	/* search cache */
	for (i = 0; i < 8; i++) 
		if (cache.blk[blk_index].way[i].valid \
			&& cache.blk[blk_index].way[i].tag == tag) {
			memcpy(data, cache.blk[blk_index].way[i].data, BLK_LEN);
			return;
		}
	/* cache miss */
	i = rand() % 8;
	ddr3_read(addr, data);
	cache.blk[blk_index].way[i].valid = 1;
	cache.blk[blk_index].way[i].tag = tag;
	memcpy(cache.blk[blk_index].way[i].data, data, BLK_LEN);
	return;
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data)
{
	return;
}
