#if !defined(S21_POLISH_H)
#define S21_POLISH_H

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <malloc.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define ERROR 1
#define TEST_EPS 1e-07

#define N_SIN 'A'
#define N_COS 'B'
#define N_TAN 'C'
#define N_ACOS 'D'
#define N_ASIN 'E'
#define N_ATAN 'F'
#define N_SQRT 'G'
#define N_LN 'H'
#define N_LOG 'I'
#define N_MINUS '-'
#define N_PLUS '+'
#define N_MUL '*'
#define N_DIV '/'
#define N_POW '^'
#define N_MOD '%'
#define N_LBRACKET '('
#define N_RBRACKET ')'
#define N_UMIN 'M'

enum token_type {
  TOKEN_TYPE_OPERATOR,
  TOKEN_TYPE_OPERAND,
};

struct token {
  enum token_type type;
  union {
    char tkn_operator;
    double tkn_operand;
  } data;
};

struct node {
  struct token value;
  struct node* next;
};

struct calculator_state {
  struct node* tkn_operands;
  struct node* tkn_operators;
  unsigned int length_all;
  bool tkn_error;
  bool op_allowed;
  unsigned int func_on;
  unsigned int bracket_num;
};

void push(struct node** head, struct token data);
struct token pop(struct node** head);
void pop_no_value(struct node** head);
struct token* str_to_token(struct calculator_state* state,
                           const char* source_str, double x_value);
void rm_spaces(char* str);
double token_to_double(struct calculator_state* state, struct token* token_str);
void init_state(struct calculator_state* state);
double raw_calculate(char* source_str, struct calculator_state* state);
double raw_calculate_with_x(char* source_str, struct calculator_state* state,
                            double x_value);
void free_all(struct calculator_state* state, struct token* token_str);

#endif  // S21_POLISH_H
