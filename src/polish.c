#include "polish.h"

bool is_equal(const struct token *token, const char sym) {
  return token->data.tkn_operator == sym;
}
// char
bool c_is_tkn_operator(const char sym) {
  return sym == N_PLUS || sym == N_MINUS || sym == N_MUL || sym == N_DIV ||
         sym == N_POW || sym == N_MOD;
}
bool c_plus_near_minus(const char sym1, const char sym2) {
  return sym2 != 0 && ((sym1 == N_PLUS && sym2 == N_MINUS) ||
                       (sym1 == N_MINUS && sym2 == N_PLUS));
}
// token
bool t_is_tkn_operator(const struct token *token) {
  return is_equal(token, N_PLUS) || is_equal(token, N_MINUS) ||
         is_equal(token, N_MUL) || is_equal(token, N_DIV) ||
         is_equal(token, N_POW) || is_equal(token, N_MOD) ||
         is_equal(token, N_UMIN);
}
bool t_is_func(const struct token *token) {
  return is_equal(token, N_SIN) || is_equal(token, N_COS) ||
         is_equal(token, N_TAN) || is_equal(token, N_ACOS) ||
         is_equal(token, N_ASIN) || is_equal(token, N_ATAN) ||
         is_equal(token, N_SQRT) || is_equal(token, N_LN) ||
         is_equal(token, N_LOG);
}
bool t_is_lbracket(const struct token *token) {
  return is_equal(token, N_LBRACKET);
}
bool t_is_rbracket(const struct token *token) {
  return is_equal(token, N_RBRACKET);
}
bool t_is_tkn_operand(const struct token *token) {
  return token->type == TOKEN_TYPE_OPERAND;
}

// basic
double do_sum(struct token *x, struct token *y) {
  return x->data.tkn_operand + y->data.tkn_operand;
}
double do_sub(struct token *x, struct token *y) {
  return x->data.tkn_operand - y->data.tkn_operand;
}
double do_mul(struct token *x, struct token *y) {
  return x->data.tkn_operand * y->data.tkn_operand;
}
double do_div(struct token *x, struct token *y) {
  return x->data.tkn_operand / y->data.tkn_operand;
}
double do_pow(struct token *x, struct token *y) {
  return pow(x->data.tkn_operand, y->data.tkn_operand);
}
double do_mod(struct token *x, struct token *y) {
  return fmod(x->data.tkn_operand, y->data.tkn_operand);
}
// advanced
double do_cos(struct token *x) { return cos(x->data.tkn_operand); }
double do_sin(struct token *x) { return sin(x->data.tkn_operand); }
double do_tan(struct token *x) { return tan(x->data.tkn_operand); }
double do_acos(struct token *x) { return acos(x->data.tkn_operand); }
double do_asin(struct token *x) { return asin(x->data.tkn_operand); }
double do_atan(struct token *x) { return atan(x->data.tkn_operand); }
double do_sqrt(struct token *x) { return sqrt(x->data.tkn_operand); }
double do_ln(struct token *x) { return log(x->data.tkn_operand); }
double do_log(struct token *x) { return log10(x->data.tkn_operand); }
// logic
void check_ptr(void *const ptr, const char *str) {
  if (!ptr) {
    perror(str);
    exit(EXIT_FAILURE);
  }
}
void check_bool(const bool value, const char *str) {
  if (value) {
    perror(str);
    exit(EXIT_FAILURE);
  }
}

