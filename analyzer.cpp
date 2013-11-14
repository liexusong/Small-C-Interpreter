/* ����������� ���������� �������������� ����������
   ������������� ���������, ���������� ����������
   � ������ �������.
*/
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

/**********************************************************************/
/* ���� ������ */
enum tok_types {DELIMITER,  // ����� ���������� � ���������
				IDENTIFIER, // ����� ���������� � �������
				NUMBER,     // �������� ���������
				KEYWORD,    // ����������������� ����� (while f.e.)
                TEMP,		// ��������������� ��� ������ ��� �����������
							// �������� ����� KEYWORD ��� IDENTIFIER
				STRING,		// ������
				BLOCK,		// { ��� }
				ARRAY};		// [ ��� ]

/* ����������������� ����� */
enum tokens {ARG, 
			CHAR, 
			INT, 
			IF, 
			ELSE, 
			FOR, 
			DO, 
			WHILE,
            SWITCH, 
			RETURN, 
			EOL, 
			FINISHED, 
			END};

/* ��� ��������� ������������ ��� ������ ������� sntx_err()
   � ������ �������������� ������. 
   ��������: ��������� SYNTAX ������������ �����, �����
   ������������� �� ����� ��������������� ������.
*/
enum error_msg
     {SYNTAX, UNBAL_PARENS, NO_EXP, EQUALS_EXPECTED,
      NOT_VAR, PARAM_ERR, SEMI_EXPECTED,
      UNBAL_BRACES, FUNC_UNDEF, TYPE_EXPECTED,
      NEST_FUNC, RET_NOCALL, PAREN_EXPECTED,
      WHILE_EXPECTED, QUOTE_EXPECTED, NOT_TEMP,
      TOO_MANY_LVARS, DIV_BY_ZERO};

enum double_ops {LT=1,  // < 
				 LE,	// <=
				 GT,	// >
				 GE,	// >=
				 EQ,	// == 
				 NE};	// !=

/**********************************************************************/

extern char token_type; /* �������� ��� ������� */
extern char tok;		/* ���������� ������������� ������� */
extern char *prog;		/* ������� ��������� � �������� ������ ��������� */
extern char *p_buf;		/* ��������� �� ������ ������ ��������� */
extern char token[80];	/* ��������� ������������� ������� */
extern jmp_buf e_buf;   /* �������� ������ ��� longjmp() */

int get_token(void);
int look_up(char *s);

/* �������� ������� �� ��������� ������ ��������� 
	� ���������� �� � �������� ������ �������� */
int isdelim(char c), iswhite(char c);
void sntx_err(int error);

/* ������� ����������������� ���� */
extern struct commands {
  char command[20];
  char tok;
} table[];

