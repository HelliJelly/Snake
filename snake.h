#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#ifndef SNAKE_H
#define SNAKE_H

using namespace std;

struct point
{
    int x,y;
    point(int col,int row):x(col),y(row){}
    point();
 };



bool collision(std::vector<point>const& snake, int maxWidth, int maxHeigh);
bool yam(std::vector<point>const& snake, point food);
void initialization();
class snakeclass
{
public:
    snakeclass();
    ~snakeclass();
    void start();

private:
    int maxWidth, maxHeight;
    int score, delay;
    char direction;
    point food;
    std::vector<point> snake;

private:

    void putfood();
    void movesnake();
    void updateFood();
};

#endif
