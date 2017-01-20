#ifndef __CACHE_H__
#define __CACHE_H__

#define BLK_NR_XPNT 10	//2^10 = 1024
#define BLK_OFFSET_BIT 3	//log2(BLK_SZ/WAY_NR) =  log2(64/8) = 3
#define WAY_NR 8

#define BLK_LEN 8
#define BLK_MASK (BLK_LEN - 1)
#define BLK_INDEX_MASK 0x1FF8 
#define TAG_MASK 0xFFFFE000

typedef struct way_s {
	uint32_t tag:19;//32 - BLK_NR_XPNT - BLK_OFFSET_BIT;
	uint32_t valid:1;
	uint8_t data[8];
}way_s;

typedef struct cache_blk_s {
	way_s way[8];
}cache_blk_s;

/* cache obj */
typedef struct cache_s {
	cache_blk_s blk[1024];
	/* Methods */
	uint32_t (*read)(hwaddr_t, size_t);
	void (*write)(hwaddr_t, size_t, uint32_t);
}cache_s;

extern cache_s cache;

//TODO:maybe this part can be deleted
uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);
#endif
