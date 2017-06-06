#ifndef __CACHE_H__
#define __CACHE_H__

//#define BLK_NR_XPNT 10	
	//size(cache block)=64B size(cache)=64KB 
	//so there shall be 1024 cache blocks
//#define BLK_OFFSET_BIT 6
	//size(cache block)=64B  2^6=64  so BLK_OFFSET_BIT = 6

/*
 * addr |-----7bits-----|--------19bits--------|---6bits---|
 *			determine set	      tag				offset
 *
 */

#define BLK_LEN 64
#define BLK_MASK (BLK_LEN - 1)
#define BLK_INDEX_MASK 0x1FC0
#define TAG_MASK 0xFFFFE000
#define OFFSET_MASK 0x3F

// define line of cache
typedef struct way_s {
	uint32_t tag:19;//32 - BLK_NR_XPNT - BLK_OFFSET_BIT;
	uint32_t valid:1;
	uint8_t data[64];
}way_s;

//define set of chache, whick comprise 8 lines
typedef struct cache_blk_s {
	way_s way[8];
}cache_blk_s;

/* cache obj */
typedef struct cache_s {
	cache_blk_s blk[128];//128 sets
	/* Methods */
	uint32_t (*read)(hwaddr_t, size_t);
	void (*write)(hwaddr_t, size_t, uint32_t);
}cache_s;

extern cache_s cache;
#endif
