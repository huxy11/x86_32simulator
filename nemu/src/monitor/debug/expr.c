#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
static bool _success = true;

enum {
	NOTYPE = 256, EQ, NUM, NEG, DRF, HEX, REG, VAR, MEM
};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {
	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},						// equal
	{"\\-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"\\(", '('},
	{"\\)", ')'},
	{"0[xX][0-9a-fA-F]+", HEX}, 
	{"\\$[a-z0-9]+", REG},
	{"[0-9]+", NUM},
	{"[mM]\\:0[xX][0-9a-fA-F]+", MEM},
	{"[a-zA-Z_][a-zA-Z0-9]*", VAR},
	{"", NEG},
	{"", DRF},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) ) - 2

static regex_t re[NR_REGEX];
/* Rules are used for many times.
 * Therefore we compile them(by using regcomp) only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static int eval(int p,int q);

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;//store start end position
	
	nr_token = -1;
	while(e[position] != '\0') {
		//Log("NR_REGEX = %d\n", NR_REGEX);
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				/* match a rule, extract the substr */
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
				//Log("%s match rules[%d] = \"%s\" at position %d with len %d: %.*s", e, i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;
				if (rules[i].token_type == NOTYPE)
					break;
				/* detect the negative */
				if ((rules[i].token_type == '-') && \
					(tokens[nr_token].type != REG) && \
					(tokens[nr_token].type != HEX) && \
					(tokens[nr_token].type != NUM) && \
					(tokens[nr_token].type != ')'))
					tokens[++nr_token].type = NEG;
				/* detect the dereference */
				else if ((rules[i].token_type == '*') && \
						(tokens[nr_token].type != REG) && \
						(tokens[nr_token].type != HEX) && \
						(tokens[nr_token].type != NUM) && \
						(tokens[nr_token].type != ')'))
					tokens[++nr_token].type = DRF;	
				else tokens[++nr_token].type = rules[i].token_type;
				switch(rules[i].token_type) {
				case VAR:
				case NUM:
				case MEM:
				case HEX:
				case REG:
					strncpy(tokens[nr_token].str, substr_start, substr_len);
					tokens[nr_token].str[substr_len] = 0;
					break;
				default: 
					//Warn("Nothing have to do for this type:%s\n", substr);
					break;
				}
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	//for (i = 0; i <= nr_token; i++) 
	//	Log("%d ",tokens[i].type);
	return true; 
}
static bool check_parentheses(int p, int q) 
{
	/* exam the parentheses */
	int cnt = 0;
	int i = p;
	bool re = true;
	for (; i <= q; i++) {
		if (tokens[i].type == '(') 
			cnt++;
		else if (tokens[i].type == ')')
			cnt--;
		Assert(cnt >= 0, "Bad expression:parentheses can't match");
		if ((cnt == 0) && (i != q))	
			re = false;
	}
	return re;
	/* exam the expression */
	if (tokens[p].type == '('){
		int cnt = 1;
		int i = p + 1;
		for (; i <= q; i++) {
			if (tokens[i].type == '(')
				cnt ++;
			else if (tokens[i].type == ')')
				cnt --;
		Assert((cnt > 0) || (i == q),"Bad expression:parenthesis can't match"); 
		}	
		return true;//surrounded by parentheses
	} else {
		int i = p;
		int cnt = 0;
		for (; i <= q ; i++) {
			if (tokens[i].type == '(')
				cnt ++;
			else if (tokens[i].type == ')')
				cnt --;
		Assert(cnt >= 0,"Bad expression:parenthesis can't match"); 
		}
		//Assert((tokens[i].type != '(') && (tokens[i].type != ')'), "Bad expression:parenthesis not show first"); 
		return false;//not surrounded by parentheses
	}
	//if ((tokens[p].type != '(') && (tokens[q].type != ')'))
}
static int eval(int p, int q)
{
	int level = -1;
	if (p > q)
		panic("Bad expression:positon p > q");
	if (p == q) {
		switch (tokens[p].type) {
		case NUM:
			return atoi(tokens[p].str);
		case HEX:
			return htoi(tokens[p].str);
		case MEM: {
			uint32_t addr = htoi(tokens[p].str + 2);
			return swaddr_read(addr, 1, 3);
		}
		case REG:
			return regtoi(tokens[p].str);
		case VAR: {
			int _re = sym_eval(tokens[p].str);
			if (!_re)
				_success = false;
			return swaddr_read(_re, 4, 3);
			}
		}
		if (tokens[p].type == NUM)
			return atoi(tokens[p].str); 
		else panic("Bad expression no-NUM token left");
	}
	if (check_parentheses(p, q)) {
		/* throw the parentheses away */
		p++;
		q--;
	}
	/* locate the dominant operator */
	int r = 0,dmnt_op = 0, i = p, cnt;
	for (; i <= q; i++) 
		switch (tokens[i].type) {
		case EQ:
			dmnt_op = tokens[i].type;
			r = i;
			level = 5;
			break;
		case '+':
		case '-':
			dmnt_op = tokens[i].type;
			r = i;
			level = 4;
			break;
		case '*':
		case '/':
			if (level < 4) {
				dmnt_op = tokens[i].type;
				r = i;
				level = 3;
			}
			break;
		case NEG:
		case DRF:
			if (level < 2) {
				dmnt_op = tokens[i].type;
				r = i;
				level =2;
			}
			break;
		case '(':
		   	cnt	= 1;
			for(i++ ; cnt > 0; i++) {
				if (tokens[i].type == '(')
					cnt++;
				else if (tokens[i].type == ')')
					cnt--;
			}
	//		Log("i = %d\n", i);
			i--;
			break;
		}
	//Log("dominant operator is %c, p = %d, q = %d, r = %d\n", dmnt_op, p, q, r);

	switch (dmnt_op) {
	case '+':
		return eval(p, r-1) + eval(r+1, q);
	case '-':
		return eval(p, r-1) - eval(r+1, q);
	case '*':
		return eval(p, r-1) * eval(r+1, q);
	case '/':
		return eval(p, r-1) / eval(r+1, q);
	case NEG:
		return -eval(p + 1, q);	
	case DRF:
		return swaddr_read(eval(p+1, q), 4, 3);
	case EQ:
		return eval(p, r-1) == eval(r+1, q);
	}
	return 0; 
}

uint32_t expr(char *e, bool *success) {
	if (!make_token(e)) {
		*success = false;
		return 0;
	}
	//Log("nr_token = %d\n", nr_token);
	int re = eval(0, nr_token);
	//printf("the result is %d\n", re);
	*success = _success;
	return re;
}

