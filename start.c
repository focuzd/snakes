#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define DUR 2e5

struct Head {
    char c;
    int x;
    int y;
    int dir;
};

struct Body {
    char c;
    int x;
    int y;
};

struct Snake{
    int length;
    struct Head *head;
    struct Body *body;
};

void move_head(struct Head *h, int row, int col)
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

bool is_occupied(struct Snake *s, int y, int x) {
    if (s->head->x == x && s->head->y == y)
        return true;

    for (int i = 0; i < s->length-1; i++) {
        if(s->body[i].x == x && s->body[i].y == y) 
            return true;
    }
    return false;
}

void spawn_food(int *y, int *x, int is_eaten, int row, int col, struct Snake *s) {
    if (!is_eaten)
        return;

    mvaddch(*y, *x, ' ');
    char food_char = '*';

    do {
    *x = rand() % col;
    *y = rand() % row;
    } while (is_occupied(s, *y, *x));
    
    mvaddch(*y, *x, food_char);
}

void spawn_body(struct Snake *s) {
    if (length == 1) {
        
}

int main()
{
    char input;
    int row, col, food_x, food_y;
    bool is_eaten;
    srand(time(NULL));

    initscr();
    noecho();
    raw();
    
    getmaxyx(stdscr, row, col);
    struct Head shead;
    shead.c = '#';
    shead.x = col/2;
    shead.y = row/2;
    shead.dir = EAST;

    struct Snake s;
    s.head = &shead;
    s.body = malloc(sizeof(struct body) * 20);
    s.length = 1;

    food_x = food_y = 0;
    mvaddch(row/2, col/2, shead.c);
    mvaddch(food_x, food_y, '*');
    nodelay(stdscr, true);

    while ((input = getch()) != 'q') {
        
        is_eaten = false;
        if (input == 'a') 
            shead.dir = (4 + shead.dir - 1) % 4;

        if (input == 'd')
            shead.dir = (shead.dir + 1) % 4;

        move_head(&shead, row, col);
        mvprintw(1, 0, "x:%d, y:%d", shead.x, shead.y);

        if (food_x == shead.x && food_y == shead.y) {
            is_eaten = true;
            spawn_body(&s);
        }
        spawn_food(&food_y, &food_x, is_eaten, row, col, &s);

        usleep(DUR);
        refresh();
    }
   
    getch();
    endwin();
    return 0;
};