void push(struct node **head, struct token data) {
  struct node *new = malloc(sizeof *new);
  check_ptr(new, "Error: memory allocation failed");
  new->value = data;
  new->next = *head;
  *head = new;
}
struct token pop(struct node **head) {
  check_ptr(*head, "Error: stack underflow");
  struct node *tmp = *head;
  struct token data = tmp->value;
  *head = tmp->next;
  free(tmp);
  return data;
}
void pop_no_value(struct node **head) {
  check_ptr(*head, "Error: stack underflow");
  struct node *tmp = *head;
  *head = tmp->next;
  free(tmp);
}
void init_state(struct calculator_state *state) {
  state->tkn_operands = NULL;
  state->tkn_operators = NULL;
  state->length_all = 0;
  state->tkn_error = false;
  state->op_allowed = false;
  state->func_on = 0;
  state->bracket_num = 0;
}
bool tkn_operators_no_empty(const struct calculator_state *state) {
  return (state->tkn_operators != NULL);
}
char find_substring(const char *sym, size_t *i, const char *func_name,
                    const int sym_name) {
  char result_char = '\0';
  if (strncmp(sym, func_name, strlen(func_name)) == 0) {
    result_char = (char)sym_name;
    *i += strlen(func_name);
  }
  return result_char;
}
size_t check_func(struct calculator_state *state, const char *source_sym,
                  char *name_func) {
  size_t i = 0;
  *name_func = 0;
  if (*source_sym == 's') {
    *name_func = find_substring(source_sym, &i, "sin", N_SIN);
    if (!*name_func)
      *name_func = find_substring(source_sym, &i, "sqrt", N_SQRT);
  } else if (*source_sym == 'c') {
    *name_func = find_substring(source_sym, &i, "cos", N_COS);
  } else if (*source_sym == 't') {
    *name_func = find_substring(source_sym, &i, "tan", N_TAN);
  } else if (*source_sym == 'a') {
    *name_func = find_substring(source_sym, &i, "acos", N_ACOS);
    if (!*name_func)
      *name_func = find_substring(source_sym, &i, "asin", N_ASIN);
    if (!*name_func)
      *name_func = find_substring(source_sym, &i, "atan", N_ATAN);
  } else if (*source_sym == 'l') {
    *name_func = find_substring(source_sym, &i, "ln", N_LN);
    if (!*name_func) *name_func = find_substring(source_sym, &i, "log", N_LOG);
  } else if (*source_sym == 'm') {
    *name_func = find_substring(source_sym, &i, "mod", N_MOD);
  }
  if (*name_func) state->op_allowed = true;
  return i;
}
void rm_spaces(char *str) {
  const char *dup = str;
  do {
    while (isspace(*dup)) ++dup;
  } while ((*str++ = *dup++));
}
int get_priority(struct token *token) {
  int priority = 0;
  switch (token->data.tkn_operator) {
    case N_LBRACKET:
    case N_RBRACKET:
      priority = 1;
      break;
    case N_PLUS:
    case N_MINUS:
      priority = 2;
      break;
    case N_MUL:
    case N_DIV:
    case N_MOD:
      priority = 3;
      break;
    case N_POW:
      priority = 4;
      break;
    case N_UMIN:
      priority = 5;
      break;
    default:
      check_bool(true, "Error: invalid tkn_operator");
  }
  return priority;
}

double eval(struct calculator_state *state, const char op) {
  struct token y = pop(&state->tkn_operands);
  struct token x = {TOKEN_TYPE_OPERATOR, {.tkn_operator = 0}};
  if (op == N_PLUS) {
    x.data.tkn_operand = 0.0;
  }
  if (c_is_tkn_operator(op) && state->tkn_operands != NULL) {
    x = pop(&state->tkn_operands);
  } else if (op != N_MINUS && op != N_PLUS && op != N_SIN && op != N_COS &&
             op != N_TAN && op != N_ACOS && op != N_ASIN && op != N_ATAN &&
             op != N_SQRT && op != N_LN && op != N_LOG && op != N_UMIN) {
    state->tkn_error = true;
  }

  double value = 0.0;
  switch (op) {
    case N_PLUS:
      value = do_sum(&x, &y);
      break;
    case N_MINUS:
      value = do_sub(&x, &y);
      break;
    case N_MUL:
      value = do_mul(&x, &y);
      break;
    case N_DIV:
      if (y.data.tkn_operand == 0)
        state->tkn_error = 1;
      else
        value = do_div(&x, &y);
      break;
    case N_POW:
      value = do_pow(&x, &y);
      break;
    case N_MOD:
      value = do_mod(&x, &y);
      break;
    case N_SIN:
      value = do_sin(&y);
      break;
    case N_COS:
      value = do_cos(&y);
      break;
    case N_TAN:
      value = do_tan(&y);
      break;
    case N_ACOS:
      if (y.data.tkn_operand <= -1 || y.data.tkn_operand >= 1)
        state->tkn_error = 1;
      else
        value = do_acos(&y);
      break;
    case N_ASIN:
      if (y.data.tkn_operand <= -1 || y.data.tkn_operand >= 1)
        state->tkn_error = 1;
      else
        value = do_asin(&y);
      break;
    case N_ATAN:
      value = do_atan(&y);
      break;
    case N_SQRT:
      if (y.data.tkn_operand <= 0)
        state->tkn_error = 1;
      else
        value = do_sqrt(&y);
      break;
    case N_LN:
      if (y.data.tkn_operand <= 0)
        state->tkn_error = 1;
      else
        value = do_ln(&y);
      break;
    case N_LOG:
      if (y.data.tkn_operand <= 0)
        state->tkn_error = 1;
      else
        value = do_log(&y);
      break;
    case N_UMIN:
      value = -y.data.tkn_operand;
      break;
    default:
      check_bool(true, "Error: invalid tkn_operator");
  }
  return value;
}