/* ���������� ������� �� �������� ������. */
int get_token(void)
{
	register char *temp; // ������ ������ ��������

	token_type = 0; 
	tok = 0;
	temp = token;
	*temp = '\0';

// {{ ������� ��������, �������� ��������� � ������ ������
	while(iswhite(*prog) && *prog) ++prog;

	if(*prog == '\r') {
		++prog;
		++prog;
		while(iswhite(*prog) && *prog) ++prog;
	}
// ������� ��������, �������� ��������� � ������ ������ }}

// {{ end of file
	if(*prog == '\0') {
		*token = '\0';
		tok = FINISHED;
		return (token_type = DELIMITER);
	}
// end of file }}

// {{ code block
	if(strchr("{}", *prog)) { /* ����������� ����� */
		*temp = *prog;
		temp++;
		*temp = '\0';
		prog++;
		return (token_type = BLOCK);
	}
// code block }}

// {{ is comment
	if(*prog == '/') {
		if(*(prog+1) == '*') {
			prog += 2;
			do { 
				while(*prog != '*') prog++;
				prog++;
			} while (*prog != '/');
			prog++;
		}
	}
// comment }}

// {{ comp. operator
	if(strchr("!<>=", *prog)) { 
		switch(*prog) {
			case '=': if(*(prog+1) == '=') { // ==
				prog++; prog++;
				*temp = EQ;
				temp++; *temp = EQ; temp++;
				*temp = '\0';
			} break;
			case '!': if(*(prog+1) == '=') { // !=
				prog++; prog++;
				*temp = NE;
				temp++; *temp = NE; temp++;
				*temp = '\0';
			} break;
			case '<': 
				if(*(prog+1) == '=') { // <=
				prog++; prog++;
				*temp = LE; temp++; *temp = LE;
				}
				else {                 // <
					prog++;
					*temp = LT;
				}
				temp++;
				*temp = '\0';
				break;
			case '>': 
				if(*(prog+1) == '=') { // >=
					prog++; prog++;
					*temp = GE; temp++; *temp = GE;
				}
				else {                // >
					prog++;
					*temp = GT;
				}
				temp++;
				*temp = '\0';
				break;
		}
		if(*token) return(token_type = DELIMITER);
	}
// comp. operator}}

// {{ delimiter 
	if(strchr("+-*^/%=;(),'", *prog)){ /* ����������� */
		*temp = *prog;
		prog++; /* ����������� �� ��������� ������� */
		temp++;
		*temp = '\0';
		return (token_type = DELIMITER);
	}
// delimiter }}

// {{ string 
	if(*prog=='"') { /* ������ � �������� */
		prog++;
		while(*prog != '"' && *prog != '\r') *temp++ = *prog++;
		if(*prog == '\r') sntx_err(SYNTAX);
		prog++; *temp = '\0';
		return (token_type = STRING);
  }
// string }}

// {{ number constant
	if(isdigit(*prog)) { /* ����� */
		while(!isdelim(*prog)) *temp++ = *prog++;
		*temp = '\0';
		return (token_type = NUMBER);
	}
// number constant }}

// {{ is an alphabetic letter
	if(isalpha(*prog)) { /* ���������� ��� �������� */
		while(!isdelim(*prog)) *temp++ = *prog++;
		token_type = TEMP;
	}
// is an alphabetic letter }}

	*temp = '\0';

// {{ check if KEYWORD, otherwise identifier 
	if(token_type==TEMP) {
		tok = look_up(token); /* ������������� �� ��������� ������������� */
		if(tok) token_type = KEYWORD; /* ��� ����������������� ����� */
		else token_type = IDENTIFIER;
	}
// }}

	return token_type;
}



/* ����� ��������� �� ������. */
void sntx_err(int error)
{
	char *p, *temp;
	int linecount = 0;
	register int i;

	static char *e[]= {
	"�������������� ������",
	"������������������ ������",
	"��������� �����������",
	"��������� ���� ���������",
	"�� ����������",
	"������ � ���������",
	"��������� ����� � �������",
	"������������������ �������� ������",
	"������� �� ����������",
	"��������� ������������ ����",
	"������� ����� ��������� ������� �������",
	"�������� return ��� �������",
	"��������� ������",
	"��������� while",
	"��������� ������������� �������",
	"�� ������",
	"������� ����� ��������� ����������",
	"������� �� ����"
	};
	printf("\n%s", e[error]);
	p = p_buf;
	while(p != prog) {  /* ����� ������ ������ � ������� */
		p++;
		if(*p == '\r') {
			linecount++;
		}
	}
	printf(" in line %d\n", linecount);

	temp = p;
	for(i=0; i < 20 && p > p_buf && *p != '\n'; i++, p--);
	for(i=0; i < 30 && p <= temp; i++, p++) printf("%c", *p);

	longjmp(e_buf, 1); /* ������� � ���������� ����� */
}

/* ���������� true (������), ���� � - �����������. */
int isdelim(char c)
{
	if(strchr(" !;,+-<>'/*%^=()", c) || c == 9 ||
		c == '\r' || c == 0) return 1;
	return 0;
}

/* ���������� 1, ���� � - ������ ��� ���������. */
int iswhite(char c)
{
	if(c == ' ' || c == '\t') return 1;
	else return 0;
}


/* ����� ����������� ������������� �������
   � ������� ������.
*/
int look_up(char *s)
{
	register int i;
	char *p;

	/* �������������� � ������ ������� */
	p = s;
	while(*p) { *p = tolower(*p); p++; }

	/* ���� �� ������� � �������? */
	for(i=0; *table[i].command; i++) {
		if(!strcmp(table[i].command, s)) return table[i].tok;
	}
	return 0; /* ���������� �������� */
}