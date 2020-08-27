
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>


#define X 100
#define Y 50


typedef struct {
    int x;
    int y;
    int vx;
    int vy;
    int len;
}SNAKE;


int die = 0;
int num_of_obs = 10;
int stage[X][Y] = {0};
int next_stage[X][Y] = {0};
SNAKE snake = {X/2, Y/2, 1, 0};

static struct termios old, current;
static int read_char = -1;



void open_termios(void)
{
    tcgetattr(0, &old);
    current = old;
    current.c_lflag &= ~ICANON;
    current.c_lflag &= ~ECHO;
    current.c_lflag &= ~ISIG;
    current.c_cc[VMIN]  = 0;
    current.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &current);
}



void close_termios(void)
{
    tcsetattr(0, TCSANOW, &old);
}


int kbhit(void)
{
char ch;
int nread;

    if(read_char != -1){
        return 1;
    }

    current.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &current);
    nread = read(0, &ch, 1);
    current.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &current);

    if(nread == 1){
        read_char = ch;
        return 1;
    }

    return 0;

}



char getch(void)
{
char ch;
    if(read_char != -1){
        ch = read_char;
        read_char = -1;
        return ch;
    }

    read(0, &ch, 1);
    return ch;
}




void init_obs()
{
    for(int i = 0; i < num_of_obs; ++i)
    {
        int x = rand()%(X-4);
        int y = rand()%(Y-4);

        if(rand()%2){
            stage[x][y] = 2;
            stage[x+1][y] = 2;
            stage[x+2][y] = 2;
            stage[x+3][y] = 2;

        }
        else{
            stage[x][y] = 2;
            stage[x][y+1] = 2;
            stage[x][y+2] = 2;
            stage[x][y+3] = 2;
        }
    }

}


void init_stage(void)
{
    init_obs();
    for(int x = 0; x < X; ++x){
        stage[x][0] = stage[x][Y-1] = 3;
    }
    for(int y = 0; y < Y; ++y){
        stage[0][y] = stage[X-1][y] = 4;
    }
}



void set_obs(void)
{
    int x = rand()%(X-4);
    int y = rand()%(Y-4);

    if(rand()%2){
        stage[x][y] = 2;
        stage[x+1][y] = 2;
        stage[x+2][y] = 2;
        stage[x+3][y] = 2;

    }
    else{
        stage[x][y] = 2;
        stage[x][y+1] = 2;
        stage[x][y+2] = 2;
        stage[x][y+3] = 2;
    }
    ++num_of_obs;
}



void next_snake(void)
{
    if(kbhit()){
        char ch = getch();
        switch (ch) {
            case 'a': snake.vx = -1; snake.vy = 0; break;
            case 'f': snake.vx = 1 ; snake.vy = 0; break;
            case 'e': snake.vx = 0 ; snake.vy = -1; break;
            case 'd': snake.vx = 0 ; snake.vy = 1; break;
            case 'q': die = 1; break;
        }

    }

    snake.x += snake.vx;
    snake.y += snake.vy;

    if(stage[snake.x][snake.y] == 2)
        die = 1;

    if(snake.x < 0 || snake.x > X || snake.y < 0 || snake.y > Y)
        die = 1;

}



int next(void)
{
    next_snake();

    if(die)
        return 0;

    memcpy(next_stage, stage, sizeof(stage));
    next_stage[snake.x][snake.y] = 1;

    return 1;
}



void display(void)
{
    for(int y = 0; y < Y; ++y){
        for(int x = 0; x < X; ++x){
            if(!next_stage[x][y])
                printf(" ");
            else if(next_stage[x][y] == 1)
                printf("+");
            else if(next_stage[x][y] == 2)
                printf("o");
            else if(next_stage[x][y] == 3)
                printf("-");

            else if(next_stage[x][y] == 4)
                printf("|");
        }
        printf("\n");
    }
    printf("score = %d\n", num_of_obs);
}




int main(void)
{
int counter = 0;
int time = 1000000 * 0.1;

    open_termios();

    init_stage();

    while(1){
        if(counter % 10 == 0){
            set_obs();
        }

        if(!next())
            break;

        display();
        ++counter;
        usleep(time);
        system("clear");

    }
    printf("game over\n");
    printf("score = %d\n", num_of_obs);
    close_termios();

    return 0;
}
