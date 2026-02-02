#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_SNAKE_LENGTH (WIDTH * HEIGHT)
#define SNAKE_SPEED_US 100000   // 100 ms

/* ---------- Types ---------- */

typedef struct {
    int x;
    int y;
} Point;

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

typedef struct {
    Point body[MAX_SNAKE_LENGTH];
    int length;
    Direction direction;
    Direction nextDirection;
} Snake;

typedef struct {
    Snake snake;
    Point food;
    int score;
    int gameOver;
} Game;

/* ---------- Terminal ---------- */

static struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int kbhit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

char getKey() {
    char c = 0;
    if (read(STDIN_FILENO, &c, 1) != 1)
        return 0;

    if (c == 27) { // ESC
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return 0;
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return 0;

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return 'w';
                case 'B': return 's';
                case 'C': return 'd';
                case 'D': return 'a';
            }
        }
        return 0;
    }
    return c;
}

void clearScreen() {
    printf("\033[2J\033[H");
}

/* ---------- Game Logic ---------- */

void generateFood(Game *g) {
    int valid = 0;

    while (!valid) {
        g->food.x = rand() % (WIDTH  - 2) + 1;
        g->food.y = rand() % (HEIGHT - 2) + 1;

        valid = 1;
        for (int i = 0; i < g->snake.length; i++) {
            if (g->snake.body[i].x == g->food.x &&
                g->snake.body[i].y == g->food.y) {
                valid = 0;
                break;
            }
        }
    }
}

void initGame(Game *g) {
    g->snake.length = 3;
    g->snake.direction = DIR_RIGHT;
    g->snake.nextDirection = DIR_RIGHT;

    int startX = WIDTH / 2;
    int startY = HEIGHT / 2;

    for (int i = 0; i < g->snake.length; i++) {
        g->snake.body[i].x = startX - i;
        g->snake.body[i].y = startY;
    }

    g->score = 0;
    g->gameOver = 0;
    generateFood(g);
}

void handleInput(Game *g) {
    if (!kbhit()) return;

    char key = getKey();

    switch (key) {
        case 'w': case 'W':
            if (g->snake.direction != DIR_DOWN)
                g->snake.nextDirection = DIR_UP;
            break;
        case 's': case 'S':
            if (g->snake.direction != DIR_UP)
                g->snake.nextDirection = DIR_DOWN;
            break;
        case 'a': case 'A':
            if (g->snake.direction != DIR_RIGHT)
                g->snake.nextDirection = DIR_LEFT;
            break;
        case 'd': case 'D':
            if (g->snake.direction != DIR_LEFT)
                g->snake.nextDirection = DIR_RIGHT;
            break;
        case 'q': case 'Q':
            g->gameOver = 1;
            break;
    }
}

void moveSnake(Game *g, Point *oldTail) {
    Snake *s = &g->snake;
    *oldTail = s->body[s->length - 1];

    for (int i = s->length - 1; i > 0; i--) {
        s->body[i] = s->body[i - 1];
    }

    s->direction = s->nextDirection;

    switch (s->direction) {
        case DIR_UP:    s->body[0].y--; break;
        case DIR_DOWN:  s->body[0].y++; break;
        case DIR_LEFT:  s->body[0].x--; break;
        case DIR_RIGHT: s->body[0].x++; break;
    }
}

int checkCollision(Game *g) {
    Point head = g->snake.body[0];

    if (head.x <= 0 || head.x >= WIDTH - 1 ||
        head.y <= 0 || head.y >= HEIGHT - 1)
        return 1;

    for (int i = 1; i < g->snake.length; i++) {
        if (head.x == g->snake.body[i].x &&
            head.y == g->snake.body[i].y)
            return 1;
    }
    return 0;
}

void drawBoard(Game *g) {
    clearScreen();

    char board[HEIGHT][WIDTH];

    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            board[y][x] = ' ';

    for (int x = 0; x < WIDTH; x++) {
        board[0][x] = '#';
        board[HEIGHT - 1][x] = '#';
    }
    for (int y = 0; y < HEIGHT; y++) {
        board[y][0] = '#';
        board[y][WIDTH - 1] = '#';
    }

    for (int i = 0; i < g->snake.length; i++) {
        Point p = g->snake.body[i];
        board[p.y][p.x] = (i == 0) ? 'O' : 'o';
    }

    board[g->food.y][g->food.x] = '*';

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++)
            putchar(board[y][x]);
        putchar('\n');
    }

    printf("\nScore: %d | Length: %d\n", g->score, g->snake.length);
    printf("Controls: WASD / Arrow Keys | Q to Quit\n");
}

/* ---------- Main ---------- */

int main() {
    srand(time(NULL));

    Game game;
    printf("Press Enter to start...");
    getchar();

    enableRawMode();
    initGame(&game);

    while (!game.gameOver) {
        handleInput(&game);

        Point oldTail;
        moveSnake(&game, &oldTail);

        if (game.snake.body[0].x == game.food.x &&
            game.snake.body[0].y == game.food.y) {

            if (game.snake.length < MAX_SNAKE_LENGTH) {
                game.snake.body[game.snake.length] = oldTail;
                game.snake.length++;
            }
            game.score += 10;
            generateFood(&game);
        }

        if (checkCollision(&game))
            game.gameOver = 1;

        drawBoard(&game);
        usleep(SNAKE_SPEED_US);
    }

    disableRawMode();
    clearScreen();

    printf("GAME OVER\nScore: %d\nLength: %d\n",
           game.score, game.snake.length);

    return 0;
}
