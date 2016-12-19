#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool(void) {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(void)
{
	WP **ins;
	for (ins = &head; *ins; ins = &(*ins)->next);
	*ins = free_;
	if (free_->next)
		free_ = free_->next;
	else panic("Not enough watch points");
	(*ins)->next = NULL;
	memset((*ins)->expr, 0, 32);
	return *ins;
}

void free_wp(WP *wp)
{
	WP **del;
	for (del = &head; *del;)
		if (*del == wp) {
			*del = wp->next;
			wp->next = free_;
			free_ = wp;
			break;
		}
		else del = &(*del)->next;
}

void show_all_wp(void)
{
	WP **node;
	printf("head:");
	for (node = &head; *node; node = &(*node)->next)
		printf("\t%d", (*node)->NO);
	printf("\nfree:");
	for (node = &free_; *node; node = &(*node)->next)
		printf("\t%d", (*node)->NO);
	printf("\n");
}
