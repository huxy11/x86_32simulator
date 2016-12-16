#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	char expr[32];

	/* TODO: Add more members if necessary */


} WP;
WP* new_wp(void);
void free_wp(WP* wp);
void show_all_wp(void);
void init_wp_pool(void);
#endif
