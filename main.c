#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


enum dir {
 UP=0,
 RIGHT,
 DOWN,
 LEFT
};

#define REVERSE_DIR(i) ((i+2)%4)
#define RIGHT_DIR(i) ((i+1)%4)
#define LEFT_DIR(i) ((i+3)%4)



int MAP_ROWS;
int MAP_COLS;
int TOTAL_GRIDS;
int scores = 0;
int is_ok = 1;
int ticks = 0;

int dir_to[4][2] = 
{
	{0, -1},
	{1, 0},
	{0, 1},
	{-1,0}
};


const int SNAKE_MAX_LENGTH = 100 * 100;
struct _snake_node {
   enum dir dir;
   int length; 
   int pos[1000*1000*2];
};
typedef struct _snake_node snake_node;

struct _snake_food {
   int x;
   int y;
   int score;
   int color;
};
typedef struct _snake_food snake_food;

snake_food food;
snake_node snake;


int is_wall_grid( int x, int y )
{
    if ( x == 0 || x == MAP_COLS - 1)
	    return 1;
    if ( y == 0 || y == MAP_ROWS - 1)
	    return 1;

    return 0;
}

int is_snake_body_grid( int x, int y)
{

    for ( int i = 0; i < snake.length; ++i) {
        int sx = snake.pos[i * 2];
        int sy = snake.pos[i * 2 + 1];

	if ( sx == x && sy == y)
		return 1;	
    }
   
    return 0;
}
int snake_hit_self(int x, int y)
{
    
    for (int i = 1; i < snake.length - 1; ++i) {
         int sx = snake.pos[i * 2];
	 int sy = snake.pos[i * 2 + 1];

	 if (sx == x && sy == y)
		 return 1;
    }

    return 0;

}

void gen_food_pos( void )
{
   if ( TOTAL_GRIDS == snake.length )
	   return ;

   int remain_grids = TOTAL_GRIDS - snake.length;
   int sed = rand() % remain_grids;


   if (sed == 0)
	   sed = remain_grids;

   int cnt = 0;
   for ( int i = 0; i < TOTAL_GRIDS; ++i) {
       int x = i % (MAP_COLS - 2);
       int y = i / (MAP_COLS - 2);

       if ( !is_snake_body_grid(x + 1, y + 1) ) {
           ++cnt;
       }
       if ( cnt == sed )
       {
           food.x = x + 1;
	   food.y = y + 1;
           food.color = 3;
	   food.score = 5;

	   if ( sed % 2 == 0) {
		food.color++;
	        food.score *= 2;
	   }
	   if ( sed % 4 == 0) {
                food.color++;
		food.score *= 2;
	   }

	   break;
       }
   }
}
void init_snake_food()
{
   srand( time(NULL) );

   gen_food_pos();
   
}
void init_snake( int x, int y, enum dir s_dir)
{
    snake.pos[0] = x;
    snake.pos[1] = y;
    snake.length = 1;
    snake.dir = s_dir;
}
void init_ncurse_color(void)
{
    start_color();

    init_color(16, 255, 0 ,255);

    init_pair(1, COLOR_RED,COLOR_RED);
    init_pair(2, COLOR_BLUE,COLOR_BLUE);
    init_pair(3, COLOR_GREEN, COLOR_GREEN);
    init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(5, 16, 16);
    init_pair(6, COLOR_WHITE, COLOR_WHITE);
}
void init_ncurses_settings()
{
    initscr();
    init_ncurse_color();
    noecho();
    curs_set( 0 );
    timeout( 0 );
    // resizeterm( 103, 103);



}
void init_map_area(int width, int height)
{
    if ( width  > COLS )
	    width = COLS ;
    if ( height + 3 > LINES )
	    height = LINES - 3;
    MAP_ROWS = height;
    MAP_COLS = width;
    TOTAL_GRIDS = ( MAP_ROWS - 2) * ( MAP_COLS - 2 );
   
    refresh();

}
void draw_wall( void )
{
   attron(COLOR_PAIR(6) );

    for ( int i = 0; i < MAP_COLS; ++i ) {
        for ( int j = 0; j < MAP_ROWS; ++j)
		if ( !i || !j || j == MAP_ROWS - 1 || i == MAP_COLS - 1 ) {
		    mvaddch(j, i, ' '); 
		}
    }

    attroff(COLOR_PAIR(6) );
}
void draw_food( void )
{
    attron( COLOR_PAIR(food.color) );

    mvaddch(food.y, food.x, '#');

    attroff( COLOR_PAIR(food.color) );
}

