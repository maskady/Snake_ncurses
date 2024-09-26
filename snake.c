/**
 * @file snake2.c
 * @brief This file contains the implementation of a Snake game using the ncurses library.
 *
 * The game logic includes initialization of the game window, handling user input,
 * updating the game state, and rendering the game on the screen.
 *
 * Dependencies:
 * - ncurses: For handling the game window and user input.
 * - stdio: For standard input and output operations.
 * - stdlib: For memory allocation and process control.
 * - unistd: For miscellaneous symbolic constants and types.
 * - time: For handling time-related functions.
 */
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "constants.h"
#include "draw.h"
#include "point.h"


/**
 * @brief Generates random coordinates for a fruit within the game boundaries.
 *
 * This function assigns random x and y coordinates to the fruit within the
 * specified game area, ensuring that the coordinates are within the valid
 * range defined by WIDTH and HEIGHT.
 *
 * @param fruitx Pointer to an integer where the x-coordinate of the fruit will be stored.
 * @param fruity Pointer to an integer where the y-coordinate of the fruit will be stored.
 */
void fruits(int* fruitx, int* fruity)
{
    *fruitx = rand() % (WIDTH-2) + 1;
    *fruity = rand() % (HEIGHT-2) + 1;
}



/**
 * @brief Handles user input for controlling the snake's direction.
 *
 * This function reads the user's input in a non-blocking manner and updates
 * the direction flag based on the arrow keys pressed. It also allows the user
 * to exit the game by pressing 'x' or 'X'.
 *
 * @param flag Pointer to an integer representing the current direction of the snake.
 *             The direction is updated based on the following key presses:
 *             - Arrow Up:    3 (if the current direction is not down)
 *             - Arrow Down:  1 (if the current direction is not up)
 *             - Arrow Right: 4 (if the current direction is not left)
 *             - Arrow Left:  2 (if the current direction is not right)
 *             - 'x' or 'X':  -1 (to exit the game)
 *
 * The function uses the `getch` function to read input and checks for escape
 * sequences to determine if an arrow key was pressed.
 */
void input(int* flag)
{
    timeout(0);  // Mode non-bloquant pour getch
    int ch = getch();  // Lire l'entrée de l'utilisateur

    // Vérifier si c'est une séquence d'échappement pour les touches fléchées
    if (ch == 27) {
        ch = getch();  // Lire le caractère suivant
        if (ch == 91) {
            ch = getch();  // Lire la direction de la flèche
            switch (ch)
            {
                case 65:  // Flèche haut
                    if(*flag != 1)
                        *flag = 3;
                    break;
                case 66:  // Flèche bas
                    if(*flag != 3)
                        *flag = 1;
                    break;
                case 67:  // Flèche droite
                    if(*flag != 2)
                        *flag = 4;
                    break;
                case 68:  // Flèche gauche
                    if(*flag != 4)
                        *flag = 2;
                    break;
                default:
                    break;
            }
        }
    }
    else {
        if(ch == 'x' || ch == 'X'){
            *flag = -1;
        }
    }
}


/**
 * @brief Moves the snake in the specified direction.
 *
 * This function updates the position of the snake based on the given direction flag.
 * The snake is represented as an array of Points, where each Point contains x and y coordinates.
 *
 * @param snake An array of Points representing the snake.
 * @param length The length of the snake.
 * @param flag The direction in which to move the snake:
 *             - 1: down
 *             - 2: left
 *             - 3: up
 *             - 4: right
 */
void move_snake(Point snake[], int length, int flag)
{
    Point temp = snake[length-1];
    for(int i = 0; i < length; i++){
        snake[i] = snake[i+1];
    }
    switch (flag)
    {
        case 1:  // down
            snake[length-1].x = temp.x + 1;
            snake[length-1].y = temp.y;
            break;
        case 2:  // left
            snake[length-1].x = temp.x;
            snake[length-1].y = temp.y - 1;
            break;
        case 3:  // up
            snake[length-1].x = temp.x - 1;
            snake[length-1].y = temp.y;
            break;
        case 4:  // right
            snake[length-1].x = temp.x;
            snake[length-1].y = temp.y + 1;
            break;
        default:
            break;
    }
}

/**
 * @brief Checks for collisions of the snake with boundaries or itself.
 *
 * This function checks if the snake has collided with the boundaries of the game area
 * or with itself. It returns a specific code based on the type of collision detected.
 *
 * @param snake An array of Point structures representing the snake's body.
 * @param length The current length of the snake.
 * @return int Returns 2 if there is a boundary collision, 1 if there is a self collision, 
 * and 0 if there is no collision.
 */
int check_collision(const Point snake[], int length)
{
    if (snake[length-1].x <= 0 || snake[length-1].x >= HEIGHT-1 || snake[length-1].y <= 0 || snake[length-1].y >= WIDTH-1){
        return 2;  // Boundary collision
    }
    for(int i = 0; i < length - 1; i++){
        if(snake[length-1].x == snake[i].x && snake[length-1].y == snake[i].y){
            return 1;  // Self collision
        }
    }
    return 0;  // No collision
}

