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

