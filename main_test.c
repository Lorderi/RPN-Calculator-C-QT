#include <check.h>

#include "src/polish.h"

bool is_equal_tokens(const struct token x, const struct token y) {
  bool equal_flag = OK;
  if (x.type != y.type) {
    equal_flag = ERROR;
  }
  if (!equal_flag) {
    if (x.type == TOKEN_TYPE_OPERATOR &&
        x.data.tkn_operator != y.data.tkn_operator) {
      equal_flag = ERROR;
    } else if (x.type == TOKEN_TYPE_OPERAND &&
               x.data.tkn_operand != y.data.tkn_operand) {
      equal_flag = ERROR;
    }
  }
  return equal_flag;
}

////|||||||||||||||||||||||   PARSER     |||||||||||||||||||||||||||
START_TEST(parser_01) {
  char source_str[] = "-3 + 2 * 4 ^ 2";
  struct calculator_state state;
  init_state(&state);
  rm_spaces(source_str);
  struct token *token_str = str_to_token(&state, source_str, 0);

  struct token result_array[8] = {
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_UMIN}},
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 3}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_PLUS}},
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 2}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_MUL}},
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 4}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_POW}},
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 2}}};

  ck_assert_uint_eq(8, state.length_all);
  for (size_t i = 0; i < state.length_all; i++) {
    size_t res = is_equal_tokens(result_array[i], token_str[i]);
    ck_assert_uint_eq(OK, res);
  }
  free_all(&state, token_str);
}
END_TEST

START_TEST(parser_02) {
  char source_str[] = "cos((-3 + 2) * 4 ^ 2)";
  struct calculator_state state;
  init_state(&state);
  rm_spaces(source_str);
  struct token *token_str = str_to_token(&state, source_str, 0);

  struct token result_array[13] = {
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_COS}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_LBRACKET}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_LBRACKET}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_UMIN}},
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 3}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_PLUS}},
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 2}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_RBRACKET}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_MUL}},
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 4}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_POW}},
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 2}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_RBRACKET}}};

  ck_assert_uint_eq(13, state.length_all);
  for (size_t i = 0; i < state.length_all; i++) {
    size_t res = is_equal_tokens(result_array[i], token_str[i]);
    ck_assert_uint_eq(OK, res);
  }
  free_all(&state, token_str);
}
END_TEST

START_TEST(parser_03) {
  char source_str[] = "4 ^ 2 - 3 mod 4 - tan(0.444)";
  struct calculator_state state;
  init_state(&state);
  rm_spaces(source_str);
  struct token *token_str = str_to_token(&state, source_str, 0);

  struct token result_array[12] = {
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 4}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_POW}},
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 2}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_MINUS}},
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 3}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_MOD}},
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 4}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_MINUS}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_TAN}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_LBRACKET}},
      {TOKEN_TYPE_OPERAND, {.tkn_operand = 0.444}},
      {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_RBRACKET}}};

  ck_assert_uint_eq(12, state.length_all);
  for (size_t i = 0; i < state.length_all; i++) {
    size_t res = is_equal_tokens(result_array[i], token_str[i]);
    ck_assert_uint_eq(OK, res);
  }
  free_all(&state, token_str);
}
END_TEST

