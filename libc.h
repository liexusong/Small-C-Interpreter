#ifndef LIBC_H
#define LIBC_H

int print(char *s);         /* вывод строки на экран */
//int print(int i);         /* вывод целого числа на экран */
int getnum(void);           /* Считывание целого числа с клавиатуры. */
int call_getche();          /* Считывание символа с консоли. Если компилятор
                              не поддерживает _getche(), то следует
                              использвать getchar() */
int call_put_char();        /* Вывод символа на экран. */
int call_put_string(void);  /* Вызов функции puts(). */

#endif //LIBC_H
