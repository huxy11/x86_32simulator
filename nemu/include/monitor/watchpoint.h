#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	int old;
	char expr[32];
} WP;
WP* new_wp(char *e);
void free_wp(WP* wp);
void del_wp(char* e);
void show_all_wp(void);
void init_wp_pool(void);
bool check_wp(void);
#endif
