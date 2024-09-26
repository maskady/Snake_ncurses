#include "draw.h"
#include <ncurses.h>
#include "constants.h"


/**
 * @brief Draws the border of the game area.
 *
 * This function iterates through the game area and prints a border using the
 * '#' character. The border is drawn along the edges of the area defined by
 * HEIGHT and WIDTH. The interior of the game area is filled with spaces.
 *
 * @note The function uses the `printw` function to print characters, which
 *       suggests it is intended to be used with the ncurses library.
 */
void draw_border()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (i == 0 || i == WIDTH - 1 || j == 0 || j == HEIGHT - 1)
            {
                printw("#");
            }
            else
            {
                printw(" ");
            }
        }
        printw("\n");
    }
}

/**
 * @brief Draws the snake on the screen.
 *
 * This function iterates through the snake's body points and draws each segment
 * at the specified coordinates using the `move` and `printw` functions.
 *
 * @param snake Pointer to an array of Point structures representing the snake's body.
 * @param length The number of segments in the snake's body.
 */
void draw_snake(const Point* snake, int length)
{
    for (int k = 0; k < length; k++)
    {
        move(snake[k].x, snake[k].y);
        printw("0");
    }
}

/**
 * @brief Draws a fruit at the specified coordinates on the screen.
 *
 * This function moves the cursor to the specified (fruitx, fruity) coordinates
 * and prints a '*' character to represent a fruit.
 *
 * @param fruitx The x-coordinate where the fruit should be drawn.
 * @param fruity The y-coordinate where the fruit should be drawn.
 */
void draw_fruit(int fruitx, int fruity)
{
    move(fruitx, fruity);
    printw("*");
}

/**
 * @brief Draws the current score and a quit message on the screen.
 *
 * This function moves the cursor to the bottom of the screen and prints the
 * current score followed by a message instructing the user to press 'X' to quit the game.
 *
 * @param score The current score to be displayed.
 */
void draw_score(int score)
{
    move(HEIGHT, 0);
    printw("Score = %d", score);
    printw("\nPress 'X' to quit the game\n");
}

/**
 * @brief Displays the footer information for the Snake Game.
 *
 * This function prints the instructions and credits for the Snake Game
 * to the screen. It includes the following information:
 * - Welcome message
 * - Instructions on how to move the snake using arrow keys
 * - Objective of eating the fruit to grow and score points
 * - Warning to avoid running into walls or the snake itself
 * - Credits to the developer, Florent Delalande
 */
void draw_footer()
{
    printw("\nWelcome to the Snake Game!\n");
    printw("Use the arrow keys to move the snake.\n");
    printw("Eat the fruit (*) to grow and score points.\n");
    printw("Avoid running into the walls or the snake itself.\n");
    printw("It has been developed by Florent Delalande.\n\n\n");
}

/**
 * @brief Draws the game elements on the screen.
 *
 * This function clears the screen, draws the game border, the snake, the fruit, 
 * and the current score, then refreshes the display.
 *
 * @param fruitx The x-coordinate of the fruit.
 * @param fruity The y-coordinate of the fruit.
 * @param length The length of the snake.
 * @param snake A pointer to an array of Points representing the snake's body.
 * @param score The current score of the game.
 */
void draw(int fruitx, int fruity, int length, const Point* snake, int score)
{
    clear();  // Efface l'écran avant de redessiner
    draw_border();
    draw_snake(snake, length);
    draw_fruit(fruitx, fruity);
    draw_score(score);
    draw_footer();
    refresh();  // Met à jour l'affichage à l'écran
}