#include <iostream>
#include <ncurses.h>
#include <cstring>
#include <fstream>
#include <vector>

#include "get.h"
#include "file.h"
#include "table.h"


int keyPress;
int highlight;
std::string token;

const std::vector<std::string> CHOICESTYPE {"Expense", "Income"};
const std::vector<std::string> CHOICESVIEWENTRY {"All", "Specific"};
std::vector<std::string> allEntries;
std::vector<std::string> foundEntries;
std::vector<std::string> allEntriesSpaces;
std::vector<std::string> specificMonthEntries;
/* Edit the two below to add/delete category options */
const std::vector<std::string> CHOICESCATEGORYEXPENSE {"Food", "Transportation", "Entertainment", "Other"};
const std::vector<std::string> CHOICESCATEGORYINCOME {"Salary", "Sale", "Gift", "Other"};

// Entry class for storing temp values
class Entry
{
private:
    std::string mem_type;
    std::string mem_category;
    std::string mem_amount;
    std::string mem_date;
    std::string mem_description;
// Set the value (not currently used)
public:
    void setType(std::string par_value) {mem_type = par_value;}
    void setCategory(std::string par_value) {mem_category = par_value;}
    void setAmount(std::string par_value) {mem_amount = par_value;}
    void setDate(std::string par_value) {mem_date = par_value;}
    void setDescription(std::string par_value) {mem_description = par_value;}

    std::string getType() {return mem_type;}
    std::string getCategory() {return mem_category;}
    std::string getAmount() {return mem_amount;}
    std::string getDate() {return mem_date;}
    std::string getDescription() {return mem_description;}
};

