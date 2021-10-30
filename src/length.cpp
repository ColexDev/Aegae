#include <iostream>
#include <vector>
#include "get.h"
#include "file.h"

extern std::vector<std::string> allEntries;

int get_greatest_date_length()
{
    int dateLength;
    int largest = 0;
    for (auto element : allEntries) {
       dateLength = get_date(element).length();
       if (dateLength > largest) {
           largest = dateLength;
       }
    }
    return dateLength;
}

int get_greatest_type_length()
{
    int typeLength;
    int largest = 0;
    for (auto element : allEntries) {
       typeLength = get_type(element).length();
       if (typeLength > largest) {
           largest = typeLength;
       }
    }
    return typeLength;
}
