#ifndef COMMON_ENUMS_DECL_H
#define COMMON_ENUMS_DECL_H

/**********************************************************************/
/* ���� ������ */
enum tok_types {DELIMITER,  // ����� ���������� � ���������
				IDENTIFIER, // ����� ���������� � �������
				NUMBER,     // �������� ���������
				KEYWORD,    // ����������������� ����� (while f.e.)
                TEMP,		// ��������������� ��� ������ ��� �����������
							// �������� ����� KEYWORD ��� IDENTIFIER
				STRING,		// ������
				BLOCK};		// { ��� }
				

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


#endif // COMMON_ENUMS_DECL_H