/**
 * @brief Grows the snake by one segment, updates the score, and generates a new fruit position.
 *
 * This function increases the length of the snake by one segment, updates the score by adding 5 points,
 * and generates a new position for the fruit. The new segment is added at the head of the snake.
 *
 * @param snake An array of Point structures representing the snake's body.
 * @param length A pointer to an integer representing the current length of the snake.
 * @param score A pointer to an integer representing the current score.
 * @param fruitx A pointer to an integer representing the x-coordinate of the fruit.
 * @param fruity A pointer to an integer representing the y-coordinate of the fruit.
 */
void grow_snake(Point snake[], int* length, int* score, int* fruitx, int* fruity)
{
    Point old = snake[0];
    fruits(fruitx, fruity);
    *score += 5;
    for(int i = *length; i > 0; i--){
        snake[i] = snake[i-1];
    }
    snake[0] = old;
    (*length)++;
}

/**
 * @brief Controls the game logic for the snake game.
 *
 * This function handles the main game logic, including moving the snake,
 * checking for collisions, and handling fruit consumption.
 *
 * @param flag An integer flag indicating the direction of the snake's movement.
 *             If the flag is 0, the function returns immediately.
 * @param snake An array of Point structures representing the snake's body.
 * @param length A pointer to an integer representing the current length of the snake.
 * @param gameover A pointer to an integer that will be set to a non-zero value if the game is over.
 * @param score A pointer to an integer representing the player's current score.
 * @param fruitx A pointer to an integer representing the x-coordinate of the fruit.
 * @param fruity A pointer to an integer representing the y-coordinate of the fruit.
 */
void logic(int flag, Point snake[], int* length, int* gameover, int* score, int* fruitx, int* fruity)
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 250000000L;  // 250 milliseconds
    if(*score >= 25){
        ts.tv_nsec = 200000000L;  // 200 milliseconds
    }
    else if(*score >= 50){
        ts.tv_nsec = 150000000L;  // 150 milliseconds
    }
    else if(*score >= 75){
        ts.tv_nsec = 100000000L;  // 100 milliseconds
    }
    nanosleep(&ts, NULL);  // Pause to slow down the game
    if(flag == 0){
        return;
    }
    move_snake(snake, *length, flag);
    *gameover = check_collision(snake, *length);
    if (*gameover != 0) {
        return;
    }
    if (snake[*length-1].x == *fruitx && snake[*length-1].y == *fruity)
    {
        grow_snake(snake, length, score, fruitx, fruity);
    }
    if(*score == 100){
        *gameover = -1;
    }
}

/**
 * @brief Main file for the Snake game implementation.
 * This file contains the main function which initializes the game, handles the game loop,
 * and manages the game over state.
 *
 * The game is implemented using the ncurses library for handling the console display.
 * The snake's position, length, and the fruit's position are managed within the game loop.
 *
 * The main function performs the following tasks:
 * - Initializes the random number generator.
 * - Declares and initializes game variables.
 * - Initializes the ncurses library.
 * - Generates the initial position of the fruit.
 * - Sets the initial position and length of the snake.
 * - Enters the game loop where it:
 *   - Draws the game state.
 *   - Handles user input.
 *   - Updates the game logic.
 * - Enters the game over loop where it:
 *   - Displays the game over message and score.
 *   - Waits for the user to quit the game.
 * - Ends the ncurses session and exits the program.
 *
 * @return int Returns 0 upon successful completion.
 */
int main()
{
    struct timespec ts;
    srand((unsigned int)time(NULL));
    int length;
    int* length_ptr = &length;  
    int gameover = 0;
    int score = 0;
    int fruitx;
    int fruity;
    int flag = 0;
    Point snake[LENGTH_SNAKE] = {0};
    initscr();
    noecho();  
    curs_set(FALSE);  

    fruits(&fruitx,&fruity);

    snake[0].x = WIDTH / 2;
    snake[0].y = HEIGHT / 2;
    length = 1;
    while (gameover == 0)
    {
        draw(fruitx,fruity,length,snake,score);
        input(&flag);
        if(flag == -1){
            break;
        }
        logic(flag,snake,length_ptr,&gameover,&score,&fruitx,&fruity);
    }
    while(1){
        clear();
        start_color();
        init_pair(1,COLOR_RED,COLOR_BLACK);
        init_pair(2,COLOR_GREEN,COLOR_BLACK);
        switch (gameover) {
            case 1:
            if (has_colors()) {
                attron(COLOR_PAIR(1));
            }
            printw("You hit yourself\n");
            break;
            case 2:
            if (has_colors()) {
                attron(COLOR_PAIR(1));
            }
            printw("You hit the boundary\n");
            break;
            case -1:
            if (has_colors()) {
                attron(COLOR_PAIR(2));
            }
            printw("You are a winner\n");
            break;
            default:
            if (has_colors()) {
                attron(COLOR_PAIR(1));
            }
            printw("Game Over\n");
            break;
        }
        printw("Score = %d\n",score);
        printw("Press 'X' to quit the game");
        refresh();
        input(&flag);
        if(flag == -1){
            break;
        }
        ts.tv_sec = 1;
        ts.tv_nsec = 0;
        nanosleep(&ts, NULL);
    }
    endwin();
    return 0;
}
