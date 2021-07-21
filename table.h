#pragma once
#include <iostream>
#include <vector>

void find_longest();
std::string add_space_after(std::string &spaceString, int numSpaces);
int find_number_of_spaces(std::string current, int longest);
void fill_all_entries_no_spaces();
int get_number_of_entries(std::vector<std::string> &vec);
void draw_header(std::vector<std::string> &vec);
