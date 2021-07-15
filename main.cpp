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

const std::vector<std::string> CHOICESMAIN {"Add an Entry", "Remove an Entry", "View an Entry", "Reports"};
const std::vector<std::string> CHOICESTYPE {"Expense", "Income"};
const std::vector<std::string> CHOICESTOTALSPENDING {"Total Spending", "Total Income", "Money Left"};
const std::vector<std::string> CHOICESVIEWENTRY {"All", "Specific"};
std::vector<std::string> allEntries;
std::vector<std::string> allEntriesSpaces;
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
void addEntry();
Entry entry;

void clearRefresh(){ wclear(stdscr); wrefresh(stdscr); }

char *strdup(const char *src_str) noexcept {
    char *new_str = new char[std::strlen(src_str) + 1];
    std::strcpy(new_str, src_str);
    return new_str;
}

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

void findEntry()
{
    clearRefresh();
    std::ifstream database("database.txt");
    std::string line;
    constexpr char MESG[]="Enter the a keyword to search for (date[02-21-2021], type, etc): ";
    char token[80];
    echo();
    mvprintw(0, 0, "%s", MESG);
    getstr(token);
    noecho();
    clearRefresh();
    int numResults = 0;
    while(std::getline(database, line)) {
        if(line.find(token) != std::string::npos) {
            numResults++;
            mvwprintw(stdscr, numResults-1, 1, line.c_str());
        }
    }
    if (numResults == 0) {
        mvwprintw(stdscr, numResults, 1, "No results were found.");
    }
    mvwprintw(stdscr, numResults+2, 1, "Press any key to continue...");
    getch();
    clearRefresh();
}

// Get an entry from the date that the user enters
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
                    draw_table(line, lineNumber);
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


// Deletes a specific entry
void removeEntry()
{
    std::ofstream outputFile("outputFileName");
    std::ifstream database("database.txt");
    std::string line;
    std::string eraseLine;
    constexpr char MESG[]="Search for the entry you want to remove (type, category, amount, date [02-21-2021]): ";
    char token[80];
    echo();
    mvprintw(0, 0, "%s", MESG);
    getstr(token);
    noecho();
    while(std::getline(database, line)) {
        if(line.find(token) != std::string::npos) {
            eraseLine = line;
            clearRefresh();
            mvwprintw(stdscr, 2, 1, line.c_str());
            echo();
            mvwprintw(stdscr, 0, 1, "Is this the line you want to delete[y/n]: ");
            keyPress = getch();
            noecho();
            if (keyPress == 121) {
                eraseFileLine("database.txt", eraseLine);
                highlight = 0;
                main();
                clearRefresh();
                break;
            } else {
                continue;
            }
        }
    }
    clearRefresh();
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
    entry.setAmount(str);
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
                clearRefresh();
                break;
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
                clearRefresh();
                break;
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
        }
    }

}



// MAKE AN EXPENSE SHEET, HAVE IT SHOW ALL MONTHS AND YEARLY TOO
int main()
{
    static bool firstRun{false};
    clearRefresh();
    highlight = 0;
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
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
        while(true) {
            curs_set(0);
            mvwprintw(stdscr, 0, 1, "Please Select an Option: ");
            menuInitilization(CHOICESMAIN, 1, 2, 1);
            switch (keyPress) {
                case 108:
                    clearRefresh();
                    switch (highlight) {
                        case 0:
                            clearRefresh();
                            addEntry();
                        case 1:
                            clearRefresh();
                            removeEntry();
                        case 2:
                            clearRefresh();
                            viewEntry();
                        case 3:
                            clearRefresh();
                            constexpr char MESG[]="What month do you want to see a report for?(1, 2, 3, etc): ";
                            char find[2];
                            echo();
                            mvprintw(0, 0, "%s", MESG);
                            getstr(find);
                            noecho();
                            int x {std::stoi(find)};
//                            totalSpending(x);
                    }
               case 113:
                    endwin();
                    exit(1);

            }
        }
}
