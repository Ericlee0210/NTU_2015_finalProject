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

    if (op == "create")
    {
      input >> ID1 >> PW1;
      if (T.find(ID1) == T.end()) // 아무것도 없을 때도 성립?
      {
        Info temp;
        temp.password = PW1;
        temp.balance = 0;
        T[ID1] = temp;
      }
      else
        std::cout << "ID " << ID1 << " exists, ";
    }

  }

  return 0;
}

