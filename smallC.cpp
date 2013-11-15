#include <string.h>
#include <stdlib.h>

#ifndef EXTERN_VAR_DECL_H
#include "externVars.h"
#endif

#ifndef COMMON_ENUMS_DECL_H
#include "commonEnums.h"
#endif

#ifndef RESTRICTIONS_DECL_H
#include "restrictions.h"
#endif

#if DEBUG == 1
#include <iostream>
using namespace std;
#endif


int func_index; /* ������ � ������� ������� */
int gvar_index; /* ������ � ������� ���������� ���������� */
int garr_index; /* ������ � ������� ���������� �������� */

char token_type; /* �������� ��� ������� */
char tok;		/* ���������� ������������� ������� */
char *prog;		/* ������� ��������� � �������� ������ ��������� */
char *p_buf;		/* ��������� �� ������ ������ ��������� */
char token[80];	/* ��������� ������������� ������� */
jmp_buf e_buf;   /* �������� ������ ��� longjmp() */

struct commands { /* ������� ����������������� ���� */
  char command[20];
  char tok;
} table[] = { /* � ��� ������� */
  "if", IF, /* ������� ������ ���� ������� �� ������ ��������. */
  "else", ELSE,
  "for", FOR,
  "do", DO,
  "while", WHILE,
  "char", CHAR,
  "int", INT,
  "return", RETURN,
  "end", END,
  "", END  /* ����� ������� */
};

struct func_type {
  char func_name[ID_LEN];
  int ret_type; 
  char *loc;  /* ����� ����� ����� � ���� */
} func_table[NUM_FUNC];

/* ������ ���� �������� �������� ����������
   � ���������� ����������.
*/
struct var_type {
  char var_name[ID_LEN];
  int v_type;
  int value;
}  global_vars[NUM_GLOBAL_VARS];

// {{* added array functionality
struct array_type {
  char arr_name[ID_LEN];
  int arr_type;
  void *value;
} global_arrays[NUM_GLOBAL_ARRAYS];
// added array functionality *}}


void decl_global(void), sntx_err(int error), putback(void), decl_global_array(void);
int get_token(void);

/* ����� ������ ���� ������� � ���������
   � ��������� ���������� ����������. */
void prescan(void)
{
	char *p, *tp;
	char temp[32];
	int datatype; 
	int brace = 0;  /* ���� brace = 0, �� �������
						������� ��������� ��������� ���������
						� �� �����-���� �������. */

	p = prog;
	func_index = 0;
	do {
		while(brace) {  /* ����� ���� ������� */
			get_token();
			if(*token == '{') brace++;
			if(*token == '}') brace--;
		}

		tp = prog; /* ����������� ������� ������� */
		get_token();

		/* ��� ���������� ���������� ��� ������������� �������� ������� */
		if(tok==CHAR || tok==INT) { 
			datatype = tok; /* ����������� ���� ������ */

			get_token();
			if(token_type == IDENTIFIER) {
				strcpy(temp, token);
				get_token();
				if(*token != '(') { /* ��� ������ ���� ���������� ���������� */
					prog = tp; /* ������� � ������ ���������� */
					decl_global();
				} else if(*token == '(') {  /* ��� ������ ���� ������� */
					func_table[func_index].loc = prog;
					func_table[func_index].ret_type = datatype;
					strcpy(func_table[func_index].func_name, temp);
					func_index++;
					while(*prog != ')') prog++;
					prog++;
					/* ������ prog ��������� �� �������������
						�������� ������ ������� */
				} else {
					putback();
				}
			} else if (token_type == ARRAY) {
				prog = tp;
				decl_global_array();
			}
		} else if(*token == '{') { 
			brace++;
		}
	} while(tok != FINISHED);

	prog = p;
}

// todo: syntx_error informative messages
void decl_global_array(void) 
{
#if DEBUG
	cout << "Declaring global array: " << endl;
#endif

	get_token(); // type
	int arrtype = tok;
	global_arrays[garr_index].arr_type = arrtype;

	get_token(); // [
#if DEBUG
	cout << token << endl;
#endif
	if (*token != '[') sntx_err(SYNTAX);

	// important! comma separated definition of arrays are not supported
	// user must define length of an array in definition
	get_token();
	if (token_type != NUMBER) sntx_err(SYNTAX);
	int arr_length = tok;

	get_token(); // ]
	if (*token != ']') sntx_err(SYNTAX);
    
	// don't be dependent on compiler
	if(arrtype == INT) {
		global_arrays[garr_index].value = malloc(sizeof(int) * arr_length);
	} else if (arrtype == CHAR) {
		global_arrays[garr_index].value = malloc(sizeof(char) * arr_length);
	}

    get_token(); // name
	strcpy(global_arrays[garr_index].arr_name, token);
    
	get_token(); // ;
	if(*token != ';') sntx_err(SEMI_EXPECTED);
	garr_index++;
}

/* ���������� ���������� ����������. */
void decl_global(void)
{
	int vartype;

	get_token();  /* ����������� ���� */

	vartype = tok; /* ����������� ���� ���������� */

	do { /* ��������� ������ */
		global_vars[gvar_index].v_type = vartype;
		global_vars[gvar_index].value = 0;  /* ������������� ����� */
		get_token();  /* ����������� ����� */
		strcpy(global_vars[gvar_index].var_name, token);
		get_token();
		gvar_index++;
	} while(*token == ',');

	if(*token != ';') sntx_err(SEMI_EXPECTED);
}