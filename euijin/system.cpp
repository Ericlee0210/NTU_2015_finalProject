#include <iostream>
#include <sstream>
#include "data.h"

int main()
{
  std::string row;
  std::string op;
  std::string ID1, ID2;
  std::string PW1, PW2;
  int a;
  std::string logedinID = "";
  AccountT T;
  History Hist;
  while(std::getline(std::cin, row))
  {
    std::istringstream input(row);
    input >> op;

  }

  return 0;
}

