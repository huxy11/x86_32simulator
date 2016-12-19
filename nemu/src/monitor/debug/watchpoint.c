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
WP* new_wp(char *e)
{
	WP **ins;
	bool success;
	for (ins = &head; *ins; ins = &(*ins)->next);
	*ins = free_;
	if (free_->next)
		free_ = free_->next;
	else panic("Not enough watch points");
	(*ins)->next = NULL;
	strcpy((*ins)->expr, e);
	(*ins)->old = expr(e, &success);
	Assert(success, "Watch point expr failed!");
	return *ins;
}

void del_wp(char *e)
{
	WP **del;
	for (del = &head; *del;)
		if (!strcmp((*del)->expr, e)) {
			WP *wp = *del;
			*del = wp->next;
			wp->next = free_;
			free_ = wp;
			break;
		}
		else del = &(*del)->next;
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
		printf("\t%d:%s=%d", (*node)->NO, (*node)->expr, (*node)->old);
	printf("\nfree:");
	for (node = &free_; *node; node = &(*node)->next)
		printf("\t%d:%s=%d", (*node)->NO, (*node)->expr, (*node)->old);
	printf("\n");
}

bool check_wp(void)
{
	bool success, re = false;
	WP *node;
	int tmp;
	for (node = head; node; node = node->next) {
		tmp = expr(node->expr, &success);
		if (tmp != node->old) {
			printf("expression:%s has been changed\n", node->expr);
			node->old = tmp;
			re = true;
		}
	}
	return re;
}
