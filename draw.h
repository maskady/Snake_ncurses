#include "point.h"
#ifndef DRAW_H
#define DRAW_H

void draw_border();
void draw_snake(const Point* snake, int length);
void draw_fruit(int fruitx, int fruity);
void draw_score(int score);
void draw_footer();
void draw(int fruitx, int fruity, int length, const Point* snake, int score);

#endif