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

    for(int i=2; i < snake.size(); i++)
    {
        if(snake[0].x==snake[i].x && snake[0].y == snake[i].y)
            return true;
    }

    return false;
}

bool yam(std::vector<point>const& snake, point food)
{
    if(snake[0].x==food.x && snake[0].y==food.y)
        return true;

    return false;
}

bool isOpposite(eCommand lhs, eCommand rhs )
{
    if(lhs == MOVE_LEFT && rhs == MOVE_RIGHT) return true;
    if(lhs == MOVE_RIGHT && rhs == MOVE_LEFT) return true;
    if(lhs == MOVE_UP && rhs == MOVE_DOWN) return true;
    if(lhs == MOVE_DOWN && rhs == MOVE_UP) return true;

    return false;
}

eCommand determinateCommand(eCommand last, eCommand prev )
{
    if(last == eCommand::NO_CHOISE) return prev;
    if (isOpposite(last, prev)) return prev;
    return last;
}

void initNcurses()
{
    initscr();
    nodelay(stdscr,true);
    keypad(stdscr,true);
    noecho();
    curs_set(0);
}

void shutdownNcurses()
{
    nodelay(stdscr,false);
    getch();
    endwin();
}

eCommand readLastCommand()
{
    int tmp=getch();
    switch(tmp)
    {
    case KEY_LEFT:      return MOVE_LEFT;
    case KEY_UP:        return MOVE_UP;
    case KEY_DOWN:      return MOVE_DOWN;
    case KEY_RIGHT:     return MOVE_RIGHT;
    case KEY_BACKSPACE: return OUT;
    case KEY_ENTER:     return PAUSE;
    case KEY_BTAB:      return SAVE;
    }
    return eCommand::NO_CHOISE;
}

void PlayingField(int maxWidth, int maxHeight)
{
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
}

void collectSnake(std::vector<point>& snake)
{
    for(int i=0; i<5; i++)
    {
      snake.push_back(point(40+i, 10));
    }
 }

snakeclass::snakeclass() :
  food(),
  score(0),
  delay(110000),
  command(NO_CHOISE)
{
    initNcurses();

    getmaxyx(stdscr, maxHeight, maxWidth);
    PlayingField( maxWidth, maxHeight);
    srand(time(NULL));
    putfood();
    collectSnake(snake);
    ///BEGIN
    for(int i=0; i<snake.size(); i++)
    {
        move(snake[i].y, snake[i].x);
        addch(kPartchar);
    }
    ///END

    ///BEGIN
    move(maxHeight-1,0);
    printw("%d", score);
    ///END

    refresh();
}

snakeclass::~snakeclass()
{
   shutdownNcurses();
}

void snakeclass::putfood()
{

    ///BEGIN point getNexFoodPosition(snake, maxWidth, maxHeight)
    point temp;
    while(true)
    {
        temp.x=rand()%maxWidth+1;
        temp.y=rand()%maxHeight+1;

        for(int i=0; i<snake.size(); i++)

            if(snake[i].x==temp.x && snake[i].y==temp.y)
                continue;

        if(temp.x>=maxWidth-2 || temp.y>=maxHeight-3)
            continue;
        break;
    }
    ///end getNexFoodPosition

    food = temp;
    move(food.y,food.x);
    addch(kFoo);
    refresh();
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

/// move command to parameters; rename command to direction
void snakeclass::movesnake()
{
    if(yam(snake,food))
    {
        snake.insert(snake.begin(), food);
        updateFood();
    }

    move(snake[snake.size()-1].y,snake[snake.size()-1].x);
    printw(" ");
    refresh();
    snake.pop_back();

    if(command==MOVE_LEFT)
    {
        snake.insert(snake.begin(),point(snake[0].x-1,snake[0].y));
    }
    else if(command==MOVE_RIGHT)
    {
        snake.insert(snake.begin(),point(snake[0].x+1,snake[0].y));
    }
    else if(command==MOVE_UP)
    {
        snake.insert(snake.begin(),point(snake[0].x,snake[0].y-1));
    }
    else if(command==MOVE_DOWN)
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

        auto newCommand = readLastCommand();
        command = determinateCommand(newCommand, command);

        switch (command) {
        case OUT:
            return;
        case NO_CHOISE:
            break;
        case MOVE_LEFT:
        case MOVE_RIGHT:
        case MOVE_UP:
        case MOVE_DOWN:
            movesnake();
            break;
        }

        usleep(delay);
    }
}