START_TEST(parser_fail_01) {
  char source_str[255] = ") + 3";
  struct calculator_state state;
  init_state(&state);
  rm_spaces(source_str);
  struct token *token_str = str_to_token(&state, source_str, 0);
  free_all(&state, token_str);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(parser_fail_02) {
  char source_str[] = "sin-3";
  struct calculator_state state;
  init_state(&state);
  rm_spaces(source_str);
  struct token *token_str = str_to_token(&state, source_str, 0);
  free_all(&state, token_str);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(parser_fail_03) {
  char source_str[] = "2 * 4 + 5 / 2)";
  struct calculator_state state;
  init_state(&state);
  rm_spaces(source_str);
  struct token *token_str = str_to_token(&state, source_str, 0);
  free_all(&state, token_str);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(parser_fail_04) {
  char source_str[] = "2 / ln";
  struct calculator_state state;
  init_state(&state);
  rm_spaces(source_str);
  struct token *token_str = str_to_token(&state, source_str, 0);
  free_all(&state, token_str);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(parser_fail_05) {
  char source_str[] = "2 / ln(";
  struct calculator_state state;
  init_state(&state);
  rm_spaces(source_str);
  struct token *token_str = str_to_token(&state, source_str, 0);
  free_all(&state, token_str);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST
// ////||||||||||||||||||||||   \PARSER     |||||||||||||||||||||||||||

// ////|||||||||||||||||||||||   POLISH   |||||||||||||||||||||||||||
START_TEST(polish_01) {
  char source_str[] = "4 ^ 2 - 3 mod 4 - tan(0.444)";
  struct calculator_state state;
  init_state(&state);
  rm_spaces(source_str);
  struct token *token_str = str_to_token(&state, source_str, 0);

  struct token num_tok1 = {TOKEN_TYPE_OPERAND, {.tkn_operand = 4}};
  struct token pow_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_POW}};
  struct token num_tok2 = {TOKEN_TYPE_OPERAND, {.tkn_operand = 2}};
  struct token minus_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_MINUS}};
  struct token num_tok3 = {TOKEN_TYPE_OPERAND, {.tkn_operand = 3}};
  struct token mod_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_MOD}};
  struct token num_tok4 = {TOKEN_TYPE_OPERAND, {.tkn_operand = 4}};
  struct token tan_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_TAN}};
  struct token lbr_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_LBRACKET}};
  struct token num_tok5 = {TOKEN_TYPE_OPERAND, {.tkn_operand = 0.444}};
  struct token rbr_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_RBRACKET}};

  struct token result_array[12] = {num_tok1, pow_tok, num_tok2, minus_tok,
                                   num_tok3, mod_tok, num_tok4, minus_tok,
                                   tan_tok,  lbr_tok, num_tok5, rbr_tok};

  ck_assert_uint_eq(12, state.length_all);
  for (size_t i = 0; i < state.length_all; i++) {
    size_t res = is_equal_tokens(result_array[i], token_str[i]);
    ck_assert_uint_eq(OK, res);
  }
  free_all(&state, token_str);
}
END_TEST

START_TEST(polish_02) {
  char source_str[] =
      "cos(18.5555) - tan(0.444 + 3 mod 4 * sin(1534-9)) + sin(7)";
  struct calculator_state state;
  init_state(&state);
  rm_spaces(source_str);
  struct token *token_str = str_to_token(&state, source_str, 0);

  struct token cos_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_COS}};
  struct token lbr_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_LBRACKET}};
  struct token num_tok1 = {TOKEN_TYPE_OPERAND, {.tkn_operand = 18.5555}};
  struct token rbr_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_RBRACKET}};
  struct token minus_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_MINUS}};
  struct token tan_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_TAN}};
  struct token num_tok2 = {TOKEN_TYPE_OPERAND, {.tkn_operand = 0.444}};
  struct token plus_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_PLUS}};
  struct token num_tok3 = {TOKEN_TYPE_OPERAND, {.tkn_operand = 3}};
  struct token mod_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_MOD}};
  struct token num_tok4 = {TOKEN_TYPE_OPERAND, {.tkn_operand = 4}};
  struct token mult_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_MUL}};
  struct token sin_tok = {TOKEN_TYPE_OPERATOR, {.tkn_operator = N_SIN}};
  struct token num_tok5 = {TOKEN_TYPE_OPERAND, {.tkn_operand = 1534}};
  struct token num_tok6 = {TOKEN_TYPE_OPERAND, {.tkn_operand = 9}};
  struct token num_tok7 = {TOKEN_TYPE_OPERAND, {.tkn_operand = 7}};

  struct token result_array[25] = {
      cos_tok,  lbr_tok,  num_tok1,  rbr_tok,  minus_tok, tan_tok,  lbr_tok,
      num_tok2, plus_tok, num_tok3,  mod_tok,  num_tok4,  mult_tok, sin_tok,
      lbr_tok,  num_tok5, minus_tok, num_tok6, rbr_tok,   rbr_tok,  plus_tok,
      sin_tok,  lbr_tok,  num_tok7,  rbr_tok};

  ck_assert_uint_eq(25, state.length_all);
  for (size_t i = 0; i < state.length_all; i++) {
    size_t res = is_equal_tokens(result_array[i], token_str[i]);
    ck_assert_uint_eq(OK, res);
  }
  free_all(&state, token_str);
}
END_TEST
////|||||||||||||||||||||||   \POLISH   |||||||||||||||||||||||||||

