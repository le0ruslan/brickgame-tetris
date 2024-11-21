#include "tetris.h"

GameInfo_t updateCurrentState(void) {  // обновляет текущее состояние игры
  AllInfo_t *info = getAllInfo();
  State_t *state = getState();
  if (!info->player.pause && info->player.start) {
    switch (*state) {
      case StartGame:
        *state = Spawn;
        break;
      case Spawn:
        spawnFigure(info);
        *state = Moving;
        break;
      case Moving:
        struct timeval cur_time = {0};
        gettimeofday(&cur_time, NULL);
        long diff_sec = cur_time.tv_sec - info->last_drop_time.tv_sec;
        long diff_msec = cur_time.tv_usec - info->last_drop_time.tv_usec;
        long diff = diff_sec * 1000000 + diff_msec;
        if (diff >= info->player.speed) {
          gettimeofday(&info->last_drop_time, NULL);
          *state = Shifting;
        }
        break;
      case Shifting:
        bool res = moveDownFigure(info);
        if (res == false) {
          *state = Attaching;
        } else {
          *state = Moving;
        }
        break;
      case Attaching:
        if (isGameEnd(info)) {
          setHighScore(info->player.high_score);
          initAllInfo();
          *state = StartGame;
        } else {
          figureToFiled(info);
          updateAllInfo(info);
          *state = Spawn;
        }
        break;
      default:
        break;
    }
  }
  GameInfo_t display = generateGameInfo(info);
  return display;
}

void userInput(UserAction_t action,
               bool hold) {  //выполняет действия в зависимости от нажатых
                             //клавиш в интерфейсе
  AllInfo_t *info = getAllInfo();
  State_t *state = getState();
  bool pause = info->player.pause;
  if (!hold) {
    switch (action) {
      case Start:
        if (*state == StartGame) info->player.start = 1;
        break;
      case Pause:
        if (*state != StartGame) info->player.pause = !info->player.pause;
        break;
      case Terminate:
        setHighScore(info->player.high_score);
        *state = End;
        break;
      case Left:
        if (*state == Moving && !pause) moveLeftFigure(info);
        break;
      case Right:
        if (*state == Moving && !pause) moveRightFigure(info);
        break;
      case Up:
        break;
      case Down:
        break;
      case Action:
        if (*state == Moving && !pause) rotateFigure(info);
        break;
      default:
        break;
    }
  } else {
    if (*state == Moving && !pause) {
      while (moveDownFigure(info)) {
        moveDownFigure(info);
      }
    }
  }
}

AllInfo_t *getAllInfo(
    void) {  // показывает общую информацию об игре. Возвращает указатель на
             // статическую переменную.
  static AllInfo_t all_info = {NULL, NULL, NULL, {0}, {0}, false};
  return &all_info;
}

State_t *getState(void) {  //  показывает состояние игры. Возвращает указатель
                           //  на статическую переменную
  static State_t state = 0;
  return &state;
}

GameInfo_t *getGameInfo(
    void) {  // показывает информацию о текущей игре. Возвращает указатель на
             // статическую переменную.
  static GameInfo_t game_info = {NULL, NULL, 0, 0, 0, 0, 0};
  return &game_info;
}

GameInfo_t generateGameInfo(
    AllInfo_t *info) {  // предоставляет значения для отображения текущего
                        // состояния игры
  GameInfo_t *display = getGameInfo();
  State_t *state = getState();

  if (display->field != NULL) {
    freeMatrix(display->field, WIDTH);
  }

  display->field = initMatrix(WIDTH, HEIGHT);
  int **disp = display->field;
  int **g = info->field;
  Figure_t *figure = info->current;

  if (figure && *state != StartGame && *state != Spawn) {
    int **f = figure->coordinates;
    int x_f = figure->x;
    int y_f = figure->y;
    int len_f = figure->length;
    int figure_id = figure->id;

    for (int i = 0; i < WIDTH; i++) {
      for (int j = 0; j < HEIGHT; j++) {
        if (i >= y_f && j >= x_f && (i - y_f < len_f) && (j - x_f < len_f)) {
          if (f[i - y_f][j - x_f]) {
            disp[i][j] = figure_id;
          } else {
            disp[i][j] = (g[i][j]) ? g[i][j] : 0;
          }
        } else {
          disp[i][j] = (g[i][j]) ? g[i][j] : 0;
        }
      }
    }
  } else if (g) {
    for (int i = 0; i < WIDTH; i++) {
      for (int j = 0; j < HEIGHT; j++) {
        disp[i][j] = g[i][j];
      }
    }
  }
  if (info->next) {
    display->next = info->next->coordinates;
  } else {
    display->next = NULL;
  }

  display->high_score = info->player.high_score;
  display->level = info->player.level;
  display->pause = info->player.pause;
  display->score = info->player.score;
  display->speed = info->player.speed;

  return *display;
}

