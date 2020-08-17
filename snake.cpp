#include<snake.h>
#include<unistd.h>

static const char kFoo = '*';
static const char kPartchar='x';
static const char kOldachar=(char)219;

point::point()
{
    x=0;
    y=0;
}

bool collision( std::vector<point>const& snake, int maxWidth, int maxHeight)
{
    if(snake[0].x==0 || snake[0].x==maxWidth-1 || snake[0].y==0 || snake[0].y==maxHeight-2)
        return true;

    auto size = snake.size();

    for(int i=2; i < size; i++)
    {
        if(snake[0].x==snake[i].x && snake[0].y==snake[i].y)
            return true;
    }

    return false;
}

snakeclass::snakeclass() :
  food(),
  score(0),
  delay(110000),
  direction('1')
{
    //////////////////////////////////////////////////////////
    initscr();
    nodelay(stdscr,true);
    keypad(stdscr,true);
    noecho();
    curs_set(0);
    //////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////
    getmaxyx(stdscr, maxHeight, maxWidth);

    for(int i=0; i<maxWidth-1; i++)
    {
        move(0,i);
        addch(kOldachar);
    }

    for(int i=0; i<maxHeight-1; i++)
    {
        move(i,0);
        addch(kOldachar);
    }

    for(int i=0; i<maxWidth-1; i++)
    {
        move(maxHeight-2, i);
        addch(kOldachar);
    }

    for(int i=0; i<maxHeight-1; i++)
    {
        move(i,maxWidth-2);
        addch(kOldachar);
    }
    ///////////////////////////////////////////////////////////////////

    srand(time(NULL));
    putfood();

    for(int i=0; i<5; i++)
    {
        snake.push_back(point(40+i, 10));
    }

    for(int i=0; i<snake.size(); i++)
    {
        move(snake[i].y, snake[i].x);
        addch(kPartchar);
    }
    refresh();

    move(maxHeight-1,0);
    printw("%d", score);
}

snakeclass::~snakeclass()
{
    nodelay(stdscr,false);
    getch();
    endwin();

}

void snakeclass::putfood()
{
    while(1)
    {
        int tmpx=rand()%maxWidth+1;
        int tmpy=rand()%maxHeight+1;

        for(int i=0; i<snake.size(); i++)

            if(snake[i].x==tmpx && snake[i].y==tmpy)
                continue;

        if(tmpx>=maxWidth-2 || tmpy>=maxHeight-3)
            continue;

        food.x=tmpx;
        food.y=tmpy;
        break;

    }

    move(food.y,food.x);
    addch(kFoo);
    refresh();
}

bool yam(std::vector<point>const& snake, point food)
{

    if(snake[0].x==food.x && snake[0].y==food.y)
        return true;

    return false;

}

void snakeclass::updateFood()
{
    putfood();
    score+=10;
    move(maxHeight-1,0);
    printw("%d",score);
    if((score%100)==0)
        delay-=10000;
}

void snakeclass::movesnake()
{
    int tmp=getch();

    switch(tmp)

    {
    case KEY_LEFT:
        if(direction!='r')
            direction='l';
        break;

    case KEY_UP:
        if(direction!='d')
            direction='u';
        break;

    case KEY_DOWN:
        if(direction!='u')
            direction='d';
        break;

    case KEY_RIGHT:
        if(direction!='l')
            direction='r';
        break;

    case KEY_BACKSPACE:
        direction='q';
        break;

    }

    if(!yam(snake,food))
    {
        move(snake[snake.size()-1].y,snake[snake.size()-1].x);
        printw(" ");
        refresh();
        if (!snake.empty())
            snake.pop_back();
    }
    else
    {
        updateFood();
    }


    if(direction=='l')

    {
        snake.insert(snake.begin(),point(snake[0].x-1,snake[0].y));
    }

    else if(direction=='r')

    {
        snake.insert(snake.begin(),point(snake[0].x+1,snake[0].y));
    }

    else if(direction=='u')
    {
        snake.insert(snake.begin(),point(snake[0].x,snake[0].y-1));
    }

    else if(direction=='d')
    {
        snake.insert(snake.begin(),  point(snake[0].x, snake[0].y+1));
    }

    move(snake[0].y,snake[0].x);
    addch(kPartchar);
    refresh();
}

void snakeclass::start()
{
    while(true)
    {
        if(collision(snake, maxWidth, maxHeight ))
        {
            move(12,36);
            printw("Game_over\n");
            move(13,36);
            printw("Your score %d \n", score);

            break;
        }

        movesnake();
        if(direction=='q')
        {
            break;
        }

        usleep(delay);


    }
}