// ////|||||||||||||||||||||||   EVALUATION   |||||||||||||||||||||||||||
START_TEST(eval_01) {
  char source_str[] = "cos(18.5555) - tan(0.444 + 4 * sin(1534-9)) + 2 ^ 3";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 9.2627617148;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_02) {
  char source_str[] = "(-3 + 2) * 4 ^ 2 - 3 mod 4 - tan(0.444)";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -19.4756763100;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_03) {
  char source_str[] = "(-3 + 2) * 4 ^ 15";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -1073741824;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_04) {
  char source_str[] = "tan(0.444 + 4 * sin(1534-9 + 2 ^ 3))";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 0.0558102435;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_05) {
  char source_str[] = "sqrt(100 + 4 * sin(1534-9 + 2 ^ 3))";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 9.9805687416;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_06) {
  char source_str[] = "(sin(5) * (2 + 4))";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -5.7535456480;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_07) {
  char source_str[] = "(sin(cos(5)))";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 0.2798733508;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_08) {
  char source_str[] = "5 + (3 * 10)";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 35;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_09) {
  char source_str[] = "(5 + 3) * 10";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 80;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_10) {
  char source_str[] = "2 ^ 3";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 8;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_11) {
  char source_str[] = "cos(1) * 2 * sin(6) - ln(5) + (6 / 2)";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 1.0886244116;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_12) {
  char source_str[] = "2 + (2 * sqrt(9))";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 8;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_13) {
  char source_str[] = "-5+-5";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -10;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_14) {
  char source_str[] = "-5+(-5)";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -10;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_15) {
  char source_str[] = "(-5)+-5";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -10;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_16) {
  char source_str[] = "-sqrt(36) + 24 / (4 * 2)";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -3;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_17) {
  char source_str[] = "-sqrt(36) / -sqrt(9)";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 2;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_18) {
  char source_str[] = "2 +(- (10 * 4))";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -38;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_19) {
  char source_str[] = "12 mod 5";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 2;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_20) {
  char source_str[] = "asin(0.5) + acos(0.5) + log(1.5)";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 1.74688758;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_21) {
  char source_str[] = "log(+1.5) - +5";
  struct calculator_state state;
  double value = raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -4.8239087409;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_fail_01) {
  char source_str[] = "* 3";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_02) {
  char source_str[] = "24 + 3 -*- 5";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_03) {
  char source_str[] = "4 -* 5";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_04) {
  char source_str[] = "4 ** 5";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_05) {
  char source_str[] = "4 * cos * 5";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_06) {
  char source_str[] = "4 + (* sqrt(10))";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_07) {
  char source_str[] = "4 cos(2)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_08) {
  char source_str[] = "4 * (2 + 3) cos(54)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_09) {
  char source_str[] = "(22 + 36) 3";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_10) {
  char source_str[] = "2 (3 + 5)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_11) {
  char source_str[] = "(2 * 3)()";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_12) {
  char source_str[] = "2 * (3-)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_13) {
  char source_str[] = "2 * (3*)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_14) {
  char source_str[] = "2 * (3*cos)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_15) {
  char source_str[] = "2 * (3*())";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_16) {
  char source_str[] = "2 * ";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_17) {
  char source_str[] = "2 - ";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_18) {
  char source_str[] = "2 / ";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_19) {
  char source_str[] = "+ / - / *";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_20) {
  char source_str[] = "22 / sin(0)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_22) {
  char source_str[] = "0 ^ -5";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_25) {
  char source_str[] = "asin(2)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_26) {
  char source_str[] = "asin(-3/2)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_27) {
  char source_str[] = "acos(22)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_28) {
  char source_str[] = "acos()";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_29) {
  char source_str[] = "ln(-2)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_30) {
  char source_str[] = "ln(0)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_31) {
  char source_str[] = "log(-2)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_32) {
  char source_str[] = "log(0)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_33) {
  char source_str[] = "sqrt(-4)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST
START_TEST(eval_fail_34) {
  char source_str[] = "sqrt(s+t)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST
START_TEST(eval_fail_35) {
  char source_str[] = "2 *";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST
START_TEST(eval_fail_36) {
  char source_str[] = "2 ^";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_37) {
  char source_str[] = "2 * cos(sin)";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_38) {
  char source_str[] = "2 * 3 mod";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_39) {
  char source_str[] = "2 * atan(sqrt(-10))";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_40) {
  char source_str[] = "2.4.5";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST

START_TEST(eval_fail_41) {
  char source_str[] = "enbwvkhlebfj";
  struct calculator_state state;
  raw_calculate(source_str, &state);
  ck_assert_uint_eq(state.tkn_error, ERROR);
}
END_TEST
// ////|||||||||||||||||||||||   \EVALUATION   |||||||||||||||||||||||||||

////|||||||||||||||||||||||   EVALUATION WITH VAR |||||||||||||||||||||||||||
START_TEST(eval_with_var_01) {
  char source_str[] = "cos(x) - tan(x + 4 * sin(9)) + 2 ^ 3";
  struct calculator_state state;
  double val = 0;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 21.8478189057;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_02) {
  char source_str[] = "(-3 + 2) * 4 ^ 2 - 3 mod 4 - tan(x)";
  struct calculator_state state;
  double val = 0.444;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -19.4756763100;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_03) {
  char source_str[] = "(-3 + 2) * 4 ^ x";
  struct calculator_state state;
  double val = 5;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -1024;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_04) {
  char source_str[] = "tan(0.444 + 4 * sin(x-9 + 2 ^ 3))";
  struct calculator_state state;
  double val = 1534;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 0.0558102435;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_05) {
  char source_str[] = "sqrt(x + 4 * sin(1534-9 + 2 ^ 3))";
  struct calculator_state state;
  double val = 100;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 9.9805687416;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_06) {
  char source_str[] = "(sin(5) * (x + 4))";
  struct calculator_state state;
  double val = 2;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -5.7535456480;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST
START_TEST(eval_with_var_07) {
  char source_str[] = "(sin(cos(x)))";
  struct calculator_state state;
  double val = 5;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 0.2798733508;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_08) {
  char source_str[] = "5 + (x * 10)";
  struct calculator_state state;
  double val = 3;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 35;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_09) {
  char source_str[] = "(5 + x) * 10";
  struct calculator_state state;
  double val = 3;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 80;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_10) {
  char source_str[] = "2 ^ x";
  struct calculator_state state;
  double val = 3;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 8;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_11) {
  char source_str[] = "cos(1) * 2 * sin(x) - ln(5) + (x / 2)";
  struct calculator_state state;
  double val = 6;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 1.0886244116;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_12) {
  char source_str[] = "2 + (2 * sqrt(x))";
  struct calculator_state state;
  double val = 9;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 8;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_13) {
  char source_str[] = "-x+-x";
  struct calculator_state state;
  double val = 5;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -10;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_14) {
  char source_str[] = "-x+(-x)";
  struct calculator_state state;
  double val = 5;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -10;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_15) {
  char source_str[] = "(-x)+-x";
  struct calculator_state state;
  double val = 5;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -10;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_16) {
  char source_str[] = "-sqrt(x) + 24 / (4 * 2)";
  struct calculator_state state;
  double val = 36;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -3;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_17) {
  char source_str[] = "-sqrt(x) / -sqrt(9)";
  struct calculator_state state;
  double val = 36;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 2;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_18) {
  char source_str[] = "2 +(-(10 * x))";
  struct calculator_state state;
  double val = 4;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = -38;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_19) {
  char source_str[] = "12 mod x";
  struct calculator_state state;
  double val = 5;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 2;  // ????????????
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_20) {
  char source_str[] = "asin(0.5) + acos(0.5) + log(x)";
  struct calculator_state state;
  double val = 1.5;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result =
      1.7468875858505778613126107001703737245305236824161402340069104753;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST

START_TEST(eval_with_var_21) {
  char source_str[] = "atan(0.5) + 12 mod 5 + log(x)";
  struct calculator_state state;
  double val = 5;
  double value = raw_calculate_with_x(source_str, &state, val);
  ck_assert_uint_eq(state.tkn_error, OK);
  double result = 3.1626176133;
  ck_assert_double_eq_tol(value, result, TEST_EPS);
}
END_TEST
////||||||||||||||||||||||   \EVALUATION WITH VAR   |||||||||||||||||||||||||||

int main() {
  Suite *s1 = suite_create("Core");
  TCase *tc_core = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;
  suite_add_tcase(s1, tc_core);

  ////|||||||||||||||||||||||   PARSER     |||||||||||||||||||||||||||
  tcase_add_test(tc_core, parser_01);
  tcase_add_test(tc_core, parser_02);
  tcase_add_test(tc_core, parser_03);
  tcase_add_test(tc_core, parser_fail_01);
  tcase_add_test(tc_core, parser_fail_02);
  tcase_add_test(tc_core, parser_fail_03);
  tcase_add_test(tc_core, parser_fail_04);
  tcase_add_test(tc_core, parser_fail_05);
  // ////||||||||||||||||||||||   \PARSER     |||||||||||||||||||||||||||

  // ////|||||||||||||||||||||||   POLISH   |||||||||||||||||||||||||||
  tcase_add_test(tc_core, polish_01);
  tcase_add_test(tc_core, polish_02);
  ////|||||||||||||||||||||||   \POLISH   |||||||||||||||||||||||||||

  // ////|||||||||||||||||||||||   EVALUATION   |||||||||||||||||||||||||||
  tcase_add_test(tc_core, eval_01);
  tcase_add_test(tc_core, eval_02);
  tcase_add_test(tc_core, eval_03);
  tcase_add_test(tc_core, eval_04);
  tcase_add_test(tc_core, eval_05);
  tcase_add_test(tc_core, eval_06);
  tcase_add_test(tc_core, eval_07);
  tcase_add_test(tc_core, eval_08);
  tcase_add_test(tc_core, eval_09);
  tcase_add_test(tc_core, eval_10);
  tcase_add_test(tc_core, eval_11);
  tcase_add_test(tc_core, eval_12);
  tcase_add_test(tc_core, eval_13);
  tcase_add_test(tc_core, eval_14);
  tcase_add_test(tc_core, eval_15);
  tcase_add_test(tc_core, eval_16);
  tcase_add_test(tc_core, eval_17);
  tcase_add_test(tc_core, eval_18);
  tcase_add_test(tc_core, eval_19);
  tcase_add_test(tc_core, eval_20);
  tcase_add_test(tc_core, eval_21);

  tcase_add_test(tc_core, eval_fail_01);
  tcase_add_test(tc_core, eval_fail_02);
  tcase_add_test(tc_core, eval_fail_03);
  tcase_add_test(tc_core, eval_fail_04);
  tcase_add_test(tc_core, eval_fail_05);
  tcase_add_test(tc_core, eval_fail_06);
  tcase_add_test(tc_core, eval_fail_07);
  tcase_add_test(tc_core, eval_fail_08);
  tcase_add_test(tc_core, eval_fail_09);
  tcase_add_test(tc_core, eval_fail_10);
  tcase_add_test(tc_core, eval_fail_11);
  tcase_add_test(tc_core, eval_fail_12);
  tcase_add_test(tc_core, eval_fail_13);
  tcase_add_test(tc_core, eval_fail_14);
  tcase_add_test(tc_core, eval_fail_15);
  tcase_add_test(tc_core, eval_fail_16);
  tcase_add_test(tc_core, eval_fail_17);
  tcase_add_test(tc_core, eval_fail_18);
  tcase_add_test(tc_core, eval_fail_19);
  tcase_add_test(tc_core, eval_fail_20);
  tcase_add_test(tc_core, eval_fail_22);
  tcase_add_test(tc_core, eval_fail_25);
  tcase_add_test(tc_core, eval_fail_26);
  tcase_add_test(tc_core, eval_fail_27);
  tcase_add_test(tc_core, eval_fail_28);
  tcase_add_test(tc_core, eval_fail_29);
  tcase_add_test(tc_core, eval_fail_30);
  tcase_add_test(tc_core, eval_fail_31);
  tcase_add_test(tc_core, eval_fail_32);
  tcase_add_test(tc_core, eval_fail_33);
  tcase_add_test(tc_core, eval_fail_34);
  tcase_add_test(tc_core, eval_fail_35);
  tcase_add_test(tc_core, eval_fail_36);
  tcase_add_test(tc_core, eval_fail_37);
  tcase_add_test(tc_core, eval_fail_38);
  tcase_add_test(tc_core, eval_fail_39);
  tcase_add_test(tc_core, eval_fail_40);
  tcase_add_test(tc_core, eval_fail_41);
  // ////|||||||||||||||||||||||   \EVALUATION   |||||||||||||||||||||||||||

  // ////|||||||||||||||||||||||   EVALUATION WITH VAR
  // ||||||||||||||||||||||||||| tcase_add_test(tc_core, eval_with_var_01);
  tcase_add_test(tc_core, eval_with_var_01);
  tcase_add_test(tc_core, eval_with_var_02);
  tcase_add_test(tc_core, eval_with_var_03);
  tcase_add_test(tc_core, eval_with_var_04);
  tcase_add_test(tc_core, eval_with_var_05);
  tcase_add_test(tc_core, eval_with_var_06);
  tcase_add_test(tc_core, eval_with_var_07);
  tcase_add_test(tc_core, eval_with_var_08);
  tcase_add_test(tc_core, eval_with_var_09);
  tcase_add_test(tc_core, eval_with_var_10);
  tcase_add_test(tc_core, eval_with_var_11);
  tcase_add_test(tc_core, eval_with_var_12);
  tcase_add_test(tc_core, eval_with_var_13);
  tcase_add_test(tc_core, eval_with_var_14);
  tcase_add_test(tc_core, eval_with_var_15);
  tcase_add_test(tc_core, eval_with_var_16);
  tcase_add_test(tc_core, eval_with_var_17);
  tcase_add_test(tc_core, eval_with_var_18);
  tcase_add_test(tc_core, eval_with_var_19);
  tcase_add_test(tc_core, eval_with_var_20);
  tcase_add_test(tc_core, eval_with_var_21);
  ////||||||||||||||||||||||   \EVALUATION WITH VAR
  // |||||||||||||||||||||||||||

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf ? 1 : 0;
}
