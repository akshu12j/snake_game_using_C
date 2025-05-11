/*
 * Snake Game in C
 * A simple console-based snake game with detailed comments for beginners
 * Controls: Arrow keys or WASD for movement, X to exit
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // For _getch() function
#include <windows.h> // For Sleep() function
#include <stdbool.h>

// Constants for game settings
#define WIDTH 40    // Width of the game board
#define HEIGHT 20   // Height of the game board
#define SNAKE_LENGTH 100 // Maximum snake length

// Structure to store snake coordinates
struct Snake {
    int x[SNAKE_LENGTH];
    int y[SNAKE_LENGTH];
    int length;
};

// Structure to store food coordinates
struct Food {
    int x;
    int y;
};

// Global variables
struct Snake snake;
struct Food food;
int score = 0;
bool gameOver = false;
char direction = 'd'; // Initial direction (right)

// Function prototypes
void initializeGame();
void drawBoard();
void generateFood();
void moveSnake();
void handleInput();
bool checkCollision();
void clearScreen();
void gotoxy(int x, int y);

int main() {
    // Set console title
    SetConsoleTitle("Snake Game");
    
    // Hide cursor
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
    
    // Initialize the game
    initializeGame();
    
    // Main game loop
    while (!gameOver) {
        // Draw the game board
        drawBoard();
        
        // Handle user input
        handleInput();
        
        // Move the snake
        moveSnake();
        
        // Check for collisions
        if (checkCollision()) {
            gameOver = true;
        }
        
        // Small delay to control game speed
        Sleep(100);
    }
    
    // Game over screen
    clearScreen();
    printf("\nGame Over! Final Score: %d\n", score);
    printf("Press any key to exit...");
    _getch();
    return 0;
}

// Initialize game components
void initializeGame() {
    // Initialize snake at the center of the board
    snake.length = 3;
    snake.x[0] = WIDTH / 2;
    snake.y[0] = HEIGHT / 2;
    
    // Initialize snake body
    for (int i = 1; i < snake.length; i++) {
        snake.x[i] = snake.x[0] - i;
        snake.y[i] = snake.y[0];
    }
    
    // Generate first food
    generateFood();
}

// Draw the game board and all game elements
void drawBoard() {
    gotoxy(0, 0);
    
    // Draw top border
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");
    
    // Draw game area
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0)
                printf("#");
                
            bool printed = false;
            
            // Draw snake
            for (int k = 0; k < snake.length; k++) {
                if (snake.x[k] == j && snake.y[k] == i) {
                    printf("O");
                    printed = true;
                    break;
                }
            }
            
            // Draw food
            if (!printed && food.x == j && food.y == i) {
                printf("*");
                printed = true;
            }
            
            // Draw empty space
            if (!printed)
                printf(" ");
                
            if (j == WIDTH - 1)
                printf("#");
        }
        printf("\n");
    }
    
    // Draw bottom border
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");
    
    // Display score
    printf("Score: %d\n", score);
    printf("Controls: Arrow keys or WASD to move, X to exit");
}

// Generate food at random position
void generateFood() {
    bool validPosition;
    do {
        validPosition = true;
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
        
        // Check if food is not on snake
        for (int i = 0; i < snake.length; i++) {
            if (food.x == snake.x[i] && food.y == snake.y[i]) {
                validPosition = false;
                break;
            }
        }
    } while (!validPosition);
}

// Move the snake based on current direction
void moveSnake() {
    // Save current head position
    int prevX = snake.x[0];
    int prevY = snake.y[0];
    int prev2X, prev2Y;
    
    // Move head based on direction
    switch (direction) {
        case 'w': // Up
        case 'W':
        case 72:  // Up arrow
            snake.y[0]--;
            break;
        case 's': // Down
        case 'S':
        case 80:  // Down arrow
            snake.y[0]++;
            break;
        case 'a': // Left
        case 'A':
        case 75:  // Left arrow
            snake.x[0]--;
            break;
        case 'd': // Right
        case 'D':
        case 77:  // Right arrow
            snake.x[0]++;
            break;
    }
    
    // Move body
    for (int i = 1; i < snake.length; i++) {
        prev2X = snake.x[i];
        prev2Y = snake.y[i];
        snake.x[i] = prevX;
        snake.y[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    
    // Check if snake ate food
    if (snake.x[0] == food.x && snake.y[0] == food.y) {
        score += 10;
        snake.length++;
        generateFood();
    }
}

// Handle user input
void handleInput() {
    if (_kbhit()) {
        int key = _getch();
        
        // Handle arrow keys (they return two values)
        if (key == 224) {
            key = _getch();
        }
        
        switch (key) {
            case 'w':
            case 'W':
            case 72:  // Up arrow
                if (direction != 's' && direction != 'S' && direction != 80)
                    direction = 'w';
                break;
            case 's':
            case 'S':
            case 80:  // Down arrow
                if (direction != 'w' && direction != 'W' && direction != 72)
                    direction = 's';
                break;
            case 'a':
            case 'A':
            case 75:  // Left arrow
                if (direction != 'd' && direction != 'D' && direction != 77)
                    direction = 'a';
                break;
            case 'd':
            case 'D':
            case 77:  // Right arrow
                if (direction != 'a' && direction != 'A' && direction != 75)
                    direction = 'd';
                break;
            case 'x':
            case 'X':
                gameOver = true;
                break;
        }
    }
}

// Check for collisions with walls or self
bool checkCollision() {
    // Check wall collision
    if (snake.x[0] < 0 || snake.x[0] >= WIDTH ||
        snake.y[0] < 0 || snake.y[0] >= HEIGHT)
        return true;
    
    // Check self collision
    for (int i = 1; i < snake.length; i++) {
        if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i])
            return true;
    }
    
    return false;
}

// Clear the console screen
void clearScreen() {
    system("cls");
}

// Move cursor to specific position
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
} 