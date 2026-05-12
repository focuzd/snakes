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
    int capacity;
    chtype chead;
    chtype cbody;
    int dir;
    struct Body *body;
};

struct Snake* init_snake(int row, int col) {
    struct Snake *s = (struct Snake*)malloc(sizeof(struct Snake));
    s->length = 1;
    s->capacity = 10;
    s->chead = ACS_DIAMOND;
    s->cbody = ACS_BULLET;
    s->body = (struct Body*)malloc(s->capacity * (sizeof(struct Body)));
    s->body[0].x = col/2;
    s->body[0].y = row/2;
    s->dir = UP;

    return s;
}

void print_screen(char *filename) {
    FILE *fp; 
    int ch;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("failed to open file\n");
        exit(1);
    }

    while ((ch = fgetc(fp)) != EOF) 
        addch(ch);

    refresh();
    getch();
    clear();
    fclose(fp);
    refresh();
}

WINDOW *create_window(int height, int width, int starty, int startx)
{
    WINDOW *local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);

    wrefresh(local_win);
    return local_win;
}

void move_head(WINDOW *win, struct Snake *s, bool has_eaten, int row, int col)
{
    if (!has_eaten)
        mvwaddch(win, s->body[s->length-1].y, s->body[s->length-1].x, ' ');
    
    struct Body *head = &(s->body[0]);

    for (int i = s->length-1; i >= 1; i--) {
        s->body[i].x = s->body[i-1].x;
        s->body[i].y = s->body[i-1].y;
    }

    if (s->dir == UP) {
        if (head->y == 1) head->y = row - 2;
        else head->y -= 1;
    } else if (s->dir == RIGHT) {
        if (head->x == col - 2) head->x = 1;
        else head->x += 1;
    } else if (s->dir == DOWN) {
        if (head->y == row - 2) head->y = 1;
        else head->y += 1;
    } else if (s->dir == LEFT) {
        if (head->x == 1) head->x = col - 2;
        else head->x -= 1;
    }

    mvwaddch(win, head->y, head->x, s->chead);
    for (int i = 1; i < s->length; i++) {
        mvwaddch(win, s->body[i].y, s->body[i].x, s->cbody);
    }
}

bool is_occupied(struct Snake *s, int y, int x, int start) {
    for (int i = start; i < s->length; i++) {
        if(s->body[i].x == x && s->body[i].y == y) 
            return true;
    }

    return false;
}

void spawn_food(WINDOW *win, int *y, int *x, struct Snake *s, int row, int col) {
    mvwaddch(win, *y, *x, ' ');
    char food_char = '*';

    do {
    *x = 1 + rand() % (col - 2);
    *y = 1 + rand() % (row - 2);
    } while (is_occupied(s, *y, *x, 0));
    
    mvwaddch(win, *y, *x, food_char);
}

void update_dir(struct Snake *s, int input) {
    switch (s->dir) {
        case UP: case DOWN:
            if (input == 'a' || input == KEY_LEFT) s->dir = LEFT;
            if (input == 'd' || input == KEY_RIGHT) s->dir = RIGHT;
            break;
        
        case RIGHT: case LEFT:
            if (input == 'w' || input == KEY_UP) s->dir = UP;
            if (input == 's' || input == KEY_DOWN) s->dir = DOWN;
            break;
    }
}

void spawn_body(WINDOW *win, struct Snake *s){ 
    // handle the case where the length is greater than or equal to the current size of body array
    s->length += 1;

    if (s->length == s->capacity) {
        s->capacity = s->length * 2;
        s->body = (struct Body *)realloc(s->body, 2*s->length*sizeof(struct Body));
        if (s->body == NULL) {
            exit(1);
        }
    }    
}

void delete_window(WINDOW *win) {
    wclear(win);
    wrefresh(win);
    delwin(win);
}

int main()
{
    int input, row, col, food_x, food_y, score;
    int sw_row, sw_col, pg_row, pg_col;
    bool has_eaten;
    struct Snake *s;
    WINDOW *playground, *score_win;

    srand(time(NULL));
    initscr();
    noecho();
    cbreak();

    print_screen("start_screen.txt");
    getmaxyx(stdscr, row, col);
    playground = create_window(20, 50, (row - 20) / 2, (col - 50) / 2);
    score_win = create_window(3, 20, 1, (col - 20) / 2);
    getmaxyx(playground, pg_row, pg_col);
    getmaxyx(score_win, sw_row, sw_col);
    
    score = 0;
    food_x = food_y = 1;
    s = init_snake(pg_row, pg_col);
    mvwaddch(playground, pg_row / 2, pg_col / 2, s->chead);
    mvwprintw(score_win, sw_row / 2, (sw_col - 8) / 2, "Score: %d", score);
    spawn_food(playground, &food_y, &food_x, s, pg_row, pg_col);
    nodelay(playground, true);
    keypad(playground, true);
    wrefresh(score_win);

    while ((input = wgetch(playground)) != 'q') {
        update_dir(s, input);
        has_eaten = false;

        if (is_occupied(s, s->body[0].y, s->body[0].x, 1)) 
            break;

        if (food_x == s->body[0].x && food_y == s->body[0].y) {
            spawn_food(playground, &food_y, &food_x, s, pg_row, pg_col);
            spawn_body(playground, s);
            has_eaten = true;
            score++;
            mvwprintw(score_win, sw_row / 2, (sw_col - 8) / 2, "Score: %d", score);  
            wrefresh(score_win);
        }
        move_head(playground, s, has_eaten, pg_row, pg_col); 
        // updating head position after spawing the last body part saves us a lot of calcuation to predict 
        // position of the last body part

        usleep(DUR);
        wrefresh(playground);
    }

    delete_window(playground);
    delete_window(score_win);

    free(s->body);
    free(s);
    print_screen("game_over.txt");
    endwin();
    return 0;
}
