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

struct Body {
    int x;
    int y;
};

struct Snake{
    int length;
    char chead;
    char cbody;
    int dir;
    struct Body *body;
};

struct Snake* init_snake(void) {
    struct Snake *s = (struct Snake*)malloc(sizeof(struct Snake));
    s->length = 1;
    s->chead = '#';
    s->cbody = '0';
    s->body = (struct Body*)malloc(50*(sizeof(struct Body)));
    s->body[0].x = s->body[0].y = 0;
    s->dir = NORTH;

    return s;
}

void move_head(struct Snake *s, int row, int col)
{
    mvaddch(s->body[0].y, s->body[0].x, ' ');
    struct Body *head = &(s->body[0]);

    if (s->dir == NORTH) {
        if (head->y == 0) head->y = row;
        else head->y -= 1;
    } else if (s->dir == EAST) {
        if (head->x == col) head->x = 0;
        else head->x += 1;
    } else if (s->dir == SOUTH) {
        if (head->y == row) head->y = 0;
        else head->y += 1;
    } else if (s->dir == WEST) {
        if (head->x == 0) head->x = col;
        else head->x -= 1;
    }

    mvaddch(head->y, head->x, s->chead);
}

bool is_occupied(struct Snake *s, int y, int x) {
    for (int i = 0; i < s->length; i++) {
        if(s->body[i].x == x && s->body[i].y == y) 
            return true;
    }

    return false;
}

void spawn_food(int *y, int *x, int row, int col, struct Snake *s) {
    mvaddch(*y, *x, ' ');
    char food_char = '*';

    do {
    *x = rand() % col;
    *y = rand() % row;
    } while (is_occupied(s, *y, *x));
    
    mvaddch(*y, *x, food_char);
}

void update_dir(struct Snake *s, char input) {
    
}

void spawn_body(struct Snake *s){ 
}

int main()
{
    char input;
    int row, col, food_x, food_y;
    srand(time(NULL));

    initscr();
    noecho();
    raw();
    
    getmaxyx(stdscr, row, col);
    struct Snake *s = init_snake();

    food_x = food_y = 0;
    mvaddch(row/2, col/2, s->chead);
    mvaddch(food_x, food_y, '*');
    nodelay(stdscr, true);

    while ((input = getch()) != 'q') {
        update_dir(s, input);

        move_head(s, row, col);
        mvprintw(row, col, "x:%d, y:%d", s->body[0].x, s->body[0].y);

        if (food_x == s->body[0].x && food_y == s->body[0].y) {
            spawn_food(&food_y, &food_x, row, col, s);
            spawn_body(s);
        }

        usleep(DUR);
        refresh();
    }
   
    getch();
    endwin();
    return 0;
};

