#include <iostream>
#include <ncurses.h>
#include <sstream>


void clear_refresh()
{
    wclear(stdscr);
    wrefresh(stdscr);
}

void add_trailing_zeros(std::string &str)
{
    if (!(str.find(".") != std::string::npos)) {
        str += ".00";
    }
}

