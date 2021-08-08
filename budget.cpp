#include <iostream>
#include <vector>
#include <ncurses.h>
#include <fstream>

#include "utils.h"
#include "get.h"
#include "table.h"

extern const std::vector<std::string> CHOICES_CATEGORY_EXPENSE {"Food", "Transportation", "Entertainment", "Misc"};
extern const std::vector<std::string> months {"January", "Feburary", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

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
        // if (par_vec.size() == allEntriesSpaces.size()) {
            // amount = get_amount_category(category, 1);
        // } else {
            // amount = get_amount_category(category);
        // }
        amount = get_amount_category(category);
        percent = (amount / stof(line)) * 100;
        attron(A_BOLD);
        std::string print_ = "Your budget for " + months[get_current_month() - 1] + " is:";
        mvprintw(par_vec.size() + 9, 1, print_.c_str());
        attroff(A_BOLD);
        add_trailing_zeros(line);
        std::string print = category + ": " + to_string_with_precision(amount) + " / " + line;
        /* unhardcode i + 12 */
        mvprintw(i + par_vec.size() + 11 , 1, print.c_str());
        temp = print.length();
        if (temp > longest) {
            longest = temp;
        }
        percentVec.push_back(percent);
        i++;
    }
    for (int j = 0; j < i; j++) {
        print_progress_bar(percentVec[j], j + 2, par_vec, longest);
    }
}

