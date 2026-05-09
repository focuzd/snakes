#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define DUR 2e5

struct head {
    char c;
    int x;
    int y;
    int dir;
};

struct body {
    char c;
    int x;
    int y;
};

void move_head(struct head *h, int row, int col)
{
    mvaddch(h->y, h->x, ' ');

    if (h->dir == NORTH) {
        if (h->y == 0) h->y = row;
        else h->y -= 1;
    } else if (h->dir == EAST) {
        if (h->x == col) h->x = 0;
        else h->x += 1;
    } else if (h->dir == SOUTH) {
        if (h->y == row) h->y = 0;
        else h->y += 1;
    } else if (h->dir == WEST) {
        if (h->x == 0) h->x = col;
        else h->x -= 1;
    }

    mvaddch(h->y, h->x, h->c);
}


int main()
{
    char input;
    int row, col;
    int iter;
    srand(time(NULL));

    initscr();
    noecho();
    raw();
    
    getmaxyx(stdscr, row, col);
    struct head shead;
    shead.c = '#';
    shead.x = col/2;
    shead.y = row/2;
    shead.dir = EAST;

    mvaddch(row/2, col/2, shead.c);

    iter = 0;
    while (true) {
        usleep(DUR);
        /*
        if (input == 'a') 
            shead.dir = (4 + shead.dir - 1) % 4;

        if (input == 'd')
            shead.dir = (shead.dir + 1) % 4;
        */
        move_head(&shead, row, col);
        mvprintw(0, 0, "iter: %d", iter);
        mvprintw(1, 0, "x:%d, y:%d", shead.x, shead.y);
        refresh();

        iter++;
        if (iter == 100) break;

    }
   
    getch();
    endwin();
    return 0;
};

