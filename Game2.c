/* snake_game.c
   Classic Snake Game for Windows console
   Controls:
     - W / Up Arrow    : Up
     - S / Down Arrow  : Down
     - A / Left Arrow  : Left
     - D / Right Arrow : Right
     - P               : Pause / Resume
     - Q               : Quit immediately
*/

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 30
#define HEIGHT 20
#define MAX_SNAKE (WIDTH * HEIGHT)

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

int snakeX[MAX_SNAKE], snakeY[MAX_SNAKE];
int snakeLength;
Direction dir;
int foodX, foodY;
int score;
int highscore = 0;
int gameOver = 0;
int paused = 0;
char highscoreFile[] = "highscore.txt";

void loadHighscore() {
    FILE *f = fopen("highscore.txt", "r");
    if (f == NULL) {
        // File does not exist → create it
        f = fopen("highscore.txt", "w");
        if (f) {
            fprintf(f, "0");
            fclose(f);
        }
        highscore = 0;
        return;
    }
    fscanf(f, "%d", &highscore);
    fclose(f);
}

void saveHighscore() {
    FILE *f = fopen("highscore.txt", "w");
    if (f == NULL) {
        printf("Error: Could not save highscore!\n");
        return;
    }
    fprintf(f, "%d", score);
    fclose(f);
}

void placeFood() {
    int ok = 0;
    while (!ok) {
        ok = 1;
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;
        for (int i = 0; i < snakeLength; i++) {
            if (snakeX[i] == foodX && snakeY[i] == foodY) {
                ok = 0;
                break;
            }
        }
    }
}

void initGame() {
    srand((unsigned)time(NULL));
    snakeLength = 4;
    int midX = WIDTH / 2;
    int midY = HEIGHT / 2;
    for (int i = 0; i < snakeLength; i++) {
        snakeX[i] = midX - i;
        snakeY[i] = midY;
    }
    dir = RIGHT;
    score = 0;
    gameOver = 0;
    paused = 0;
    placeFood();
    loadHighscore();
}

void draw() {
    // Clear screen
    system("cls");
    // Top border
    for (int x = 0; x < WIDTH + 2; x++) putchar('#');
    putchar('\n');

    for (int y = 0; y < HEIGHT; y++) {
        putchar('#');
        for (int x = 0; x < WIDTH; x++) {
            int drawn = 0;
            // draw snake head
            if (snakeX[0] == x && snakeY[0] == y) {
                putchar('O'); // head
                drawn = 1;
            } else {
                // draw body
                for (int k = 1; k < snakeLength; k++) {
                    if (snakeX[k] == x && snakeY[k] == y) {
                        putchar('o');
                        drawn = 1;
                        break;
                    }
                }
            }
            // draw food
            if (!drawn) {
                if (foodX == x && foodY == y) {
                    putchar('*');
                    drawn = 1;
                }
            }
            if (!drawn) putchar(' ');
        }
        putchar('#');
        putchar('\n');
    }

    // Bottom border
    for (int x = 0; x < WIDTH + 2; x++) putchar('#');
    putchar('\n');

    printf("Score: %d    Highscore: %d\n", score, highscore);
    printf("Controls: W/A/S/D or Arrow keys | P = Pause | Q = Quit\n");

    if (paused) printf("== PAUSED ==\n");
}

void moveSnake() {
    // Move body
    for (int i = snakeLength - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    // Move head
    switch (dir) {
        case UP:    snakeY[0]--; break;
        case DOWN:  snakeY[0]++; break;
        case LEFT:  snakeX[0]--; break;
        case RIGHT: snakeX[0]++; break;
    }
}

int checkCollision() {
    // Wall collision
    if (snakeX[0] < 0 || snakeX[0] >= WIDTH || snakeY[0] < 0 || snakeY[0] >= HEIGHT)
        return 1;
    // Self collision
    for (int i = 1; i < snakeLength; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) return 1;
    }
    return 0;
}

void update() {
    if (paused || gameOver) return;

    moveSnake();

    // Check for food
    if (snakeX[0] == foodX && snakeY[0] == foodY) {
        if (snakeLength < MAX_SNAKE) {
            // add new segment at the tail (duplicate last segment - will be corrected automatically next moves)
            snakeX[snakeLength] = snakeX[snakeLength - 1];
            snakeY[snakeLength] = snakeY[snakeLength - 1];
            snakeLength++;
        }
        score += 10;
        placeFood();
    }

    if (checkCollision()) {
        gameOver = 1;
    }
}

void handleInput() {
    if (!_kbhit()) return;
    int ch = _getch();
    // Arrow keys are two-character sequences starting with 0 or 224
    if (ch == 0 || ch == 224) {
        int ch2 = _getch();
        if (ch2 == 72) { // up arrow
            if (dir != DOWN) dir = UP;
        } else if (ch2 == 80) { // down arrow
            if (dir != UP) dir = DOWN;
        } else if (ch2 == 75) { // left arrow
            if (dir != RIGHT) dir = LEFT;
        } else if (ch2 == 77) { // right arrow
            if (dir != LEFT) dir = RIGHT;
        }
    } else {
        // letters
        if (ch == 'w' || ch == 'W') { if (dir != DOWN) dir = UP; }
        else if (ch == 's' || ch == 'S') { if (dir != UP) dir = DOWN; }
        else if (ch == 'a' || ch == 'A') { if (dir != RIGHT) dir = LEFT; }
        else if (ch == 'd' || ch == 'D') { if (dir != LEFT) dir = RIGHT; }
        else if (ch == 'p' || ch == 'P') { paused = !paused; draw(); }
        else if (ch == 'q' || ch == 'Q') { gameOver = 1; }
    }
}

int main() {
    initGame();

    while (1) {
        initGame();
        while (!gameOver) {
            handleInput();
            update();
            draw();

            // dynamic speed: faster as score increases
            int sleepMs = 150 - (score / 20); // reduces as score grows
            if (sleepMs < 50) sleepMs = 50;
            Sleep(sleepMs);
        }

        // Game over screen
        system("cls");
        printf("\n\n   === GAME OVER ===\n\n");
        printf("Score: %d\n", score);
        if (score > highscore) {
            printf("New Highscore! Old: %d  New: %d\n", highscore, score);
            highscore = score;
            saveHighscore();
        } else {
            printf("Highscore: %d\n", highscore);
        }
        printf("\nPlay again? (Y/N): ");

        int c = _getch();
        if (c == 'y' || c == 'Y') {
            gameOver = 0;
            // reset head position and snake length but keep highscore loaded
            initGame();
        } else {
            printf("\n\nThanks for playing! Exiting...\n");
            break;
        }
    }
    return 0;
}