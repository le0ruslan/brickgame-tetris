#ifndef TETRIS_H
#define TETRIS_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 20   // высота игрового поля
#define HEIGHT 10  // ширина игрового поля
#define MAX_LEVEL 10  // максимальное количество уровней
#define SCORE_TO_LEVEL \
  600  // количество очков для перехода на следующий уровень

typedef struct {  //. Дана по заданию
  int **field;  // двумерный массив инфомация об игровом поле, о его текущем
  // состоянии
  int **next;  // двумерный массив следующая фигура
  int score;   // счет игрока
  int high_score;  // максимальный счет
  int level;       // уровень
  int speed;       // скорость
  int pause;       // пауза
} GameInfo_t;

typedef enum {  // описание действий пользователя. Дана по заданию
  Start,        // начало игры
  Pause,        // пауза
  Terminate,    // завершение
  Left,         // перемещение фигуры влево
  Right,        // перемещение фигуры вправо
  Up,           // перемещение фигуры вверх
  Down,         // перемещение фигуры вниз
  Action        // поворот фигуры
} UserAction_t;

typedef enum State_t {  // состояние игры
  StartGame,            // начальное состояние
  Spawn,                // появление фигуры на поле
  Moving,               // движение фигуры
  Shifting,             // перемещение фигуры по полю
  Attaching,  // присоединение фигуры к уже имеющимся фигурам
  End         // конец игры
} State_t;

typedef struct Figure_t {  // описание фигур
  int id;                  // уникальный идентификатор
  int **coordinates;  // двумерный массив для отрисовки фигуры
  int length;  // размер двумерного массива
  int x, y;  // координаты начальной точки фигуры
} Figure_t;

typedef struct PlayerInfo_t {  // описание текущей игры
  int score;                   // текущий счет
  int high_score;              // максимальный счет
  int level;                   // текущий уровень
  int speed;                   // скорость падения фигуры
  bool pause;                  // состояние игры
  bool start;                  // состояние паузы
} PlayerInfo_t;

typedef struct AllInfo_t {
  int **field;  // двумерный массив инфомация об игровом поле, о его текущем
                // состоянии
  Figure_t *current;              // текущая фигура
  Figure_t *next;                 // следующая фигура
  PlayerInfo_t player;            // // описание текущей игры
  struct timeval last_drop_time;  // время последнего падения фигуры, нужна для
                                  // рассчета скорости падения
  bool err;  // наличие ошибки при создании двумерного массива
} AllInfo_t;

typedef enum {  // Описание фигур
  TC_EMPTY,
  TC_CELLI,  // горизонтальная линия
  TC_CELLO,  // квадрат
  TC_CELLL,  // L-образная фигура (левый верхний угол)
  TC_CELLJ,  // J-образная фигура (правый верхний угол)
  TC_CELLZ,  // Z-образная фигура (влево).
  TC_CELLS,  // S-образная фигура .
  TC_CELLT   // Т-образная фигура.
} TetrisCell_t;

// Функции взаимодействия с интерфейсом
GameInfo_t updateCurrentState(void);  // обновляет текущее состояние игры
void userInput(UserAction_t action,
               bool hold);  // выполняет действия в зависимости от нажатых
                            // клавиш в интерфейсе

// Функции для работы с общей информацией об игре
State_t *getState(void);  // показывает состояние игры. Возвращает указатель на
                          // статическую переменную
AllInfo_t *getAllInfo(void);  // показывает общую информацию об игре. Возвращает
                              // указатель на статическую переменную.
GameInfo_t *getGameInfo(
    void);  // показывает информацию о текущей игре. Возвращает указатель на
            // статическую переменную.
void initAllInfo(void);  // создает стартовую общую информацию об игре
void destroyGame(void);  // очистка памяти выделенной для работы игры
GameInfo_t generateGameInfo(
    AllInfo_t *info);  // предоставляет значения для отображения текущего
                       // состояния игры
void updateAllInfo(AllInfo_t *info);  // обновление общей информации об игре
                                      // после присоединения фигур

// Функции для работы с фигурми
Figure_t *randomFigure(void);  // инициализация случайной фигуры
void freeFigure(Figure_t *figure);  // очистка памяти, выделенной для фигуры
int getFigureLength(int id);  // функция для получения длины двумерного массива
                              // в котором хранится фигура
int **getFigure(
    int id,
    int length);  // инициализация двумерного массива, который содержит фигуру
bool checkFigureField(AllInfo_t info);  // проверяет не вышла ли фигура за
                                        // границы поля(слева/справа/снизу) и не
                                        // столкнулась ли с др. фигурой
bool moveLeftFigure(AllInfo_t *info);  // двигает фигура влево
bool moveRightFigure(AllInfo_t *info);  // двигает фигуру вправо
bool moveDownFigure(
    AllInfo_t *info);  // перемещает фигуру вниз до соприкасновения с др фигурой
                       // или с нижней гранью поля
void rotateFigure(AllInfo_t *info);  // вращает фигуру
void figureToFiled(AllInfo_t *info);  // закрепление фигуры на игровом поле
void spawnFigure(
    AllInfo_t *info);  // функция для вызова функции создания случайной фигуры
bool isGameEnd(AllInfo_t *info);  // проверяет достигла ли текущая фигура верха
                                  // игрового поля

// Функции для работы с набарнными очками
int getHighScore(void);  // получает из файла максимальное количество очков
void setHighScore(
    int high_score);  // записывает максимальное количество очков в файл
int getPoints(int **field);  // возвращает количество очков
void destroyLine(int **field, int row);  // стирает заполненную строку
bool checkLine(int **field, int row);  // проверяет строку на заполненность
int computePoints(int count);  //  // возврщает количество очков в зависимости
                               //  от количества заполненных строк

// Функции для инициализации массивов и очещения памяти
int **initMatrix(int rows, int cols);
void freeMatrix(int **matrix, int length);

#endif  // TETRIS_H