void initAllInfo(void) {  // создает стартовую общую информацию об игре
  AllInfo_t *info = getAllInfo();
  if (info->field) freeMatrix(info->field, WIDTH);
  info->field = initMatrix(WIDTH, HEIGHT);
  if (!info->field) {
    fprintf(stderr, "Error: Memory allocation failed for game field.\n");
    info->err = true;
  } else {
    gettimeofday(&info->last_drop_time, NULL);
    info->player.high_score = getHighScore();
    info->player.level = 0;
    info->player.pause = 0;
    info->player.score = 500;
    info->player.start = 0;
    info->player.speed = 1048576;
  }
}

void updateAllInfo(AllInfo_t *info) {  // обновление общей информации об игре
                                       // после присоединения фигур
  info->player.score += getPoints(info->field);
  if (info->player.high_score < info->player.score) {
    info->player.high_score = info->player.score;
  }
  int new_level = info->player.score / SCORE_TO_LEVEL;
  if (new_level > info->player.level && new_level <= MAX_LEVEL) {
    info->player.level = new_level;
    info->player.speed = info->player.speed / 4;
  }
}

void destroyGame(void) {  // очистка памяти выделенной для работы игры
  AllInfo_t *info = getAllInfo();
  GameInfo_t *display = getGameInfo();
  if (display->field) freeMatrix(display->field, WIDTH);
  if (info->field) freeMatrix(info->field, WIDTH);
  if (info->current) freeFigure(info->current);
  if (info->next) freeFigure(info->next);
}

bool checkFigureField(
    AllInfo_t info) {  // проверяет не вышла ли фигура за границы поля и не
                       // столкнулась ли с др. фигурой
  int **field = info.field;
  int **figure = info.current->coordinates;
  int x = info.current->x;
  int y = info.current->y;
  int len = info.current->length;
  bool flag = false;
  for (int i = 0; i < len && !flag; i++) {
    for (int j = 0; j < len && !flag; j++) {
      int val_figure = figure[i][j];
      if (y + i >= 0) {
        if (x + j >= 0 && x + j < HEIGHT && y + i < WIDTH) {
          int val_field = field[i + y][j + x];
          flag = (val_figure > 0 && val_field > 0) ? true : false;
        } else {
          flag = (val_figure > 0) ? true : false;
        }
      } else if (x + j < 0 || x + j >= HEIGHT) {
        flag = true;
      }
    }
  }
  return flag;
}
// Функции для работы с фигурами
Figure_t *randomFigure(void) {  //  // инициализация случайной фигуры
  Figure_t *figure = (Figure_t *)calloc(1, sizeof(Figure_t));
  srand(time(NULL));
  figure->id = 1 + rand() % 7;
  figure->length = getFigureLength(figure->id);
  figure->coordinates = getFigure(figure->id, figure->length);
  figure->x = 5 - figure->length / 2;
  figure->y = -figure->length;
  return figure;
}

void freeFigure(Figure_t *figure) {  // очистка памяти, выделенной для фигуры
  if (figure) {
    freeMatrix(figure->coordinates, figure->length);
    free(figure);
    figure = NULL;
  }
}
bool moveLeftFigure(AllInfo_t *info) {
  bool res = true;
  Figure_t *figure = info->current;
  figure->x -= 1;
  if (checkFigureField(*info)) {
    figure->x += 1;
    res = false;
  }
  return res;
}

bool moveRightFigure(AllInfo_t *info) {
  bool res = true;
  Figure_t *figure = info->current;
  figure->x += 1;
  if (checkFigureField(*info)) {
    figure->x -= 1;
    res = false;
  }
  return res;
}

bool moveDownFigure(AllInfo_t *info) {
  bool res = true;
  Figure_t *figure = info->current;
  figure->y += 1;
  if (checkFigureField(*info)) {
    figure->y -= 1;
    res = false;
  }
  return res;
}

void rotateFigure(AllInfo_t *info) {
  Figure_t *figure = info->current;
  int len = figure->length;
  int **cur_m = figure->coordinates;
  int **new_m = initMatrix(len, len);
  if (!new_m) {
    fprintf(stderr,
            "Error: Memory allocation failed for rotateFigure new_m.\n");
    info->err = true;
  } else {
    for (int i = 0; i < len; i++) {
      for (int j = 0; j < len; j++) {
        new_m[j][len - i - 1] = cur_m[i][j];
      }
    }
    figure->coordinates = new_m;
    if (checkFigureField(*info)) {
      figure->coordinates = cur_m;
      freeMatrix(new_m, len);
    } else {
      freeMatrix(cur_m, len);
    }
  }
}

void figureToFiled(AllInfo_t *info) {  // закрепление фигуры на игровом поле
  int **field = info->field;
  Figure_t *figure = info->current;
  int x = figure->x;
  int y = figure->y;

  for (int i = 0; i < figure->length; i++) {
    for (int j = 0; j < figure->length; j++) {
      if (figure->coordinates[i][j]) {
        field[y + i][x + j] = figure->id;
      }
    }
  }
}