const std::vector<std::string> months {"January", "Feburary", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

int longestDate;
int longestType;
int longestCategory;
int longestAmount;
int longestDescription;

int main();
void menuInitilization(const std::vector<std::string> &arrChoice, int direction, int xStarxStart, int yStart);
void find_entry();
void addEntry();
Entry entry;

void clearRefresh(){ wclear(stdscr); wrefresh(stdscr); }

void menuInitilization(const std::vector<std::string> &par_ARRCHOICE, int direction, int xStart, int yStart)
{
    int size = par_ARRCHOICE.size();
    for(int i = 0; i < size; i++) {
        if(i == highlight) {wattron(stdscr, A_REVERSE);}
        if (direction == 1) {
            mvwprintw(stdscr, i+xStart, yStart, par_ARRCHOICE[i].c_str());
        } else {
            mvwprintw(stdscr, 1, i, par_ARRCHOICE[i].c_str());
        }
        wattroff(stdscr, A_REVERSE);
    }
    keyPress = wgetch(stdscr);
    switch(keyPress) {
        case 107:
            highlight--;
            if (highlight == -1)
                highlight = 0;
                break;
        case 106:
            highlight++;
            if (highlight == size)
                highlight = size - 1;
                break;
    }
}
/*
void viewEntry()
{
    highlight = 0;
    clearRefresh();
    std::string line;
    std::ifstream database("database.txt");
    int lineNumber = 0;
    while(true) {
        mvwprintw(stdscr, 0, 1, "Which entries would you like to see: ");
        menuInitilization(CHOICESVIEWENTRY, 1, 2, 1);
        if(keyPress == 108) {
            switch (highlight) {
                case 0:
                    clearRefresh();
                    endwin();
                    draw_table();
                    break;
                case 1:
                    clearRefresh();
                    findEntry();
            }
            break;
        }
    }
    highlight = 0;
    main();
}
*/

void removeEntry()
{
    std::ifstream database("database.txt");
    std::string line;
    std::string eraseLine;
    while(std::getline(database, line)) {
        if(line.find(allEntries[highlight]) != std::string::npos) {
            eraseLine = line;
            attron(A_BOLD);
            mvwprintw(stdscr, allEntries.size() + 5, 0, "Are you sure you want to DELETE this entry? [Y/n]");
            attroff(A_BOLD);
            keyPress = getch();
            if (keyPress == 121) {
                eraseFileLine("database.txt", eraseLine);
                main();
                break;
            } else {
                continue;
            }
        }
    }
}

// This does too much based off its name
void getSetAmount()
{
    std::ofstream outputFile("outputFileName");
    std::ifstream inputFile("database.txt");
    constexpr char MESG[]="Enter the Amount: ";
    char str[80];
    echo();
    mvprintw(0, 0, "%s", MESG);
    getstr(str);
    std::string str2 = str;
    if (str2.find(".") != std::string::npos) {
    } else {
        str2 = str2 + ".00";
    }
    entry.setAmount(str2);
    noecho();
    char msg[]="Enter a Description (Press enter for none): ";
    char desc[80];
    echo();
    mvprintw(0, 0, "%s", msg);
    getstr(desc);
    noecho();
    entry.setDescription(desc);
    if (strlen(desc) == 0) {
        entry.setDescription("None");
    }
    // Sets the date and time
    // entry.setDate(getCurrentDateTime() + '\n');
    entry.setDate(getCurrentDateTime());
    // Writes to file
    outputFile << entry.getType() << ", " << entry.getCategory() << ", " << "$"
    << entry.getAmount() << ", " << entry.getDescription() << ", " << entry.getDate() << ",\n";
    outputFile << inputFile.rdbuf();
    inputFile.close();
    outputFile.close();
    remove("database.txt");
    rename("outputFileName","database.txt");
    clearRefresh();
    highlight = 0;
    main();
}

void setCategoryIncome()
{
    highlight = 0;
    entry.setType("Income");
    while(true) {
        clearRefresh();
        mvwprintw(stdscr, 0, 1, "What was it from ");
        menuInitilization(CHOICESCATEGORYINCOME, 1, 2, 1);
        switch(keyPress) {
            case 113:
                endwin();
                exit(1);
            case 108:
                entry.setCategory(CHOICESCATEGORYINCOME[highlight]);
                clearRefresh();
                getSetAmount();
            case 104:
                clearRefresh();
                addEntry();
        }
    }
}

void setCategoryExpense()
{
    highlight = 0;
    entry.setType("Expense");
    while(true) {
        clearRefresh();
        mvwprintw(stdscr, 0, 1, "What was it spent on: ");
        menuInitilization(CHOICESCATEGORYEXPENSE, 1, 2, 1);
        switch(keyPress) {
            case 113:
                endwin();
                exit(1);
            case 108:
                entry.setCategory(CHOICESCATEGORYEXPENSE[highlight]);
                clearRefresh();
                getSetAmount();
                break;
            case 104:
                clearRefresh();
                addEntry();
        }
    }
}

void addEntry()
{
    highlight = 0;
    while(true) {
        mvwprintw(stdscr, 0, 1, "Please Select the Category: ");
        menuInitilization(CHOICESTYPE, 1, 2, 1);
        switch(keyPress) {
            case 108:
                clearRefresh();
                if(CHOICESTYPE[highlight] == "Expense") {
                    setCategoryExpense();
                } else if (CHOICESTYPE[highlight] == "Income") {
                    setCategoryIncome();
                }
            case 104:
                main();
                clearRefresh();
                break;
            case 113:
                endwin();
                exit(1);
        }
    }

}

void filter_results()
{

}

void setup_menu(std::vector<std::string> &vec)
{
    clearRefresh();
    draw_table();
    while(true) {
        curs_set(0);
        menuInitilization(vec, 1, 4, 0);
        switch (keyPress) {
        case 110:
            clearRefresh();
            addEntry();
        case 120:
            removeEntry();
            setup_menu(allEntriesSpaces);
        case 47:
            find_entry();
            break;
        case 102:
            filter_results();
        case 27:
            setup_menu(allEntriesSpaces);
        case 113:
            endwin();
            exit(1);
        }
    }
}

void find_entry()
{
    foundEntries.clear();
    int row, col;
    int numEntries = 0;
    char token[80];
    getmaxyx(stdscr, row, col);
    mvprintw(row - 1, 1, "/");
    echo();
    move(row - 1, 2);
    getstr(token);
    noecho();
    for (auto entry : allEntriesSpaces) {
        if (entry.find(token) != std::string::npos) {
            numEntries++;
            foundEntries.push_back(entry);
        }
    }
    if (numEntries == 0) {
        attron(A_BOLD);
        mvprintw(row - 2, 1, "No results were found");
        attroff(A_BOLD);
    } else {
        clearRefresh();
    }
    draw_table();
    highlight = 0;
    setup_menu(foundEntries);
}


int main()
{
    write_database_to_vector();
    fill_all_entries_no_spaces();
    static bool firstRun = false;
    clearRefresh();
    highlight = 0;
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    /*
    if(firstRun == false) {
        while(true) {
            wattron(stdscr, A_BOLD);
            mvwprintw(stdscr, 0, 1, "Corcyra");
            mvwprintw(stdscr, 1, 1, "By: ColexDev");
            wattroff(stdscr, A_BOLD);
            mvwprintw(stdscr, 3, 1, "Press any key to continue...");
            getch();
            clearRefresh();
            break;
        }
            firstRun = true;
    }
    */
    setup_menu(allEntriesSpaces);
}
