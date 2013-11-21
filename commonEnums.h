#ifndef COMMON_ENUMS_DECL_H
#define COMMON_ENUMS_DECL_H

/**********************************************************************/
enum tok_types {DELIMITER,  // ����� ���������� � ���������
				IDENTIFIER, // ����� ���������� � �������
				NUMBER,     // �������� ���������
				KEYWORD,    // ����������������� ����� (while f.e.)
                TEMP,		// ��������������� ��� ������ ��� �����������
							// �������� ����� KEYWORD ��� IDENTIFIER
				STRING,		// ������
				BLOCK,		// { or }
				ARRAY};		// [ or ]
				

enum tokens {ARG, 
			CHAR, 
			INT, 
			IF,
			ELSE, 
			FOR, 
			DO, 
			WHILE,
			RETURN, 
			EOL, 
			FINISHED, 
			END};

enum error_msg
     {SYNTAX, UNBAL_PARENS, NO_EXP, EQUALS_EXPECTED,
      NOT_VAR, PARAM_ERR, SEMI_EXPECTED,
      UNBAL_BRACES, FUNC_UNDEF, TYPE_EXPECTED,
      NEST_FUNC, RET_NOCALL, PAREN_EXPECTED,
      WHILE_EXPECTED, QUOTE_EXPECTED, NOT_TEMP,
      TOO_MANY_LVARS, DIV_BY_ZERO,
      ASSIGN_ARRAY_ILLEGAL,INDEX_OUT_OF_BOUNDS,
      NOT_ARR,ARRAY_BRACE_EXPECTED, NUM_EXPECTED};

enum double_ops {LT=1,  // < 
				 LE,	// <=
				 GT,	// >
				 GE,	// >=
				 EQ,	// == 
				 NE};	// !=

/**********************************************************************/


#endif // COMMON_ENUMS_DECL_H