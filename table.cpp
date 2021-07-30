#include <iostream>
#include <vector>
#include <ncurses.h>

#include "get.h"
#include "file.h"

extern int longestDate;
extern int longestType;
extern int longestCategory;
extern int longestAmount;
extern int longestDescription;
extern int numEntries;
extern int amount;
extern std::vector<std::string> allEntries;
extern std::vector<std::string> allEntriesSpaces;
extern std::vector<std::string> specificMonthEntries;

void find_longest(int par_month = 0)
{
    /* These are initialized here so that the lines under the header gets redrawn
       if not the old longest values are changed, so if a long entry is removed the
       lines will still stay long unless something longer comes along to make it longer */
    longestDate = 0;
    longestType = 0;
    longestCategory = 0;
    longestAmount = 0;
    longestDescription = 0;
    /* FIX THIS */
    for (auto entry : allEntries) {
        if ((get_month(entry) == par_month) || (par_month == 0)) {
            int date = get_date(entry).length();
            int type = get_type(entry).length();
            int category = get_category(entry).length();
            int amount = get_amount_line_string(entry).length();
            int description = get_description(entry).length();
            if (date > longestDate) {
                longestDate = date;
            }
            if (type > longestType) {
                longestType = type;
            }
            if (category > longestCategory) {
                longestCategory = category;
            }
            if (amount > longestAmount) {
                longestAmount = amount;
            }
            if(description > longestDescription) {
                longestDescription = description;
            }
        }
    }
}

std::string add_space_after(std::string &par_spaceString, int par_numSpaces)
{
    std::string space = " ";
    for (int i = 0; i < par_numSpaces; i++) {
        par_spaceString+= space;
    }
    return par_spaceString;
}

int find_number_of_spaces(std::string par_current, int par_longest)
{
    return par_longest - par_current.length();
}

int get_number_of_entries(std::vector<std::string> &par_vec)
{
    int entries = 0;
    for (auto entry : par_vec) {
        entries++;
    }
    return entries;
}

void fill_all_entries_no_spaces()
{
    find_longest();
    allEntriesSpaces.clear();
    int print = 2;
    std::string pushBack;
    amount = 0;
    for (auto entry: allEntries) {
        std::string date = get_date(entry);
        pushBack = add_space_after(date, print);
        std::string type = get_type(entry);
        pushBack+= add_space_after(type, find_number_of_spaces(type, longestType + 1));
        std::string category = get_category(entry);
        pushBack+= add_space_after(category, find_number_of_spaces(category, longestCategory + 2));
        std::string amount = get_amount_line_string(entry);
        pushBack+= add_space_after(amount, find_number_of_spaces(amount, longestAmount + 1));
        std::string description = get_description(entry);
        pushBack+= add_space_after(description, find_number_of_spaces(description, longestDescription));

        allEntriesSpaces.push_back(pushBack);
    }
}

/* Change this to all be dynamic for code readability (Change the first 2 printings of the bars) */
void draw_header(std::vector<std::string> &par_vec)
{
    int print2 = 0;
    int print = 12;
    fill_all_entries_no_spaces();
    /* Keybinds */
    mvprintw(0, 1, "q = Quit; n = New Entry; x = Remove Selected Entry; f = Filter Results; b = Set Budget; / = Search");
    /* Header */
    mvprintw(2, 1, "Date");
    mvprintw(3, 1, "----------");
    mvprintw(2, print + 1, "Type");
    print = print + longestType + 3;
    mvprintw(3, 13, "------");
    mvprintw(2, print, "Category");

    print2 = print;
    for (int i = 0; i < longestCategory - 1; i++) {
        /* Nested for loop and if statement needed for drawing enough -'s to be the length of the word above the cell */
        if (longestCategory < 8) {
            for (int j = 0; j < 8; j++) {
                mvprintw(3, print2 + i, "-");
            }
        } else {
            mvprintw(3, print2 + i, "-");
        }
    }
    print = print + longestCategory + 1;

    mvprintw(2, print, "Amount");
    for (int i = 0; i < longestAmount; i++) {
        mvprintw(3, print + i, "-");
    }
    print = print + longestAmount + 2;

    print2 = print;
    mvprintw(2, print, "Description");
    for (int i = 0; i < longestDescription - 1; i++) {
        mvprintw(3, print2 + i, "-");
    }
    /* Draws footer */
    int total = longestDate + longestType + longestAmount + longestCategory + longestDescription + 5;
    for (int i = 0; i < total; i++) {
        mvprintw(par_vec.size() + 4, i + 1, "-");
    }
}