void draw_snake( void )
{
    attron(COLOR_PAIR(1));
   
    mvaddch(snake.pos[1], snake.pos[0],' ');

    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(2));
    for (int i = 1;i < snake.length; ++i) {
       
        mvaddch(snake.pos[ i * 2 + 1], snake.pos[i * 2], ' ');	    
    }
    attroff(COLOR_PAIR(2));
    
} 

int process_input(int ch)
{
    int is_mv_bt = 1;
    enum dir to_dir = snake.dir;
    switch ( ch )
    {
         case 'w':
         case 'W':
		 to_dir = UP;
		 break;
	 case 'a':
         case 'A':
		 to_dir = LEFT;
		 break;
	 case 's':
	 case 'S':
		 to_dir = DOWN;
		 break;
	 case 'd':
	 case 'D':
		 to_dir = RIGHT;
		 break;
	 case 'q':
         case 'Q':
		 is_ok = 0;
	default:
		 is_mv_bt = 0;
		 break;
    }
    if ( is_mv_bt && REVERSE_DIR(snake.dir) != to_dir ) {
    	snake.dir = to_dir;    
        return  1;
    }

    return 0;
}
void draw_tips( void )
{
   mvprintw( MAP_ROWS, 0, "food's postition (%d,%d)",food.x, food.y);
   mvprintw( MAP_ROWS + 1, 0, "snake's length: %d, scores: %d", snake.length, scores);

}


void draw_frame( void )
{
  clear();
  draw_wall();
  draw_food();
  draw_snake();
  draw_tips();
  refresh();
}

enum snake_status {
    GET_FOOD,
    NORMAL,
    HIT_WALL,
    HIT_SELF,
};


int check_collision( void )
{
    int nx = snake.pos[0] + dir_to[snake.dir][0];
    int ny = snake.pos[1] + dir_to[snake.dir][1];

    if ( is_wall_grid(nx, ny) )
	    return HIT_WALL;
    if ( snake_hit_self(nx, ny) )
	    return HIT_SELF;

    if ( nx == food.x && ny == food.y ) {
	return GET_FOOD;
    }
    else {
	return NORMAL;
    }
}
void update_snake_and_food( enum snake_status ss)
{
    if ( ss > NORMAL)
	    return ;

    int nx = snake.pos[0] + dir_to[snake.dir][0];
    int ny = snake.pos[1] + dir_to[snake.dir][1];

    memmove(&snake.pos[2], &snake.pos[0], sizeof(int) * snake.length * 2 );
    snake.pos[0] = nx;
    snake.pos[1] = ny;

   if ( ss == GET_FOOD ) {
      scores += food.score;
      snake.length++;
      gen_food_pos();
   } 

}
int do_tick()
{
    if ( ++ticks > 30 ) {
        ticks = 0;
        
	enum snake_status st = check_collision( );
	update_snake_and_food ( st );

	draw_frame();
	return st > NORMAL ? 0 : 1;
    }
    return 1;
}

int main( int argc, char *argv[])
{

   init_ncurses_settings();
   init_map_area( 100, 80);
   init_snake(5, 5,RIGHT );
   init_snake_food();

   int ch;

   while ( is_ok )
   {
       is_ok = do_tick();
       usleep(10000);
       ch = 0;
       ch = getch(); 
       if (  process_input(ch) ) {
           enum snake_status st = check_collision();
	   is_ok = st > NORMAL ? 0 : 1;
           update_snake_and_food( st );
           draw_frame();
       }
   }
   endwin();



    return 0;
}
