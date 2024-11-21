#ifndef CLI_H
#define CLI_H

#include <ncurses.h>

#include "../../brick_game/tetris/tetris.h"

typedef enum {  // определение состояний игры
  TS_START = 8,
  TS_PAUSE = 9
} game_status;

void initializeGame();  // Функция для инициализации игры
void gameLoop(WINDOW *fieldWin, WINDOW *playerWin,
              WINDOW *keyboardWin);  // Основной игровой цикл
void terminateGame(WINDOW *fieldWin, WINDOW *playerWin,
                   WINDOW *keyboardWin);  // Завершение игры и очистка ресурсов
UserAction_t getUserInput(void);  // Функция для получения действия пользователя
void updateGameState(
    UserAction_t userAction);  // Функция обновления состояния игры
void renderGame(WINDOW *fieldWin, WINDOW *playerWin,
                WINDOW *keyboardWin);  // Функция для отрисовки игры
void showField(WINDOW *fieldWin,
               GameInfo_t game);  // Функция для отрисовки игрового поля
void showInfoAboutGame(
    WINDOW *playerWin,
    GameInfo_t game);  // Функция для отрисовки ифнформации о текущей игре
void showKeyboard(
    WINDOW *keyboardWin);  // Функция для отрисовки управления клавишами
UserAction_t getKeyboardTap(
    int user_input);  // Функция для захвата нажатых клавиш
void init_colors(void);  // Функция для инициализации цветов

int convertPlayerLevel(int num);  // //Функция для конвертации скорости игры в
                                  // человекопонятное выражение

#endif  // CLI_H