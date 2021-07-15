#include <iostream>
#include <fstream>
#include <vector>
#include "get.h"


extern std::vector<std::string> allEntries;

static int longestDate = 0;
static int longestType = 0;
static int longestCategory = 0;
static int longestAmount = 0;

// Copied from stack overflow, its function is self-explanatory.
void eraseFileLine(const std::string par_PATH, const std::string par_ERASELINE)
{
    std::string line;
    std::fstream database;
    database.open(par_PATH);
    // contents of path must be copied to a temp file then
    // renamed back to the path file
    std::ofstream temp;
    temp.open("temp.txt");

    while(std::getline(database, line)) {
        // write all lines to temp other than the line marked for erasing
        if (line != par_ERASELINE) {temp << line << std::endl;}
    }

    temp.close();
    database .close();

    // required conversion for remove and rename functions
    const char * path = par_PATH.c_str();
    remove(path);
    rename("temp.txt", path);
}

void write_database_to_vector()
{
    allEntries.clear();
    std::string line;
    std::fstream database("database.txt");
    while(std::getline(database, line)) {
        allEntries.push_back(line);
    }
}

