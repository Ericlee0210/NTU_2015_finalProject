#include <vector>
#include <string>
#include <sstream>
#include "data_un.h"
#include "md5.h"

int main()
{
  std::string row;
  std::string op;
  std::string ID1, ID2;
  std::string PW1, PW2;
  std::string num;
  int money;
  std::string logedinID = "";
  account_table T;

  while(std::getline(std::cin, row))
  {
    std::istringstream input(row);
    input >> op;

    if (op == "create")
    {
      input >> ID1 >> PW1;
      std::string password_tmp = md5(PW1);
      T.create(ID1, password_tmp);
    }
    else if (op == "login") // log n
    {
      input >> ID1 >> PW1;
      std::string password_tmp = md5(PW1);
      T.login(ID1, password_tmp);
    }
    else if (op == "delete") // log n
    {
      input >> ID1 >> PW1;
      std::string password_tmp = md5(PW1);
      T.del(ID1, password_tmp);
    }
    else if (op == "merge") // n1 n2 -> log n1 log n2 h1 h2
    {
      input >> ID1 >> PW1 >> ID2 >> PW2;
      std::string password_tmp1 = md5(PW1);
      std::string password_tmp2 = md5(PW2);
      T.merge(ID1, password_tmp1, ID2, password_tmp2);
    }
    else if (op == "deposit")
    {
      input >> num;
      money = std::stoi(num);
      T.deposit(money);
    }
    else if (op == "withdraw")
    {
      input >> num;
      money = std::stoi(num);
      T.withdraw(money);
    }
    else if (op == "transfer")
    {
      input >> ID1 >> num;
      money = std::stoi(num);
      T.transfer(ID1, money);
    }
    else if (op == "find")
    {
      input >> ID1;
      T.find(ID1);
    }
    else if (op == "search")
    {
      input >> ID1;
      T.search(ID1);
    }
  }
  return 0;
}