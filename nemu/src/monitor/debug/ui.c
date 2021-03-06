#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "myfunc.h"
#include "memory/cache.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
CPU_state cpu;
typedef struct {
	swaddr_t prev_ebp;
	swaddr_t ret_addr;
	uint32_t *atgs;
}stackFram;


/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() 
{
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

static int cmd_c(char *args) 
{
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) 
{
	return -1;
}

static int cmd_nn(char *args) 
{
	cpu_exec(100);
	return 0;
}

static int cmd_n(char *args) 
{
	if (!args) 
		cpu_exec(1);
	else {
		int cnt = atoi(args);
		cpu_exec(cnt);
	}
		
	return 0;
}

static int cmd_info(char *args) 
{
	int i;
	if(!args) {
		Warn("No info argument!\nw:watchpoints\tr:registers\tf:flags\n");
		return 0;
	}
	switch (args[0]) {
	case 'r':
		printf("%%cr3 = %#x\n", cpu.cr3);
		printf("%%cr0 = %#x\n", cpu.cr0);
		printf("%%eip = %#x\n", cpu.eip);
		printf("\n");
		for (i = 0;i < 8; i++) 
			printf("%%%s:%#-8x%s", regsl[i], cpu.gpr[i]._32,	\
				   	i % 4 == 3 ? "\n" : "\t");
		printf("\n");
		for (i = 0; i < 8; i++)
			printf("%%%s:%#-8x%s", regsw[i], cpu.gpr[i]._16,	\
					i % 4 == 3 ? "\n" : "\t");
		printf("\n");
		for (i = 0; i < 8; i++)
			printf("%%%s:%#-8x%s", regsb[i], reg_b(i),			\
					i % 4 == 3 ? "\n" : "\t");
		printf("\n");
		printf("%%gdtr.base = %#x\tgdtr.lmt = %#x\n", cpu.gdtr.base, cpu.gdtr.lmt);
		for (i = 0; i < 4; i++)
			printf("%%%s:%#-8x\tbase:%#-8x\tlimit:%#-8x\n",	\
					sregs[i], cpu.sreg[i], cpu.sreg_base[i], cpu.sreg_lmt[i]); 		
		printf("\n");
		break;
	case 'w':
		show_all_wp();
		break;
	case 'f':
		printf("EFLAGS = 0x%08x\n", cpu.eflags);
		if (cpu._of) printf("OF is set\n");
		if (cpu._sf) printf("SF is set\n");
		if (cpu._zf) printf("ZF is set\n");
		if (cpu._af) printf("AF is set\n");
		if (cpu._pf) printf("PF is set\n");
		if (cpu._cf) printf("CF is set\n");
		break;
	default:
		Warn("Wrong info argument\n");
	}
	return 0;
} 

static int cmd_x(char *args) 
{
	char *str;
	/* extract count */
	str = strtok(args, " ");
	if (!str) {
		Warn("Wrong input arguments!\n");
		return 0;
	}
	int cnt = atoi(str) - 1;
	/* extract address */
	str = strtok(NULL, " ");
	if (!str) {
		Warn("Wrong input arguments!\n");
		return 0;
	}int add = htoi(str);

	for(; cnt >= 0; cnt--, add++) { 
		printf("add:%x->val:%02x\n", add, swaddr_read(add, 1, 3));
		if (cnt % 4 == 0) 
			printf("\n");
	}
	return 0;	
}


static int cmd_w(char *args) 
{
	new_wp(args);
	return 0;	
}

static int cmd_d(char *args) 
{
	del_wp(args);
	return 0;
}

static int cmd_r(char *args) 
{
	if (!args) {
	}
	return 0;
}

static int cmd_p(char *args) 
{
	char* str = strtok(args, " ");
	if (!str) {
		Warn("Wrong input arguments!\n");
		return 0;
	}
	bool success;
	int re = expr(str, &success);
	if (success)
		printf("%s = %d\t\t%#x\n", str, re, re);
	return 0;
}

static int cmd_bt(char *args) 
{
	Log("cpu.ebp = %#x\n", cpu.ebp);
	Log("cpu.esp = %#x\n", cpu.esp);
	int ebp = cpu.ebp, esp = cpu.esp;
	do {
		for (; ebp != esp; esp += 4) {
			printf("0x%08x|\n%*.s\n", swaddr_read(esp, 4, 2), 10, "-");
		}
		printf("%*.s\n", 10, "+");
		ebp = swaddr_read(ebp, 4, 2);
	} while(ebp);
	return 0;
}

static int cmd_write(char *args) 
{
	char *str = strtok(args, " ");
	if (!str) {
		Warn("Wrong input arguments!\n");
		return 0;
	}
	int cnt = atoi(str);
	str = strtok(NULL, " ");
	if (!str) {
		Warn("Wrong input arguments!\n");
		return 0;
	}
	int add = htoi(str);
	swaddr_write(add, cnt, 0xff);
	return 0;
}
static int cmd_show_cache(char *args)
{
	int i, j;
	cache_addr addr;
	for (i = 0; i < 128; i++)
		for (j = 0; j < 8; j++)
			if (cache.blk[i].way[j].valid) {
				addr.tag = cache.blk[i].way[j].tag;
				addr.idx = i;
				addr.ofs = 0;
				Log("addr = 0x%x", addr.addr);
				uint8_t k;
				for (k = 0; k < 64; k++)
					Log("data = 0x%x", cache.blk[i].way[j].data[k]);
				Log("cache end\n");
			}
	return 0;
}

static int cmd_cache(char *args) 
{
	char *str = strtok(args, " ");
	if (!str) {
		Warn("Wrong input arguments!\n");
		return 0;
	}
	int addr = htoi(str);
	uint32_t blk_index = (addr & BLK_INDEX_MASK) >> 6;
	uint32_t tag = (addr & TAG_MASK) >> 13;
	uint32_t offset = (addr & OFFSET_MASK);
	int i = 0;
	for(;i < 8; i++) {
		if (cache.blk[blk_index].way[i].valid == 1)
			if (cache.blk[blk_index].way[i].tag == tag) {
				Log("cache hit!base = 0x%x offset = %d\n", addr - offset, offset);
				int j = 0,k;
				for (;j < 8;j++) {
					for (k = 0; k < 8; k++) 
						Log("tag = 0x%x : 0x%x  ", cache.blk[blk_index].way[i].tag, cache.blk[blk_index].way[i].data[j*8 + k]);
				}
				return 0;
			}
	}
	Log("cache miss!\n");
	return 0;
	
}

static int cmd_test(char *args) {
	/* colorful print */
	Redp("this is red!\n");
	Greenp("this is green!\n");
	Yellowp("this is yellow!\n");
	Bluep("this is blue\n");
#if 0
	bool success;
	char e[32] = "0xFF + 32 * 7";
	expr(e, &success);	

	char e2[32] = "$al + 1";
	expr(e2, &success);	
	return 0;
#endif
	WP *wp;
	init_wp_pool();
	show_all_wp();
	wp = new_wp("1+213");
	show_all_wp();
	free_wp(wp);
	show_all_wp();
	printf("\n\n");
	wp = new_wp("2+213");
	printf("wp->NO = %d\n", wp->NO);
	wp = new_wp("3+213");
	printf("wp->NO = %d\n", wp->NO);
	show_all_wp();
	del_wp("2+213");
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
	{ "nn", "Execute 100 instruction", cmd_nn },
	{ "n", "Next step", cmd_n },
	{ "", "Next step(if no argu)", cmd_n},
	{ "info", "Show w:watchpoints r:registers f:flags", cmd_info},
	{ "x", "Examine memory", cmd_x},
	{ "w", "Set watch point", cmd_w},
	{ "d", "Delete watch point", cmd_d},
	{ "r", "Check the specified register", cmd_r},
	{ "p", "Print the value", cmd_p},
	{ "bt", "Backtrace stack frame", cmd_bt},
	{ "write", "Write a 0xff to memory", cmd_write},
	{ "showcache", "Show all the cache", cmd_show_cache},
	{ "cache", "Examine cache", cmd_cache},
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
			Greenp("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				Greenp("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
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

		if(i == NR_CMD) { Warn("Unknown command '%s'\n", cmd); }
	}
}