void spawnFigure(
    AllInfo_t *info) {  // функция для вызова функции создания случайной фигуры
  if (info->current != NULL) {
    freeFigure(info->current);
  }
  if (info->next == NULL) {
    info->next = randomFigure();
  }
  info->current = info->next;
  info->next = randomFigure();
}

int getFigureLength(int id) {  // // функция для получения длины двумерного
                               // массива в котором хранится фигура
  int length = 3;
  if (id == 1) {
    length = 5;
  } else if (id == 2) {
    length = 2;
  }
  return length;
}

int **getFigure(
    int id,
    int length) {  // инициализация двумерного массива, который содержит фигуру
  AllInfo_t *info = getAllInfo();
  int **figure = initMatrix(length, length);
  if (!figure) {
    fprintf(stderr, "Error: Memory allocation failed for getFigure.\n");
    info->err = true;
  } else {
    switch (id) {
      case 1:  // горизонтальная линия
        for (int j = 1; j < length; j++) figure[1][j] = TC_CELLI;
        break;
      case 2:  // квадрат
        for (int i = 0; i < length; i++)
          for (int j = 0; j < length; j++) figure[i][j] = TC_CELLO;
        break;
      case 3:  // L-образная фигура (левый верхний угол)
        for (int j = 0; j < length; j++) figure[1][j] = TC_CELLL;
        figure[0][0] = TC_CELLL;
        break;
      case 4:  // J-образная фигура (правый верхний угол)
        for (int j = 0; j < length; j++) figure[1][j] = TC_CELLJ;
        figure[0][2] = TC_CELLJ;
        break;
      case 5:  // Z-образная фигура (влево).
        for (int j = 0; j < length - 1; j++) {
          figure[0][j + 1] = TC_CELLZ;
          figure[1][j] = TC_CELLZ;
        }
        break;
      case 6:  // S-образная фигура .
        for (int j = 0; j < length - 1; j++) {
          figure[0][j] = TC_CELLS;
          figure[1][j + 1] = TC_CELLS;
        }
        break;
      case 7:  // Т-образная фигура.
        for (int j = 0; j < length; j++) figure[1][j] = TC_CELLT;
        figure[0][1] = TC_CELLT;
        break;
    }
  }
  return figure;
}

bool isGameEnd(AllInfo_t *info) {  // проверяет достигла ли текущая фигура верха
                                   // игрового поля
  bool res = false;
  Figure_t *cur = info->current;
  int **coordinates = cur->coordinates;
  int len = cur->length;
  int y = cur->y;
  if (y <= 0) {
    for (int i = 0; i < len; i++) {
      if (coordinates[-y][i]) {
        res = true;
        break;
      }
    }
  }
  return res;
}

int getHighScore(void) {  // получает из файла максимальное количество очков
  int high_score = 0;
  FILE *f = fopen("./high-all-time.txt", "r");
  if (f != NULL) {
    fscanf(f, "%d", &high_score);
    fclose(f);
  }
  return high_score;
}

void setHighScore(
    int high_score) {  // записывает максимальное количество очков в файл
  FILE *f = fopen("./high-all-time.txt", "w");
  if (f != NULL) {
    fprintf(f, "%d", high_score);
    fclose(f);
  }
}

int getPoints(int **field) {  // возвращает количество очков
  int num_row = 19;
  int count_full_lines = 0;
  while (num_row > 0) {
    if (checkLine(field, num_row)) {
      destroyLine(field, num_row);
      count_full_lines++;
    } else {
      num_row--;
    }
  }
  int points = computePoints(count_full_lines);
  return points;
}

void destroyLine(int **field, int row) {  // стирает заполненную строку
  for (int i = row; i > 0; i--) {
    for (int j = 0; j < HEIGHT; j++) {
      field[i][j] = field[i - 1][j];
    }
  }
}

bool checkLine(int **field, int row) {  // проверяет строку на заполненность
  bool flag = true;
  for (int j = 0; j < HEIGHT; j++) {
    if (!field[row][j]) {
      flag = false;
      break;
    }
  }
  return flag;
}

int computePoints(int count) {  // возврщает количество очков в зависимости от
                                // количества заполненных строк
  int points = 0;
  switch (count) {
    case 1:
      points = 100;
      break;
    case 2:
      points = 300;
      break;
    case 3:
      points = 700;
      break;
    case 4:
      points = 1500;
      break;
    default:
      break;
  }
  return points;
}

// Функции для инициализации массивов и очистки памяти
int **initMatrix(int rows, int cols) {  //
  int **matrix = (int **)calloc(rows, sizeof(int *));
  if (!matrix) return NULL;

  for (int i = 0; i < rows; i++) {
    matrix[i] = (int *)calloc(cols, sizeof(int));
    if (!matrix[i]) {
      for (int j = 0; j < i; j++) {
        free(matrix[j]);
      }
      free(matrix);
      return NULL;
    }
  }
  return matrix;
}

void freeMatrix(int **matrix, int rows) {
  if (matrix) {
    for (int i = 0; i < rows; i++) {
      free(matrix[i]);
    }
    free(matrix);
    matrix = NULL;
  }
}
