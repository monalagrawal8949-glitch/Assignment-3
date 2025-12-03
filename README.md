🐍 Snake Game – Windows Console Edition

A Classic Snake Game written in C for the Windows CMD console

This project implements the iconic Snake Game using C language, complete with:
✔ Smooth movement
✔ Increasing speed
✔ Food system
✔ Highscore saving
✔ Pause/Resume
✔ Game Over handling
✔ Arrow keys + WASD support

🎮 Features
🟢 1. Fully Functional Snake Movement

Move using W/A/S/D or Arrow Keys

Prevents the snake from reversing into itself

Body follows head smoothly

🍎 2. Random Food Generation

Food never spawns on the snake’s body

Eating food increases:

Snake length

Score (+10 each time)

⚡ 3. Dynamic Speed

Game auto-speeds up as your score increases

Minimum speed capped at 50 ms

🧱 4. Collision Detection

Snake dies when:

It hits the wall

It bites its own tail

⏸️ 5. Pause and Quit

Press P to Pause/Resume

Press Q anytime to quit immediately

🏆 6. Highscore System

Highscore stored in highscore.txt

Automatically loaded at start

Updated when you beat the existing score

🕹️ Controls
Key	Action
W / ↑	Move Up
S / ↓	Move Down
A / ←	Move Left
D / →	Move Right
P	Pause/Resume
Q	Quit Game
📂 File Structure
snake_game.c
highscore.txt   (auto-created on first run)
README.md

🧠 Game Logic Overview
🔹 Snake Representation

The snake is stored as arrays:

snakeX[MAX_SNAKE];
snakeY[MAX_SNAKE];

🔹 Movement

Tail moves forward following the head

Direction stored as an enum UP, DOWN, LEFT, RIGHT

🔹 Food Placement

Ensures it never spawns on the snake:

while (!ok) {
    ok = 1;
    foodX = rand() % WIDTH;
    foodY = rand() % HEIGHT;
    for (...) if (collision) ok = 0;
}

🔹 Increasing Difficulty
int sleepMs = 150 - (score / 20);
if (sleepMs < 50) sleepMs = 50;

🔹 Highscore Storage

Reads from "highscore.txt"

Writes on game over if score is highest

📦 How to Compile & Run
Windows + GCC (MinGW):
gcc snake_game.c -o snake
snake

Turbo C++ / Borland / DOSBox

100% compatible

Just compile & run

📸 Sample Gameplay (Console Render)
##############################
#                            #
#      Oooo                  #
#                            #
#                 *          #
#                            #
##############################
Score: 40    Highscore: 70
Controls: W/A/S/D or Arrow keys | P = Pause | Q = Quit

🏁 Game Over Screen
=== GAME OVER ===

Score: 120
New Highscore! Old: 100  New: 120

Play again? (Y/N):
