#include "test.h"

START_TEST(test_1) {
  initAllInfo();
  getState();

  userInput(Start, 0);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Spawn);

  updateCurrentState();
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);

  updateCurrentState();
  userInput(Down, 0);
  userInput(Down, 1);
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);

  updateCurrentState();
  ck_assert_int_eq(*getState(), Attaching);

  userInput(-1, 0);
  userInput(Terminate, 0);
}
END_TEST

START_TEST(test_2) {
  initAllInfo();
  *getState() = StartGame;

  userInput(Start, 0);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Spawn);

  updateCurrentState();
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);

  updateCurrentState();
  userInput(Left, 0);
  userInput(Right, 0);
  userInput(Up, 0);
  userInput(Action, 0);
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);

  userInput(-1, 0);
  userInput(Terminate, 0);
}
END_TEST

START_TEST(test_3) {
  initAllInfo();
  *getState() = StartGame;
  userInput(Start, 0);
  updateCurrentState();

  for (int i = 0; i < 15; i++) {
    ck_assert_int_eq(*getState(), Spawn);

    updateCurrentState();
    ck_assert_int_eq(*getState(), Moving);

    while (*getState() != Shifting) {
      updateCurrentState();
    }
    ck_assert_int_eq(*getState(), Shifting);

    updateCurrentState();
    if (i % 2) {
      for (int toleft = 0; toleft < 6; toleft++) {
        userInput(Left, 0);
      }
    } else {
      for (int toright = 0; toright < 6; toright++) {
        userInput(Right, 0);
      }
    }
    userInput(Down, 0);
    userInput(Down, 1);
    ck_assert_int_eq(*getState(), Moving);

    while (*getState() != Shifting) {
      updateCurrentState();
    }
    ck_assert_int_eq(*getState(), Shifting);

    updateCurrentState();
    ck_assert_int_eq(*getState(), Attaching);
    updateCurrentState();
    *getState() = Spawn;
    sleep(1);
  }
}
END_TEST

START_TEST(test_4) {
  initAllInfo();
  getState();

  *getState() = StartGame;
  AllInfo_t *info = getAllInfo();
  for (int i = WIDTH - 1; i > WIDTH - 2; i--) {
    for (int j = 0; j < HEIGHT; j++) {
      info->field[i][j] = 1;
    }
  }

  userInput(Start, 0);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Spawn);

  updateCurrentState();
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);

  updateCurrentState();
  userInput(Down, 0);
  userInput(Down, 1);
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Attaching);
  updateCurrentState();

  ck_assert_int_eq(0, info->player.level);

  userInput(-1, 0);
  userInput(Terminate, 0);
}
END_TEST

START_TEST(test_5) {
  initAllInfo();
  getState();
  *getState() = StartGame;
  AllInfo_t *info = getAllInfo();
  for (int i = WIDTH - 1; i > WIDTH - 3; i--) {
    for (int j = 0; j < HEIGHT; j++) {
      info->field[i][j] = 1;
    }
  }
  userInput(Start, 0);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Spawn);

  updateCurrentState();
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);

  updateCurrentState();
  userInput(Down, 0);
  userInput(Down, 1);
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Attaching);
  updateCurrentState();

  ck_assert_int_eq(0, info->player.level);

  userInput(-1, 0);
  userInput(Terminate, 0);
}
END_TEST

START_TEST(test_6) {
  initAllInfo();
  getState();
  *getState() = StartGame;
  AllInfo_t *info = getAllInfo();
  for (int i = WIDTH - 1; i > WIDTH - 4; i--) {
    for (int j = 0; j < HEIGHT; j++) {
      info->field[i][j] = 1;
    }
  }
  userInput(Start, 0);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Spawn);

  updateCurrentState();
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);

  updateCurrentState();
  userInput(Down, 0);
  userInput(Down, 1);
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Attaching);
  updateCurrentState();

  ck_assert_int_eq(1, info->player.level);

  userInput(-1, 0);
  userInput(Terminate, 0);
}
END_TEST

START_TEST(test_7) {
  initAllInfo();
  getState();
  *getState() = StartGame;
  AllInfo_t *info = getAllInfo();
  for (int i = WIDTH - 1; i > WIDTH - 5; i--) {
    for (int j = 0; j < HEIGHT; j++) {
      info->field[i][j] = 1;
    }
  }
  userInput(Start, 0);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Spawn);

  updateCurrentState();
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);

  updateCurrentState();
  userInput(Down, 0);
  userInput(Down, 1);
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Attaching);
  updateCurrentState();

  ck_assert_int_eq(2, info->player.level);

  userInput(-1, 0);
  userInput(Terminate, 0);
}
END_TEST

START_TEST(test_8) {
  initAllInfo();
  getState();
  *getState() = StartGame;
  AllInfo_t *info = getAllInfo();
  for (int j = 0; j < HEIGHT; j++) {
    info->field[1][j] = 1;
  }

  userInput(Start, 0);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Spawn);

  updateCurrentState();
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);

  updateCurrentState();
  userInput(Down, 0);
  userInput(Down, 1);
  ck_assert_int_eq(*getState(), Moving);

  while (*getState() != Shifting) {
    updateCurrentState();
  }
  ck_assert_int_eq(*getState(), Shifting);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Attaching);
  updateCurrentState();

  userInput(-1, 0);
  userInput(Terminate, 0);
}
END_TEST

START_TEST(test_9) {
  initAllInfo();
  getState();
  *getState() = StartGame;

  userInput(Start, 0);
  updateCurrentState();
  ck_assert_int_eq(*getState(), Spawn);

  updateCurrentState();
  ck_assert_int_eq(*getState(), Moving);

  userInput(Pause, 0);
  GameInfo_t game = updateCurrentState();
  ck_assert_int_eq(game.pause, true);

  userInput(Pause, 0);
  game = updateCurrentState();
  ck_assert_int_eq(game.pause, false);

  userInput(-1, 0);
  userInput(Terminate, 0);

  destroyGame();
}
END_TEST

Suite *suite_calc_test() {
  Suite *s = suite_create("s21_tetris");
  TCase *tc = tcase_create("s21_tetris_test");

  tcase_add_test(tc, test_1);
  tcase_add_test(tc, test_2);
  tcase_add_test(tc, test_3);
  tcase_add_test(tc, test_4);
  tcase_add_test(tc, test_5);
  tcase_add_test(tc, test_6);
  tcase_add_test(tc, test_7);
  tcase_add_test(tc, test_8);
  tcase_add_test(tc, test_9);

  suite_add_tcase(s, tc);
  return s;
}
