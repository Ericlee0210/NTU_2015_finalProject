#include <iostream>
#include "data.h"

void print_log(Log l, std::string ID)
{
  if (l.from == ID)
    std::cout << "\t To " << l.to << " " << l.amount << std::endl;
  else if (l.to == ID)
    std::cout << "\t From " << l.from << " " << l.amount << std::endl;

}

void print_info(std::string ID, AccountT T, History H)
{
  if (T.find(ID) != T.end())
  {
    AccountT::iterator it = T.find(ID);
    std::cout << "ID : " << ID << std::endl;
    std::cout << "PW : " << it->second.password << " Balance : " << it->second.balance << "\nHistory\n";
    for (std::vector<int>::iterator log_it = it->second.log_idx.begin(); log_it != it->second.log_idx.end(); ++log_it)
      print_log(H[*log_it], ID);
  }
  else std::cout << "ERROR!!\n";
}

bool smatch(std::string str1, std::string str2)
{
  std::string::iterator it1;
  std::string::iterator it2;
  std::string::iterator asterisk;
  int flag = 0;
  it1 = str1.begin();
  it2 = str2.begin();

  while(1)
  {
    if (it1 == str1.end() && it2 == str2.end())
      return true; //?
    if (it2 == str2.end() || it1 == str1.end())
      return false;

    if (*it1 == *it2 || *it2 == '?')
    {
      ++it1; ++it2;
    }
    else if (*it2 == '*')
    {
      flag = 1;
      asterisk = it2;
      ++it2;
      if (it2 == str2.end()) // 별 마지막이면 다 트루
        return true;
    }
    else if (flag == 1) // !!!!!!!!!계속 여기서 돈다 abc a*c 케이스
    {
      it2 = asterisk;
      ++it2;
      if (*it1 != *it2)
        ++it1;
    }
    else return false;
  }
}
