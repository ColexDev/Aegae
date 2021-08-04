#include <iostream>
#include <ncurses.h>
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>

#include "get.h"
#include "file.h"
#include "table.h"
#include "def.h"

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

/* Variable declarations */
int keyPress;
int month;
int highlight = 0;
int numEntries = 0;
float amount = 0;

/* remove this from global variables */
std::string token;

char token_[80];
Entry entry;
const std::vector<std::string> CHOICES_TYPE {"Income", "Expense", "Loan", "Debt"};
const std::vector<std::string> CHOICES_VIEW_ENTRY {"All", "Specific"};
std::vector<std::string> allEntries;
std::vector<std::string> foundEntries;
std::vector<std::string> budget;
std::vector<std::string> allEntriesSpaces;
std::vector<std::string> specificMonthEntries;
/* Edit the two below to add/delete category options */
const std::vector<std::string> CHOICES_CATEGORY_EXPENSE {"Food", "Transportation", "Entertainment", "Misc"};
const std::vector<std::string> CHOICES_CATEGORY_INCOME {"Salary", "Sale", "Gift", "Misc"};
const std::vector<std::string> months {"January", "Feburary", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

/* Function forward declarations*/
int main();
void find_entry();
void addEntry();

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

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

/* Initilizes a menu either horizonatally or vertically. This NEEDS to be run in a while loop */
void menuInitilization(const std::vector<std::string> &par_ARRCHOICE, int par_direction, int par_xStart, int par_yStart)
{
    int size = par_ARRCHOICE.size();
    /* The array is printed, if i == highlight that item will be highlighted */
    for(int i = 0; i < size; i++) {
        if(i == highlight) {wattron(stdscr, A_REVERSE);}

        if (par_direction == 1) {
            mvwprintw(stdscr, i + par_xStart, par_yStart, par_ARRCHOICE[i].c_str());
        } else {
            mvwprintw(stdscr, 1, i, par_ARRCHOICE[i].c_str());
        }

        wattroff(stdscr, A_REVERSE);
    }

    /* This gets which key is pressed, when K is pressed highlight is decreased by 1, then i == highlight at a different value, highlighting that value of the array. The if statement checks for bounds */
    keyPress = wgetch(stdscr);
    switch(keyPress) {
        case KEY_K:
            highlight--;
            if (highlight == -1)
                highlight = 0;
                break;
        case KEY_J:
            highlight++;
            if (highlight == size)
                highlight = size - 1;
                break;
    }
}

/* Removes a selected entry from the database file */
void removeEntry()
{
    std::ifstream database("database.txt");
    std::string line;
    std::string eraseLine;

    /* Loops through the database until it finds the highlighed entry */
    while(std::getline(database, line)) {
        if(line.find(allEntries[highlight]) != std::string::npos) {
            eraseLine = line;
            /* Asks for deletion confirmation */
            attron(A_BOLD);
            mvwprintw(stdscr, allEntries.size() + 5, 0, "Are you sure you want to DELETE this entry? [Y/n]");
            attroff(A_BOLD);
            keyPress = getch();
            if (keyPress == KEY_Y) {
                erase_file_line("database.txt", eraseLine);
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
    std::ofstream outputFile("tempFile");
    std::ifstream inputFile("database.txt", std::fstream::app);
    constexpr char MESG[]="Enter the Amount: $";
    char str[80];
    echo();
    mvprintw(0, 0, "%s", MESG);
    getstr(str);
    std::string str2 = str;
    add_trailing_zeros(str2);
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
    entry.setDate(get_current_date());
    // Writes to file
    outputFile << entry.getType() << ", " << entry.getCategory() << ", " << "$"
    << entry.getAmount() << ", " << entry.getDescription() << ", " << entry.getDate() << ",\n";
    outputFile << inputFile.rdbuf();
    inputFile.close();
    outputFile.close();
    remove("database.txt");
    rename("tempFile","database.txt");
    clear_refresh();
    highlight = 0;
    main();
}

/* Make both setCategory functions into 1 */
void setCategoryIncome()
{
    highlight = 0;
    entry.setType("Income");
    while(true) {
        clear_refresh();
        mvwprintw(stdscr, 0, 1, "What was it from ");
        menuInitilization(CHOICES_CATEGORY_INCOME, 1, 2, 1);
        switch(keyPress) {
            case KEY_L:
                entry.setCategory(CHOICES_CATEGORY_INCOME[highlight]);
                clear_refresh();
                getSetAmount();
            case KEY_H:
                clear_refresh();
                addEntry();
            case KEY_Q:
                endwin();
                exit(1);
        }
    }
}

void setCategoryExpense()
{
    highlight = 0;
    entry.setType("Expense");
    while(true) {
        clear_refresh();
        mvwprintw(stdscr, 0, 1, "What was it spent on: ");
        menuInitilization(CHOICES_CATEGORY_EXPENSE, 1, 2, 1);
        switch(keyPress) {
            case KEY_L:
                entry.setCategory(CHOICES_CATEGORY_EXPENSE[highlight]);
                clear_refresh();
                getSetAmount();
                break;
            case KEY_H:
                clear_refresh();
                addEntry();
            case KEY_Q:
                endwin();
                exit(1);
        }
    }
}

void addEntry()
{
    highlight = 0;
    while(true) {
        mvwprintw(stdscr, 0, 1, "Please Select the Category: ");
        menuInitilization(CHOICES_TYPE, 1, 2, 1);
        switch(keyPress) {
            case KEY_L:
                clear_refresh();
                if(CHOICES_TYPE[highlight] == "Expense") {
                    setCategoryExpense();
                } else if (CHOICES_TYPE[highlight] == "Income") {
                    setCategoryIncome();
                }
            case KEY_H:
                main();
                clear_refresh();
                break;
            case KEY_Q:
                endwin();
                exit(1);
        }
    }
}

/* make all of these into 1 */
void calculate_money_left_over_month(std::vector<std::string> &par_vec, int par_month)
{
    amount = 0;
    for (auto entry : par_vec) {
        if (get_month(entry) == par_month) {
            if (get_type(entry) == "Expense") {
                amount-= get_amount_line_float(entry);
            } else {
                amount+= get_amount_line_float(entry);
            }
        }
    }
}

void calculate_money_left_over(std::vector<std::string> &par_vec)
{
    amount = 0;
    for (auto entry : par_vec) {
        if (get_type(entry) == "Expense") {
            amount-= get_amount_line_float(entry);
        } else {
            amount+= get_amount_line_float(entry);
        }
    }
}

void calculate_money_left_over_specific(std::vector<std::string> &par_vec)
{
    amount = 0;
    for (auto entry : par_vec) {
        if (entry.find(token_) != std::string::npos) {
            if (get_type(entry) == "Expense") {
                amount-= get_amount_line_float(entry);
            } else {
                amount+= get_amount_line_float(entry);
            }
        }
    }
}

void init_budget()
{
    clear_refresh();
    remove("budget.txt");
    std::fstream budgetFile("budget.txt", std::fstream::app);
    int i = 0;
    echo();
    for (auto category : CHOICES_CATEGORY_EXPENSE) {
        std::string print = "What is your budget for " + category + ": $";
        mvprintw(i, 0, "%s", print.c_str());
        char str[80];
        getstr(str);
        std::string str2 = str;
        budgetFile << str2 << '\n';
        i++;
    }
    noecho();
}


void draw_budget(std::vector<std::string> &par_vec)
{
    std::vector<float> percentVec;
    std::fstream budgetFile("budget.txt");
    std::string line;
    int i = 0;
    float percent = 0;
    float amount = 0;
    int longest = 0;
    int temp = 0;
    while (std::getline(budgetFile, line)) {
        std::string category = CHOICES_CATEGORY_EXPENSE[i];
        if (par_vec.size() == allEntriesSpaces.size()) {
            amount = get_amount_category(category, 1);
        } else {
            amount = get_amount_category(category);
        }
        percent = (amount / stof(line)) * 100;
        attron(A_BOLD);
        std::string print_ = "Your budget for " + months[get_current_month() - 1] + " is:";
        mvprintw(allEntries.size() + 6, 1, print_.c_str());
        attroff(A_BOLD);
        add_trailing_zeros(line);
        std::string print = category + ": " + to_string_with_precision(amount, 2) + " / " + line;
        /* unhardcode i + 12 */
        mvprintw(i + allEntries.size() + 8 , 1, print.c_str());
        temp = print.length();
        if (temp > longest) {
            longest = temp;
        }
        percentVec.push_back(percent);
        i++;
    }
    for (int j = 0; j < i; j++) {
        print_progress_bar(percentVec[j], j, par_vec, longest);
    }
}

/* Try replacing specificMonthEntriesw with allEntriesSpaces and just rewriting to it everytime */
/* Sets up and draws the main table based on which vector is passed */
void setup_menu(std::vector<std::string> &par_vec)
{
    clear_refresh();
    draw_header(par_vec);
    if (par_vec.size() == allEntriesSpaces.size()) {
        calculate_money_left_over(allEntries);
    } else if (par_vec.size() == foundEntries.size()) {
        calculate_money_left_over_specific(allEntries);
    } else {
        draw_budget(par_vec);
        calculate_money_left_over_month(allEntries, month);
    }

    /* Just fucking get rid of numEntries from the entire program */
    numEntries = par_vec.size();
    highlight = 0;
    std::ostringstream ss;
    ss << amount;
    std::string amountString(ss.str());
    add_trailing_zeros(amountString);
    std::string print = "Balance: $" + amountString;
    mvprintw(numEntries + 5, 1, print.c_str());
    while(true) {
        curs_set(0);
        menuInitilization(par_vec, 1, 4, 0);
        switch (keyPress) {
        case KEY_N:
            clear_refresh();
            addEntry();
        case KEY_X:
            removeEntry();
            setup_menu(specificMonthEntries);
        case KEY_BACKSLASH:
            find_entry();
            break;
        case KEY_A:
            setup_menu(allEntriesSpaces);
        case KEY_B:
            init_budget();
        case KEY_M:
            setup_menu(specificMonthEntries);
        case KEY_Q:
            endwin();
            exit(1);
        }
    }
}

/* REMOVE NUMENTRIES FROM THIS */
void current_month_entries()
{
    fill_all_entries_no_spaces();
    clear_refresh();
    month = get_current_month();
    specificMonthEntries.clear();
    for (auto entry : allEntriesSpaces) {
        if (get_month(entry) == month) {
            specificMonthEntries.push_back(entry);
            numEntries++;
        }
    }
    setup_menu(specificMonthEntries);
}

/* REMOVE NUMENTRIES FROM THIS */
void find_entry()
{
    foundEntries.clear();
    int row, col;
    getmaxyx(stdscr, row, col);
    mvprintw(row - 1, 1, "/");
    echo();
    move(row - 1, 2);
    getstr(token_);
    noecho();
    for (auto entry : allEntriesSpaces) {
        if (entry.find(token_) != std::string::npos) {
            numEntries++;
            foundEntries.push_back(entry);
        }
    }
    if (numEntries == 0) {
        attron(A_BOLD);
        mvprintw(row - 2, 1, "No results were found");
        attroff(A_BOLD);
    } else {
        clear_refresh();
    }
    /* Why is the call to draw_header here???? */
    draw_header(foundEntries);
    highlight = 0;
    setup_menu(foundEntries);
}

/* Add budget for each category, show if going over the budget in reports section. Display a notification to check reports on main page if going over budget. Show budget/spending for each category in reports */

/* Try changing calculate_money_left_over to .find("Debit") instead of calling get_type so that it can be done on any vector (this needs to be done so that the calculate_money_left_over can be called on the vectors with spaces, this will decrease the amount of if statements needed to filter down allEntries to just what is inside of specificMonthEntries or foundEntries. This will also clean up the code a lot and remove bloat. Just remove all of the get functions all together, or change them to do .find() instead */

/* Change the default view to be of the current month, have a button to click to show ALL entries. The user wants to see the current month more than they do all entries */
int main()
{
    write_database_to_vector();
    clear_refresh();
    /* Sets up ncurses */
    initscr();
    setlocale(LC_ALL, "");
    cbreak();
    noecho();
    keypad(stdscr, true);
    current_month_entries();
}
