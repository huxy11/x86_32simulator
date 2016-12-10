#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
	NOTYPE = 256, EQ, NUM

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},						// equal
	{"\\-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"\\(", '('},
	{"\\)", ')'},
	{"[0-9]+", NUM},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

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

int eval(int p,int q);

bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;//store start end position
	
	nr_token = -1;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				/* match a rule, extract the substr */
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
				char *substr = malloc(substr_len+1);
				strncpy(substr, substr_start, substr_len);
				substr[substr_len] = 0;
				Log("str = %s substr = %s\n", e, substr);
				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;
				nr_token++;
				tokens[nr_token].type = rules[i].token_type;
				switch(rules[i].token_type) {
				case NUM:
					strcpy(tokens[nr_token].str, substr); 
					break;
				default: 
					//Log("Nothing have to do for this type:%s\n", substr);
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
	
	//test
	for (i = 0; i <= nr_token; i++)
		Log("tokens[%d] = %d\n", i, tokens[i].type);
	Log("eval = %d\n", eval(0,0));
	return true; 
}
bool check_parentheses(int p, int q) 
{
	/* exam the expression */
	int cnt = 0;
	int i;
	for (i = p; i <= q; i++) {
		if (tokens[i].type == '(')
			cnt ++;
		else if (tokens[i].type == ')')
			cnt --;
		Assert((cnt > 0) || (i == q),"Bad expression(parenthesis), %d, %d", i, q);
	}	
	//if ((tokens[p].type != '(') && (tokens[q].type != ')'))
		return true;
}
int eval(int p, int q)
{
	//int i;
	if (p > q)
		panic("Bad expression!");
	if (p == q)
		return atoi(tokens[p].str); 
	if (check_parentheses(p, q)) {

			}
	return 0; 
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	eval(0, nr_token);

	/* TODO: Insert codes to evaluate the expression. */
	panic("please implement me");
	return 0;
}