size_t real_length(struct calculator_state *state, const char *source_str) {
  size_t i = 0, j = 0;
  while (i < strlen(source_str)) {
    if (isdigit(source_str[i])) {
      char *end;
      strtod(&source_str[i], &end);
      check_bool(end == source_str, "Error: invalid expression");
      i += end - (source_str + i);
    } else {
      size_t old_i = i;
      char tmp = 0;
      i += check_func(state, &source_str[i], &tmp);
      if (i == old_i) i++;
    }
    j++;
  }
  return j;
}
bool next_minus_plus(const char *sym) {
  return (*sym != '\0' && (*sym == N_MINUS || *sym == N_PLUS));
}
bool prev_digit_or_lbracket(const char *sym) {
  return (*sym != '\0' && (isdigit(*sym) || *sym == N_RBRACKET));
}

struct token *str_to_token(struct calculator_state *state,
                           const char *source_str, double x_value) {
  state->length_all = real_length(state, source_str);
  struct token *token = NULL;
  if (state->length_all)
    token = (struct token *)calloc(state->length_all, sizeof(*token));
  check_ptr(token, "Error: Invalid source_str");
  if (source_str[0] != '\0' && source_str[0] == N_MINUS) {
    state->op_allowed = true;
  } else if (source_str[0] != '\0' && source_str[0] == N_LBRACKET) {
    state->op_allowed = false;
  }
  for (size_t i = 0, j = 0; source_str[i] != '\0';) {
    char func_name = 0;
    i += check_func(state, &source_str[i], &func_name);

    if (j > 0 && t_is_func(&token[j - 1]) && source_str[i] != N_LBRACKET) {
      state->tkn_error = true;
      break;
    } else if ((state->op_allowed &&
                (func_name || c_is_tkn_operator(source_str[i])))) {
      token[j].type = TOKEN_TYPE_OPERATOR;
      if (func_name) {
        token[j++].data.tkn_operator = func_name;
        state->func_on++;
      } else if ((source_str[i] == N_MINUS) &&
                 (i == 0 || (i > 1 && (source_str[i - 1] == N_LBRACKET)) ||
                  source_str[i + 1] == N_LBRACKET ||
                  (t_is_tkn_operator(&token[j - 1]) &&
                   check_func(state, &source_str[i + 1], &func_name)))) {
        token[j].type = TOKEN_TYPE_OPERATOR;
        if (source_str[i] == N_MINUS) token[j++].data.tkn_operator = N_UMIN;
        i++;
      } else if (c_plus_near_minus(source_str[i], source_str[i + 1])) {
        token[j++].data.tkn_operator = N_MINUS;
        i += 2;
        state->length_all--;
      } else {
        token[j++].data.tkn_operator = source_str[i++];
      }
      if (!next_minus_plus(&source_str[i])) state->op_allowed = false;
    } else if (!state->op_allowed && source_str[i] == N_LBRACKET) {
      token[j].type = TOKEN_TYPE_OPERATOR;
      token[j++].data.tkn_operator = source_str[i++];
      if (next_minus_plus(&source_str[i])) state->op_allowed = true;
      state->bracket_num--;
    } else if (state->op_allowed && source_str[i] == N_RBRACKET) {
      token[j].type = TOKEN_TYPE_OPERATOR;
      token[j++].data.tkn_operator = source_str[i++];
      state->bracket_num++;
      if (state->func_on && i > 2 && prev_digit_or_lbracket(&source_str[i - 2]))
        state->func_on--;
    } else {
      token[j].type = TOKEN_TYPE_OPERAND;
      if (source_str[i] == 'x' || isdigit(source_str[i])) {
        if (state->func_on && j >= 2) {
          state->func_on--;
        }
      }
      if (source_str[i] == 'x') {
        token[j++].data.tkn_operand = x_value;
        i++;
      } else if (isdigit(source_str[i])) {
        char *end;
        token[j++].data.tkn_operand = strtod(&source_str[i], &end);
        if (end == (source_str + i)) {
          state->tkn_error = true;
          break;
        }
        i += end - (source_str + i);
      } else {
        state->tkn_error = true;
        break;
      }
      state->op_allowed = true;
    }
  }
  if (state->bracket_num || state->func_on) state->tkn_error = 1;
  return token;
}
void eval_pop_and_push(struct calculator_state *state) {
  double num = eval(state, pop(&state->tkn_operators).data.tkn_operator);
  struct token result = {TOKEN_TYPE_OPERATOR, {.tkn_operator = 0}};
  result.data.tkn_operand = num;
  push(&state->tkn_operands, result);
}

