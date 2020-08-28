#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#ifndef SNAKE_H
#define SNAKE_H

using namespace std;

///BEGIN move to types file
enum eCommand {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    OUT,
    PAUSE,
    SAVE,
    NO_CHOISE
};

struct point
{
    int x,y;
    point(int col,int row):x(col),y(row){}
    point();
    /// operator==
 };
///END

///MOVE to separete file
bool collision(std::vector<point>const& snake, int maxWidth, int maxHeigh);
bool yam(std::vector<point>const& snake, point food);
bool isOpposite(eCommand lhs, eCommand rhs );
eCommand determinateCommand(eCommand last, eCommand prev );
void playingField(int maxWidth, int maxHeight);
void collectSnake(std::vector<point>& snake);

///move to file ncursesHelper
void initNcurses();
void shutdownNcurses();
eCommand readLastCommand();


class snakeclass
{
public:
    snakeclass();
    ~snakeclass();
    void start();

private:
    int maxWidth, maxHeight;
    int score, delay;
    eCommand command;
    point food;
    std::vector<point> snake;

private:

    void putfood();
    void movesnake();
    void updateFood();
};

#endif
