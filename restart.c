#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
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

struct Snake* init_snake(int row, int col) {
    struct Snake *s = (struct Snake*)malloc(sizeof(struct Snake));
    s->length = 1;
    s->chead = '#';
    s->cbody = '0';
    s->body = (struct Body*)malloc(50*(sizeof(struct Body)));
    s->body[0].x = col/2;
    s->body[0].y = row/2;
    s->dir = UP;

    return s;
}

void move_head(struct Snake *s, bool has_eaten, int row, int col)
{
    if (!has_eaten) 
        mvaddch(s->body[s->length-1].y, s->body[s->length-1].x, ' ');
    
    struct Body *head = &(s->body[0]);

    for (int i = s->length-1; i >= 1; i--) {
        s->body[i].x = s->body[i-1].x;
        s->body[i].y = s->body[i-1].y;
    }

    if (s->dir == UP) {
        if (head->y == 0) head->y = row;
        else head->y -= 1;
    } else if (s->dir == RIGHT) {
        if (head->x == col-1) head->x = 0;
        else head->x += 1;
    } else if (s->dir == DOWN) {
        if (head->y == row-1) head->y = 0;
        else head->y += 1;
    } else if (s->dir == LEFT) {
        if (head->x == 0) head->x = col;
        else head->x -= 1;
    }

    mvaddch(head->y, head->x, s->chead);
    for (int i = 1; i < s->length; i++) {
        mvaddch(s->body[i].y, s->body[i].x, s->cbody);
    }
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
    switch (s->dir) {
        case UP: case DOWN:
            if (input == 'a') s->dir = LEFT;
            if (input == 'd') s->dir = RIGHT;
            break;
        
        case RIGHT: case LEFT:
            if (input == 'w') s->dir = UP;
            if (input == 's') s->dir = DOWN;
            break;
    }
}

void spawn_body(struct Snake *s){ 
    // handle the case where the length is greater than or equal to the current size of body array
    
    s->body[s->length].x = s->body[s->length-1].x;
    s->body[s->length].y = s->body[s->length-1].y;
    mvaddch(s->body[s->length].y, s->body[s->length].x, s->cbody);
    s->length += 1;
}

int main()
{
    char input;
    int row, col, food_x, food_y;
    bool has_eaten;
    srand(time(NULL));

    initscr();
    noecho();
    raw();
    
    getmaxyx(stdscr, row, col);
    struct Snake *s = init_snake(row, col);

    food_x = food_y = 0;
    mvaddch(row/2, col/2, s->chead);
    mvaddch(food_x, food_y, '*');
    nodelay(stdscr, true);

    while ((input = getch()) != 'q') {
        update_dir(s, input);
        has_eaten = false;
        mvprintw(row - 1, 0, "x:%d, y:%d", s->body[0].x, s->body[0].y);

        if (food_x == s->body[0].x && food_y == s->body[0].y) {
            spawn_food(&food_y, &food_x, row, col, s);
            spawn_body(s);
            has_eaten = true;
        }
        move_head(s, has_eaten, row, col); 
        // updating head position after spawing the last body part saves us a lot of calcuation to predict 
        // position of the last body part

        usleep(DUR);
        refresh();
    }
   
    getch();
    endwin();
    return 0;
};

