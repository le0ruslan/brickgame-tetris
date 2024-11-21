#include "cli.h"

// Основная функция, которая запускает игру
int main(void) {
  initializeGame();
  AllInfo_t *info = getAllInfo();
  if (!info->err) {
    WINDOW *fieldWin = newwin(22, 22, 0, 0);
    WINDOW *playerWin = newwin(11, 20, 0, 24);
    WINDOW *keyboardWin = newwin(11, 20, 11, 24);
    gameLoop(fieldWin, playerWin, keyboardWin);
    terminateGame(fieldWin, playerWin, keyboardWin);
  }
  return 0;
}

// Функция для инициализации игры
void initializeGame() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  timeout(100);
  curs_set(0);
  init_colors();
  initAllInfo();  // Инициализация структуры данных с информацией об игре
}

// Основной игровой цикл
void gameLoop(WINDOW *fieldWin, WINDOW *playerWin, WINDOW *keyboardWin) {
  AllInfo_t *info = getAllInfo();
  while (*getState() != End && !info->err) {
    UserAction_t userAction =
        getUserInput();  // Получение действия от пользователя
    updateGameState(userAction);  // Обновление состояния игры
    renderGame(fieldWin, playerWin,
               keyboardWin);  // Отрисовка текущего состояния игры
  }
}

// Завершение игры и очистка ресурсов
void terminateGame(WINDOW *fieldWin, WINDOW *playerWin, WINDOW *keyboardWin) {
  destroyGame();
  delwin(fieldWin);
  delwin(playerWin);
  delwin(keyboardWin);
  endwin();
}

// Функция для получения действия пользователя
UserAction_t getUserInput(void) {
  int ch = getch();
  return getKeyboardTap(ch);
}

// Функция обновления состояния игры
void updateGameState(UserAction_t userAction) {
  bool hold = (userAction == Down);
  userInput(userAction, hold);
}

// Функция для отрисовки игры
void renderGame(WINDOW *fieldWin, WINDOW *playerWin, WINDOW *keyboardWin) {
  GameInfo_t game = updateCurrentState();  // Получение текущего состояния игры

  if (*getState() == StartGame) {
    attron(COLOR_PAIR(TS_START));
    mvwprintw(stdscr, 10, 5, "Press \"Enter\"");
    mvwprintw(stdscr, 11, 7, "to Start");
    attroff(COLOR_PAIR(TS_START));
  }
  if (game.pause) {
    attron(COLOR_PAIR(TS_PAUSE));
    mvwprintw(stdscr, 10, 8, "PAUSE");
    mvwprintw(stdscr, 12, 1, "Press \"P\" to resume");
    attroff(COLOR_PAIR(TS_PAUSE));
  }
  showField(fieldWin, game);
  showInfoAboutGame(playerWin, game);
  showKeyboard(keyboardWin);
  doupdate();
  napms(50);
}

// Функция для отрисовки игрового поля
void showField(WINDOW *fieldWin, GameInfo_t game) {
  wclear(fieldWin);
  box(fieldWin, 0, 0);
  int **coordinates = game.field;

  if (coordinates) {
    for (int i = 0; i < WIDTH; i++) {
      for (int j = 0; j < HEIGHT; j++) {
        if (coordinates[i][j]) {
          int color;
          color = game.field[i][j];
          wattron(fieldWin, COLOR_PAIR(color));
          mvwaddch(fieldWin, i + 1, j * 2 + 1,
                   ' ' | A_REVERSE | COLOR_PAIR(color));
          mvwaddch(fieldWin, i + 1, j * 2 + 2,
                   ' ' | A_REVERSE | COLOR_PAIR(color));
          wattroff(fieldWin, COLOR_PAIR(color));
        }
      }
    }
  }

  wnoutrefresh(fieldWin);
}

