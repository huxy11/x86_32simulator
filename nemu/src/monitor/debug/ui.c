#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "myfunc.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
CPU_state cpu;


/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_help(char *args);
static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}
static int cmd_q(char *args) {
	return -1;
}
//TODO:implement multiple executions
static int cmd_n(char *args) {
	cpu_exec(1);
	return 0;
}
static int cmd_info(char *args) {
	int i = 0;
	for (;i < 8; i++) 
		printf("%s:%#-8x\t", regsl[i], cpu.gpr[i]._32);
	printf("\n");
	for (i = 0; i < 8; i++)
		printf("%s:%#-8x\t", regsw[i], cpu.gpr[i]._16);
	printf("\n");
	for (i = 0; i < 8; i++)
		printf("%s:%#-8x\t", regsb[i], reg_b(i));
	printf("\n");
	return 0;
} 
static int cmd_x(char *args) {
	char *str;
	/* extract count */
	str = strtok(args, " ");
	if (!str) {
		printf("Wrong input arguments!\n");
		return 0;
	}
	int cnt = atoi(str);
	printf("%d\n", cnt);
	/* extract address */
	str = strtok(NULL, " ");
	if (!str) {
		printf("Wrong input arguments!\n");
		return 0;
	}int add = htoi(str);
	printf("add:%x\n", add);

	for(; cnt >= 0; cnt--, add++) 
		printf("add:%x->val:%x\n", add, swaddr_read(add, 4));
	return 0;	
	add = 0x10001f;
	int re;
	re = swaddr_read(add, cnt);
	printf("%x\n", re);
	cnt = 2;
	re = swaddr_read(add, cnt);
	printf("%x\n", re);
	cnt = 1;
	re = swaddr_read(add, cnt);
	printf("%x\n", re);
	return 0;
}
static int cmd_test(char *args){
	WP *wp;
	init_wp_pool();
	show_all_wp();
	wp = new_wp();
	printf("wp->NO = %d\n", wp->NO);
	wp = new_wp();
	printf("wp->NO = %d\n", wp->NO);
	wp = new_wp();
	printf("wp->NO = %d\n", wp->NO);
	show_all_wp();
	free_wp(wp);
	show_all_wp();
	return 0;
}
static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "n", "Next step", cmd_n },
	{ "info", "Show register", cmd_info},
	{ "x", "Examine memory", cmd_x},
	{ "test", "Test examples", cmd_test},
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))
static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;
 
	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}


void ui_mainloop() {
	while(1) {
		char *str = rl_gets();//get and save the cmd to log.txt by readline()
		char *str_end = str + strlen(str);
		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }
		//printf("cmd = %p   %s\n", cmd, cmd);
		//printf("str = %p   %s\n", str, str);

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif
		//
		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
