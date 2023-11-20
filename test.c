#include <ncurses.h>
#include <curses.h>



int main( int argc, char *argv[])
{
   
    initscr();
    timeout( 0 );

    int y;
    int x;
    getmaxyx(stdscr, y, x);
   // mvprintw(5,5, "maxx = %d, maxy = %d", x, y );
   //  mvprintw(10,10, "COLS=%d, LINE=%d", COLS, LINES);
    box(stdscr,'#', '#');
    
    refresh();
    int ch;

    while ( true )
    {
       usleep();	
       ch = getch();
       mvaddch(10,10,ch | A_BOLD);
       refresh();
    }
    endwin();

    return 0;
}
