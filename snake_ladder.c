#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 100
#define NUM_PLAYERS 2

// Structure to define snakes and ladders
typedef struct {
    int start;
    int end;
} SnakeOrLadder;

// Function prototypes
void initializeBoard();
void displayBoard(int player1Pos, int player2Pos);
int rollDice();
int checkSnakeOrLadder(int position);
void displayPosition(int player, int position);

// Define snakes (head -> tail)
SnakeOrLadder snakes[] = {
    {99, 54},
    {70, 55},
    {52, 42},
    {25, 2},
    {95, 72}
};

// Define ladders (bottom -> top)
SnakeOrLadder ladders[] = {
    {6, 25},
    {11, 40},
    {60, 85},
    {46, 90},
    {17, 69}
};

int numSnakes = sizeof(snakes) / sizeof(snakes[0]);
int numLadders = sizeof(ladders) / sizeof(ladders[0]);

int main() {
    int player1 = 0, player2 = 0;
    int currentPlayer = 1;
    int diceRoll;
    int newPosition;
    
    srand(time(NULL));
    
    printf("===================================\n");
    printf("   SNAKE AND LADDER GAME\n");
    printf("===================================\n\n");
    printf("Game Rules:\n");
    printf("1. Two players take turns rolling the dice\n");
    printf("2. Move forward by the number shown on dice\n");
    printf("3. If you land on a ladder, climb up!\n");
    printf("4. If you land on a snake, slide down!\n");
    printf("5. First to reach 100 wins!\n\n");
    
    printf("Snakes on board:\n");
    for (int i = 0; i < numSnakes; i++) {
        printf("  %d -> %d\n", snakes[i].start, snakes[i].end);
    }
    
    printf("\nLadders on board:\n");
    for (int i = 0; i < numLadders; i++) {
        printf("  %d -> %d\n", ladders[i].start, ladders[i].end);
    }
    
    printf("\nPress Enter to start the game...");
    getchar();
    
    // Game loop
    while (player1 < BOARD_SIZE && player2 < BOARD_SIZE) {
        printf("\n===================================\n");
        printf("Player %d's turn\n", currentPlayer);
        printf("Current positions - Player 1: %d, Player 2: %d\n", player1, player2);
        printf("Press Enter to roll the dice...");
        getchar();
        
        diceRoll = rollDice();
        printf("You rolled: %d\n", diceRoll);
        
        if (currentPlayer == 1) {
            newPosition = player1 + diceRoll;
            
            if (newPosition > BOARD_SIZE) {
                printf("You need exactly %d to win. Stay at %d.\n", 
                       BOARD_SIZE - player1, player1);
            } else {
                player1 = newPosition;
                printf("Player 1 moves to position %d\n", player1);
                
                int finalPos = checkSnakeOrLadder(player1);
                if (finalPos != player1) {
                    player1 = finalPos;
                }
                
                if (player1 == BOARD_SIZE) {
                    printf("\n🎉 PLAYER 1 WINS! 🎉\n");
                    break;
                }
            }
        } else {
            newPosition = player2 + diceRoll;
            
            if (newPosition > BOARD_SIZE) {
                printf("You need exactly %d to win. Stay at %d.\n", 
                       BOARD_SIZE - player2, player2);
            } else {
                player2 = newPosition;
                printf("Player 2 moves to position %d\n", player2);
                
                int finalPos = checkSnakeOrLadder(player2);
                if (finalPos != player2) {
                    player2 = finalPos;
                }
                
                if (player2 == BOARD_SIZE) {
                    printf("\n🎉 PLAYER 2 WINS! 🎉\n");
                    break;
                }
            }
        }
        
        // Switch player
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }
    
    printf("\nGame Over! Thanks for playing!\n");
    printf("===================================\n");
    
    return 0;
}

int rollDice() {
    return (rand() % 6) + 1;
}

int checkSnakeOrLadder(int position) {
    // Check for snakes
    for (int i = 0; i < numSnakes; i++) {
        if (snakes[i].start == position) {
            printf("🐍 Oh no! Snake bite! Sliding down from %d to %d\n", 
                   snakes[i].start, snakes[i].end);
            return snakes[i].end;
        }
    }
    
    // Check for ladders
    for (int i = 0; i < numLadders; i++) {
        if (ladders[i].start == position) {
            printf("🪜 Yay! Ladder! Climbing up from %d to %d\n", 
                   ladders[i].start, ladders[i].end);
            return ladders[i].end;
        }
    }
    
    return position;
}
