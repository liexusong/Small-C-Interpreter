#ifndef EXTERN_VAR_DECL_H
#define EXTERN_VAR_DECL_H

#include <setjmp.h>

extern char token_type; /* �������� ��� ������� */
extern char tok;		/* ���������� ������������� ������� */
extern char *prog;		/* ������� ��������� � �������� ������ ��������� */
extern char token[80];	/* ��������� ������������� ������� */

extern char *p_buf;		/* ��������� �� ������ ������ ��������� */
extern jmp_buf e_buf;   /* �������� ������ ��� longjmp() */

#endif //EXTERN_VAR_DECL_H