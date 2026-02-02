#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_SNAKE_LENGTH (WIDTH * HEIGHT)

// Direction constants
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// Structure for coordinates
typedef struct {
    int x;
    int y;
} Point;

// Snake structure
typedef struct {
    Point body[MAX_SNAKE_LENGTH];
    int length;
    int direction;
} Snake;

// Game state
Snake snake;
Point food;
int score = 0;
int gameOver = 0;

// Terminal settings
struct termios orig_termios;

// Function prototypes
void disableRawMode();
void enableRawMode();
int kbhit();
char getKey();
void initGame();
void generateFood();
void drawBoard();
void moveSnake();
int checkCollision();
void handleInput();
void clearScreen();

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
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
    if (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == 27) { // ESC sequence
            char seq[3];
            if (read(STDIN_FILENO, &seq[0], 1) == 1) {
                if (read(STDIN_FILENO, &seq[1], 1) == 1) {
                    if (seq[0] == '[') {
                        switch (seq[1]) {
                            case 'A': return 'w'; // Up
                            case 'B': return 's'; // Down
                            case 'C': return 'd'; // Right
                            case 'D': return 'a'; // Left
                        }
                    }
                }
            }
        }
        return c;
    }
    return 0;
}

void clearScreen() {
    printf("\033[2J\033[H");
}

void initGame() {
    // Initialize snake in the middle
    snake.length = 3;
    snake.direction = RIGHT;
    
    int startX = WIDTH / 2;
    int startY = HEIGHT / 2;
    
    for (int i = 0; i < snake.length; i++) {
        snake.body[i].x = startX - i;
        snake.body[i].y = startY;
    }
    
    generateFood();
    score = 0;
    gameOver = 0;
}

void generateFood() {
    int validPosition = 0;
    
    while (!validPosition) {
        food.x = rand() % (WIDTH - 2) + 1;
        food.y = rand() % (HEIGHT - 2) + 1;
        
        validPosition = 1;
        // Check if food spawns on snake
        for (int i = 0; i < snake.length; i++) {
            if (snake.body[i].x == food.x && snake.body[i].y == food.y) {
                validPosition = 0;
                break;
            }
        }
    }
}

void drawBoard() {
    clearScreen();
    
    char board[HEIGHT][WIDTH];
    
    // Initialize board with spaces
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = ' ';
        }
    }
    
    // Draw borders
    for (int i = 0; i < WIDTH; i++) {
        board[0][i] = '#';
        board[HEIGHT - 1][i] = '#';
    }
    for (int i = 0; i < HEIGHT; i++) {
        board[i][0] = '#';
        board[i][WIDTH - 1] = '#';
    }
    
    // Draw snake
    for (int i = 0; i < snake.length; i++) {
        if (i == 0) {
            board[snake.body[i].y][snake.body[i].x] = 'O'; // Head
        } else {
            board[snake.body[i].y][snake.body[i].x] = 'o'; // Body
        }
    }
    
    // Draw food
    board[food.y][food.x] = '*';
    
    // Print board
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    
    printf("\nScore: %d | Length: %d\n", score, snake.length);
    printf("Controls: Arrow Keys or WASD | Q to Quit\n");
}

void moveSnake() {
    // Move body
    for (int i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }
    
    // Move head based on direction
    switch (snake.direction) {
        case UP:
            snake.body[0].y--;
            break;
        case DOWN:
            snake.body[0].y++;
            break;
        case LEFT:
            snake.body[0].x--;
            break;
        case RIGHT:
            snake.body[0].x++;
            break;
    }
}

int checkCollision() {
    Point head = snake.body[0];
    
    // Check wall collision
    if (head.x <= 0 || head.x >= WIDTH - 1 || head.y <= 0 || head.y >= HEIGHT - 1) {
        return 1;
    }
    
    // Check self collision
    for (int i = 1; i < snake.length; i++) {
        if (head.x == snake.body[i].x && head.y == snake.body[i].y) {
            return 1;
        }
    }
    
    return 0;
}

void handleInput() {
    if (kbhit()) {
        char key = getKey();
        
        switch (key) {
            case 'w':
            case 'W':
                if (snake.direction != DOWN) snake.direction = UP;
                break;
            case 's':
            case 'S':
                if (snake.direction != UP) snake.direction = DOWN;
                break;
            case 'a':
            case 'A':
                if (snake.direction != RIGHT) snake.direction = LEFT;
                break;
            case 'd':
            case 'D':
                if (snake.direction != LEFT) snake.direction = RIGHT;
                break;
            case 'q':
            case 'Q':
                gameOver = 1;
                break;
        }
    }
}

int main() {
    srand(time(NULL));
    
    printf("===================================\n");
    printf("       SNAKE GAME\n");
    printf("===================================\n");
    printf("\nControls:\n");
    printf("  Arrow Keys or WASD - Move\n");
    printf("  Q - Quit\n");
    printf("\nRules:\n");
    printf("  - Eat the food (*) to grow\n");
    printf("  - Don't hit the walls or yourself\n");
    printf("  - Try to get the highest score!\n");
    printf("\nPress Enter to start...");
    getchar();
    
    enableRawMode();
    initGame();
    
    while (!gameOver) {
        handleInput();
        moveSnake();
        
        // Check if snake ate food
        if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
            score += 10;
            snake.length++;
            generateFood();
        }
        
        // Check collision
        if (checkCollision()) {
            gameOver = 1;
        }
        
        drawBoard();
        usleep(100000); // 100ms delay (adjust for speed)
    }
    
    disableRawMode();
    
    clearScreen();
    printf("\n===================================\n");
    printf("         GAME OVER!\n");
    printf("===================================\n");
    printf("\n  Final Score: %d\n", score);
    printf("  Snake Length: %d\n\n", snake.length);
    printf("Thanks for playing!\n\n");
    
    return 0;
}
