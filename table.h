#pragma once
#include <iostream>
#include <vector>

void find_longest(int par_month = 0);
std::string add_space_after(std::string &par_spaceString, int par_numSpaces);
int find_number_of_spaces(std::string par_current, int par_longest);
void fill_all_entries_no_spaces();
int get_number_of_entries(std::vector<std::string> &par_vec);
void draw_header(std::vector<std::string> &par_vec);
