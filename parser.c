#include <stdio.h>
#include <stdlib.h>

#include "dxf.h"

typedef enum {
	Main,
	Section,
	EndSec,
	Class,
	Table,
	EndTab,
	Block,
	EndBlk,
	Arc,
	Line,
	Vertex,
	MaxState
} state_t;

typedef int (*state_action_f) (int act, char *str);

typedef struct {
	state_action_f action;
	state_t state;
	const char *state_name;
} state_machine_t;

static int main_control (int act, char *str);
static int section_control (int act, char *str);
static int class_control (int act, char *str);
static int table_control (int act, char *str);
static int block_control (int act, char *str);
static int arc_control (int act, char *str);
static int line_control (int act, char *str);
static int vertex_control (int act, char *str);

static const char * const names[10] = {
	"SECTION",
	"ENDSEC",
	"CLASS",
	"TABLE",
	"ENDTAB",
	"BLOCK",
	"ENDBLK",
	"ARC",
	"LINE",
	"VERTEX"
};

static const char names0[] = "SECTION";
static const char names1[] = "ENDSEC";
static const char names2[] = "CLASS";
static const char names3[] = "TABLE";
static const char names4[] = "ENDTAB";
static const char names5[] = "BLOCK";
static const char names6[] = "ENDBLK";
static const char names7[] = "ARC";
static const char names8[] = "LINE";
static const char names9[] = "VERTEX";

static state_machine_t states[] = {
	{ main_control, Main, NULL },
	{ section_control, Section, names0 },
	{ NULL, EndSec, names1},
	{ class_control, Class, names2},
	{ table_control, Table, names3},
	{ NULL, EndTab, names4},
	{ block_control, Block, names5},
	{ NULL, EndBlk, names6},
	{ arc_control, Arc, names7},
	{ line_control, Line, names8},
	{ vertex_control, Vertex, names9},
	{ NULL, MaxState, NULL }
};


static state_t state_stack[10];
static int state_stack_idx = 0;
static state_t state_curr = Main;
static state_action_f action_curr = main_control;


static state_t get_state (char *name)
{
	int i;
	for (i = 0; states[i].state != MaxState; i++) {
		if (!strcmp (name, states[i].state_name))
			return states[i].state;
	}
	return MaxState;
}

static state_action_f get_action (state_t state)
{
	int i;
	for (i = 0; states[i].state != state; i++);
	return states[i].action;
}

static void state_push (state_t state)
{
	state_stack[state_stack_idx++] = state;
}
static state_t state_pop (void)
{
	return state_stack[state_stack_idx--];
}


void print_contr (void)
{
	int i;
	for (i = 0; states[i].state != MaxState; i++)
		printf ("%s: %d -- %lx\n", states[i].state_name,
				states[i].state, states[i].action);
}

void dxf_parser (char *fname)
{
	FILE *fp;
	int r, n;
	char s[256];

	print_contr();

	fp = fopen (fname, "r");
	if (fp == NULL) {
		perror ("open file");
//		exit (1);
		return;
	}
	do {
		r = fscanf (fp, "%d\n", &n);
		if (r != EOF) {
			r = fscanf (fp, "%s\n", s);
			r = action_curr (n, s);
			while (r && state_curr != Main) {
				state_curr = state_pop();
				action_curr = get_action (state_curr);
				r = action_curr (n, s);
			}
			if (r) {
				printf ("error parsing file\n");
				return;
			}
		}
	} while (r != EOF);
	fclose (fp);
}

static int main_control (int act, char *str)
{
	state_t st = MaxState;
	if (act == 0)
		st = get_state (str);
	switch (st) {
		case Section:
			state_push (state_curr);
			state_curr = st;
			action_curr = get_action (st);
			printf ("switch to %d -- %lx\n", state_curr, action_curr);
			break;
		default:
			break;
	}
	return 0;
}

static int section_control (int act, char *str)
{
	state_t st = MaxState;
	static int name = 1;
	if (act == 2 && name) {
		printf ("Process section %s\n", str);
		name--;
		return 0;
	} else if (act == 0) {
		st = get_state (str);
	}
	switch (st) {
		case Class:
		case Table:
		case Block:
		case Arc:
		case Line:
		case Vertex:
			state_push (state_curr);
			state_curr = st;
			action_curr = get_action (st);
			printf ("switch to %d -- %lx\n", state_curr, action_curr);
			break;
		case EndSec:
			name++;
			state_curr = state_pop ();
			action_curr = get_action (state_curr);
			printf ("switch to %d -- %lx\n", state_curr, action_curr);
			break;
		default:
			break;
	}
	return 0;
}

static int class_control (int act, char *str)
{
	if (act == 2) {
		printf ("Process class %s\n", str);
		return 0;
	} else if (act == 0) {
		return 1;
	} else {
		return 0;
	}
}

static int table_control (int act, char *str)
{
	state_t st = MaxState;
	if (act == 2) {
		printf ("Process table %s\n", str);
		return 0;
	} else if (act == 0) {
		st = get_state (str);
	}
	switch (st) {
		case EndTab:
			state_curr = state_pop ();
			action_curr = get_action (state_curr);
			break;
		default:
			break;
	}
	return 0;
}


static int block_control (int act, char *str)
{
	state_t st = MaxState;
	if (act == 2) {
		printf ("Process block %s\n", str);
		return 0;
	} else if (act == 0) {
		st = get_state (str);
	}
	switch (st) {
		case EndBlk:
			state_curr = state_pop ();
			action_curr = get_action (state_curr);
			break;
		default:
			break;
	}
	return 0;
}

static int arc_control (int act, char *str)
{
	if (act == 0) {
		printf ("add arc\n");
		return 1;
	} else {
		return 0;
	}
}

static int line_control (int act, char *str)
{
	if (act == 0) {
		printf ("add line\n");
		return 1;
	} else {
		return 0;
	}
}

static int vertex_control (int act, char *str)
{
	if (act == 0) {
		printf ("add vertex\n");
		return 1;
	} else {
		return 0;
	}
}