// Функция для отрисовки ифнформации о текущей игре
void showInfoAboutGame(WINDOW *playerWin, GameInfo_t game) {
  wclear(playerWin);
  box(playerWin, 0, 0);
  mvwprintw(playerWin, 1, 1, " Next:");
  int **next = game.next;
  if (next) {
    int len = getAllInfo()->next->length;
    int color = getAllInfo()->next->id;
    for (int i = 0; i < len; i++) {
      for (int j = 0; j < len; j++) {
        if (next[i][j]) {
          wattron(playerWin, COLOR_PAIR(color));
          mvwaddch(playerWin, i + 3, j * 2 + 5,
                   ' ' | A_REVERSE | COLOR_PAIR(color));
          mvwaddch(playerWin, i + 3, j * 2 + 6,
                   ' ' | A_REVERSE | COLOR_PAIR(color));
          wattroff(playerWin, COLOR_PAIR(color));
        }
      }
    }
  }
  mvwprintw(playerWin, 6, 1, " Score:      %d", game.score);
  mvwprintw(playerWin, 7, 1, " Hihg score: %d", game.high_score);
  mvwprintw(playerWin, 8, 1, " Level:      %d", game.level);
  mvwprintw(playerWin, 9, 1, " Speed:      x%d",
            convertPlayerLevel(game.speed));
  wnoutrefresh(playerWin);
}

// Функция для отрисовки управления клавишами
void showKeyboard(WINDOW *keyboardWin) {
  wclear(keyboardWin);
  box(keyboardWin, 0, 0);
  mvwprintw(keyboardWin, 1, 6, "Controls");
  mvwprintw(keyboardWin, 3, 1, "\"Enter\" - Start");
  mvwprintw(keyboardWin, 4, 1, "\"P\"     - Pause");
  mvwprintw(keyboardWin, 5, 1, "\"Esc\"   - Exit");
  mvwprintw(keyboardWin, 6, 1, "\"<\"     - To left");
  mvwprintw(keyboardWin, 7, 1, "\">\"     - To right");
  mvwprintw(keyboardWin, 8, 1, "\"v\"     - To down");
  mvwprintw(keyboardWin, 9, 1, "\"z\"     - Rotate");
  wnoutrefresh(keyboardWin);
}

// Функция для захвата нажатых клавиш
UserAction_t getKeyboardTap(int user_input) {
  State_t *state = getState();
  UserAction_t action = -1;

  switch (user_input) {
    case KEY_UP:
      action = Up;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case 'z':
    case 'Z':
      action = Action;
      break;
    case '\n':
      action = Start;
      break;
    case 27:
      action = Terminate;
      break;
    case 'p':
    case 'P':
      action = Pause;
      break;
    default:
      break;
  }
  if ((*state == Spawn || *state == Shifting) && action >= Left &&
      action <= Action) {
    ungetch(user_input);
  }
  return action;
}

// Функция для инициализации цветов
void init_colors(void) {
  start_color();
  init_pair(TC_CELLI, COLOR_CYAN, COLOR_BLACK);
  init_pair(TC_CELLJ, COLOR_BLUE, COLOR_BLACK);
  init_pair(TC_CELLL, COLOR_WHITE, COLOR_BLACK);
  init_pair(TC_CELLO, COLOR_YELLOW, COLOR_BLACK);
  init_pair(TC_CELLS, COLOR_GREEN, COLOR_BLACK);
  init_pair(TC_CELLT, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(TC_CELLZ, COLOR_RED, COLOR_BLACK);

  init_pair(TS_START, COLOR_GREEN, COLOR_BLACK);
  init_pair(TS_PAUSE, COLOR_WHITE, COLOR_BLACK);
}

//Функция для конвертации скорости игры в человекопонятное выражение
int convertPlayerLevel(int num) {
  int speed = 0;
  switch (num) {
    case 1048576:
      speed = 1;
      break;
    case 262144:
      speed = 2;
      break;
    case 65536:
      speed = 3;
      break;
    case 16384:
      speed = 4;
      break;
    case 4096:
      speed = 5;
      break;
    case 1024:
      speed = 6;
      break;
    case 256:
      speed = 7;
      break;
    case 64:
      speed = 8;
      break;
    case 16:
      speed = 9;
      break;
    case 4:
      speed = 10;
      break;
    default:
      break;
  }
  return speed;
}