double token_to_double(struct calculator_state *state,
                       struct token *token_str) {
  check_bool(!state->length_all, "Error: invalid token_str");
  for (size_t i = 0; !state->tkn_error && i < state->length_all; i++) {
    if (t_is_tkn_operand(&token_str[i])) {
      push(&state->tkn_operands, token_str[i]);
    } else if (t_is_lbracket(&token_str[i]) || t_is_func(&token_str[i])) {
      push(&state->tkn_operators, token_str[i]);
    } else if (t_is_tkn_operator(&token_str[i])) {
      while (tkn_operators_no_empty(state) &&
             get_priority(&state->tkn_operators->value) >=
                 get_priority(&token_str[i])) {
        eval_pop_and_push(state);
      }
      push(&state->tkn_operators, token_str[i]);
    } else if (t_is_rbracket(&token_str[i])) {
      while (tkn_operators_no_empty(state) &&
             !t_is_lbracket(&state->tkn_operators->value)) {
        eval_pop_and_push(state);
      }
      pop_no_value(&state->tkn_operators);
      if (state->tkn_operators && t_is_func(&state->tkn_operators->value)) {
        eval_pop_and_push(state);
      }
    } else {
      state->tkn_error = true;  //  invalid tkn_operator
      break;
    }
  }
  if (!state->tkn_error && state->tkn_operators != NULL &&
      (t_is_rbracket(&state->tkn_operators->value) ||
       t_is_lbracket(&state->tkn_operators->value))) {
    state->tkn_error = true;  //  invalid expr
  }
  if (!state->tkn_error && state->tkn_operands && !state->tkn_operands->next &&
      state->tkn_operators) {
    state->tkn_error = true;
  }
  while (!state->tkn_error && tkn_operators_no_empty(state)) {
    eval_pop_and_push(state);
  }
  if (!state->tkn_error &&
      (state->tkn_operands == NULL || state->tkn_operands->next != NULL)) {
    state->tkn_error = true;  //  too many tkn_operands on stack
  }
  double result = 0;
  if (!state->tkn_error) result = state->tkn_operands->value.data.tkn_operand;
  return result;
}
void free_all(struct calculator_state *state, struct token *token_str) {
  free(token_str);
  while (state->tkn_operands) {
    pop_no_value(&state->tkn_operands);
  }
  while (state->tkn_operators) {
    pop_no_value(&state->tkn_operators);
  }
}
double raw_calculate(char *source_str, struct calculator_state *state) {
  setlocale(LC_ALL, "en_US.UTF-8");
  init_state(state);
  rm_spaces(source_str);
  struct token *token_str = str_to_token(state, source_str, 0);
  double result = token_to_double(state, token_str);
  free_all(state, token_str);
  return result;
}
double raw_calculate_with_x(char *source_str, struct calculator_state *state,
                            double x_value) {
  setlocale(LC_ALL, "en_US.UTF-8");
  init_state(state);
  rm_spaces(source_str);
  struct token *token_str = str_to_token(state, source_str, x_value);
  double result = token_to_double(state, token_str);
  free_all(state, token_str);
  return result;
}
