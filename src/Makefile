CFLAGS = gcc -g -Wall -Werror -Wextra -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage

LDFLAGS = -lncurses
MKDIR_P = mkdir -p

TETRIS_DIR = brick_game/tetris
TESTS_DIR = unit_tests
GUI_DIR = gui/cli
BIN_DIR = bin

SOURCES = $(TETRIS_DIR)/tetris.c $(GUI_DIR)/cli.c
OBJECTS = $(TETRIS_DIR)/*.o $(GUI_DIR)/*.o

PROJECT_NAME = BrickGame_v1.0

OS = $(shell uname)
ifeq ($(OS), Linux)
	CHECK_FLAGS = -lpthread -lcheck -pthread -lrt -lm -lsubunit
	OPEN_CMD = xdg-open
else
	CHECK_FLAGS = -lcheck
	OPEN_CMD = open
endif


all: clean install

install:
	@echo "Installing $(PROJECT_NAME)..."
	@$(MKDIR_P) $(BIN_DIR)
	$(CFLAGS) $(SOURCES) -o $(BIN_DIR)/tetris $(LDFLAGS)
	./$(BIN_DIR)/tetris

uninstall:
	@echo "Uninstalling $(PROJECT_NAME)..."
	rm -rf $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR) $(OBJECTS) \
	*.gcno *.gcda *.a *.info report *.out *.dSYM gcov_test $(TESTS_DIR)/unit_test \
	$(PROJECT_NAME) $(PROJECT_NAME).tgz \
	dvi/*.aux dvi/*.log dvi/*.pdf dvi/*.dvi \
	high-all-time.txt \


dvi:
	@echo "Building DVI documentation..."
	cd dvi && latex dvi.tex
	cd dvi && rm -rf dvi.log dvi.aux
	cd dvi && dvipdf dvi.dvi

dist: clean
	@echo "Creating distribution archive..."
	mkdir $(PROJECT_NAME)
	cp -r brick_game $(PROJECT_NAME)
	cp -r dvi $(PROJECT_NAME)
	cp -r gui $(PROJECT_NAME)
	cp Makefile $(PROJECT_NAME)
	tar -cvzf $(PROJECT_NAME).tgz $(PROJECT_NAME)
	rm -rf $(PROJECT_NAME)

tetris_backend.a:
	$(CFLAGS) -c $(TETRIS_DIR)/*.c
	ar rcs tetris_backend.a *.o
	rm -rf *.o

test: tetris_backend.a
	@echo "Running tests..."
	$(CFLAGS) $(TESTS_DIR)/*.c tetris_backend.a $(CHECK_FLAGS) -o $(TESTS_DIR)/unit_test
	./$(TESTS_DIR)/unit_test

gcov_report: tetris_backend.a
	@echo "Generating coverage report..."
	$(CFLAGS) $(GCOVFLAGS) $(TESTS_DIR)/*.c $(TETRIS_DIR)/*.c $(CHECK_FLAGS) -o gcov_test
	./gcov_test
	gcov *.c
	gcov testc/*.c
	lcov -t "test" -o test.info -c -d .
	genhtml -o report/ test.info
	rm -rf *.o *.gcno *.gcda
	$(OPEN_CMD) ./report/index.html

clang-fix:
	@echo "Running fix Google style errors..."
	clang-format -style=Google -i */*/*.c */*/*.h */*.c */*.h

clang:
	@echo "Running Google style check..."
	clang-format -style=Google -n */*/*.c */*/*.h */*.c */*.h

leaks: test
	@echo "Running leaks..."
	CK_FORK=no leaks --atExit -- ./$(TESTS_DIR)/unit_test

valgrind: test
	@echo "Running valgrind..."
	valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./$(TESTS_DIR)/unit_test

.PHONY: all clean install uninstall dvi dist test gcov_report