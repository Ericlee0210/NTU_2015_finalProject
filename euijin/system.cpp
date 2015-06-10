#include <iostream>
#include <sstream>
#include "data.h"
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
  AccountT T;
  History Hist;
  while(std::getline(std::cin, row))
  {
    std::istringstream input(row);
    input >> op;

    if (op == "create") // find 하는데 드는 시간은 log n
    {
      input >> ID1 >> PW1;
      if (T.find(ID1) == T.end()) // 아무것도 없을 때도 성립?
      {
        Info temp;
        temp.password = md5(PW1);
        temp.balance = 0;
        T[ID1] = temp;
        std::cout << "success\n";
      }
      else
        std::cout << "ID " << ID1 << " exists, \n";
    }

    else if (op == "login") // log n
    {
      input >> ID1 >> PW1;
      AccountT::iterator id1_pos = T.find(ID1);
      if (id1_pos == T.end())
        std::cout << "ID " << ID1 << " not found\n";
      else if (md5(id1_pos->second.password) != PW1)
        std::cout << "wrong password\n";
      else
      {
        std::cout << "success\n";
        logedinID = ID1;
      }
    }

    else if (op == "delete") // log n
    {
      input >> ID1 >> PW1;
      AccountT::iterator id1_pos = T.find(ID1);
      if (id1_pos == T.end())
        std::cout << "ID " << ID1 << " not found\n";
      else if (md5(id1_pos->second.password) != PW1)
        std::cout << "wrong password\n";
      else
      {
        std::cout << "success\n";
        T.erase(ID1);
      }
    }

    else if (op == "merge") // n1 n2 -> log n1 log n2 h1 h2
    {
      input >> ID1 >> PW1 >> ID2 >> PW2;
      AccountT::iterator id1_pos = T.find(ID1);
      AccountT::iterator id2_pos = T.find(ID2);
      if (id1_pos == T.end())
        std::cout << "ID " << ID1 << " not found\n";
      else if (id2_pos == T.end())
        std::cout << "ID " << ID2 << " not found\n";
      else if (md5(id1_pos->second.password) != PW1)
        std::cout << "wrong password1\n";
      else if (md5(id2_pos->second.password) != PW2)
        std::cout << "wrong password2\n";
      else
      {
        std::vector<int>::iterator it1 = id1_pos->second.log_idx.begin();
        std::vector<int>::iterator it2 = id2_pos->second.log_idx.begin();
        std::vector<int> temp;

        while (it1 != id1_pos->second.log_idx.end() && it2 != id2_pos->second.log_idx.end())
        {
          if (*it1 < *it2)
          {
            temp.push_back(*it1);
            it1++;
          }
          else if (*it1 == *it2)
          {
            temp.push_back(*it1);
            if (Hist[*it1].from == ID2)
              Hist[*it1].from = ID1;
            else Hist[*it1].to = ID1;
            it1++; it2++;
          }
          else
          {
            temp.push_back(*it2);
            if (Hist[*it2].from == ID2)
              Hist[*it2].from = ID1;
            else Hist[*it2].to = ID1;
            it2++;
          }
        }
        if (it1 != id1_pos->second.log_idx.end() && it2 == id1_pos->second.log_idx.end())
          for (;it1 != id1_pos->second.log_idx.end(); ++it1)
            temp.push_back(*it1);
        else if (it1 == id1_pos->second.log_idx.end() && it2 != id1_pos->second.log_idx.end())
        {
          for (;it2 != id2_pos->second.log_idx.end(); ++it2)
          {
            temp.push_back(*it2);
            if (Hist[*it2].from == ID2)
              Hist[*it2].from = ID1;
            else Hist[*it2].to = ID1;
          }
        }

        id1_pos->second.log_idx = temp;

        id1_pos->second.balance += id2_pos->second.balance;
        T.erase(ID2);
      }
    }

    else if (op == "deposit")
    {
      input >> num;
      money = std::stoi(num);
      T[logedinID].balance += money;
      std::cout << "success, " << T[logedinID].balance << " dollars in current account\n";
    }

    else if (op == "withdraw")
    {
      input >> num;
      money = std::stoi(num);
      if (T[logedinID].balance < money)
        std::cout << "fail, " << T[logedinID].balance << " dollars only in current account\n";
      else
      {
        T[logedinID].balance -= money;
        std::cout << "success, " << T[logedinID].balance << " dollars left in current account\n";
      }
    }

    else if (op == "transfer")
    {
      input >> ID1 >> num;
      money = std::stoi(num);
      if (T.find(ID1) == T.end())
        std::cout << "ID " << ID1 << " not found\n";
      else if (T[logedinID].balance < money)
        std::cout << "fail, " << T[logedinID].balance << " dollars only in current account\n";
      else
      {
        T[ID1].log_idx.push_back(Hist.size());
        T[logedinID].log_idx.push_back(Hist.size());

        Log temp = Log(logedinID, ID1, money);
        Hist.push_back(temp);

        T[ID1].balance += money;
        T[logedinID].balance -= money;

        std::cout << "success, " << T[logedinID].balance << " dollars left in current account\n";
      }
    }

    else if (op == "find")
    {
      input >> ID1;

      int comma = 0;
      for (AccountT::iterator it = T.begin(); it != T.end(); ++it)
      {
        if (smatch(it->first, ID1) && it->first != logedinID)
        {
          if (comma == 0)
          {
            std::cout << it->first;
            comma = 1;
          }
          else
            std::cout << ", " << it->first;
        }
      }
      std::cout << std::endl;
    }

    else if (op == "search")
    {
      input >> ID1;

      if (T.find(ID1) == T.end())
        std::cout << "ID " << ID1 << " not found\n";
      else
      {
        std::vector<int> lst;
        std::set_intersection(T[ID1].log_idx.begin(), T[ID1].log_idx.end(),
            T[logedinID].log_idx.begin(), T[logedinID].log_idx.end(),
            std::back_inserter(lst));
        if (lst.empty())
          std::cout << "no record\n";
        else
        {
          for (std::vector<int>::iterator it = lst.begin(); it != lst.end(); ++it)
            print_log(Hist[*it], logedinID);
        }
      }
    }


  }

  return 0;
}

