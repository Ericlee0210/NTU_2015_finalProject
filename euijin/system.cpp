#include <iostream>
#include <sstream>
#include "data.h"
#include "md5.h"
#include "math.h"

// 추천수가 열개 안되고 다읽으면 멈추는 코드 집어 넣기
// 자기 자식 빼는 것도 안넣음
// print 할 때 투먼전지 프롬먼전지도 체크해야함 머지한 것의 경우 이런경우가 생길 수 있는가

int func(int n)
{
  return (-1 + sqrt(1 + 8 * n)) / 2;
}

int score(std::string str1, std::string str2)
{
  int min = std::min(str1.length(), str2.length());
  int max = std::max(str1.length(), str2.length());
  int temp = (max - min) * (max - min + 1) / 2;
  for (int i = 0; i < min; i++)
    if (str1[i] != str2[i])
      temp += min - i;
  return temp;
}

/* Input : score
 * Output : vector of int vector, each int vector is set of position which make sum as score 
 * example)
 * Input : 5
 * Output : (5) -> (4->1) -> (3->2) */
std::vector<std::vector<int> > get_pos_set(int n)
{
  std::vector<std::vector<std::vector<int> > > table; // Save all list of output for each score input
  std::vector<std::vector<int> > temp_lst;
  std::vector<int> temp_set;

  if (n == 0)
    return temp_lst; // output has no content so that we can check with size() funtion

  temp_set.push_back(1);
  temp_lst.push_back(temp_set);
  table.push_back(temp_lst);

  if (n == 1)
    return table[0];

  temp_set.clear();
  temp_lst.clear();
  temp_set.push_back(2);
  temp_lst.push_back(temp_set);
  table.push_back(temp_lst);

  if (n == 2)
    return table[1];

  for (int i = 3; i <= n; i++)
  {
    temp_set.clear();
    temp_lst.clear();
    temp_set.push_back(i);
    temp_lst.push_back(temp_set);
    int cnt = i-1;
    while(cnt > i/2)
    {
      for (std::vector<std::vector<int> >::iterator it = table[i-cnt-1].begin(); it != table[i-cnt-1].end(); ++it)
      {
        temp_set.clear();
        temp_set.push_back(cnt);
        for(std::vector<int>::iterator inner_it = it->begin(); inner_it != it->end(); ++inner_it)
        {
          temp_set.push_back(*inner_it);
        }
        temp_lst.push_back(temp_set);
      }
      cnt--;
    }
    table.push_back(temp_lst);
  }

  return table[n-1];
}

/* Input : length we want to make
 * Output : vector of permuted string
 * example)
 * Input : 3 -> Output : 000 to zzz */
std::vector<std::string> permute(int n) // If we put those function inside main function, is it okay with memory?
{
  std::vector<std::string> result;
  std::string set[] = {"a","b","c","d","e"};
//  std::string set[] = {"0","1","2","3","4","5","6","7","8","9",
//   "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
//    "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

  if (n == 0)
    return result;

  if (n == 1)
  {
    //for (int i=0;i<62;i++)
    for (int i=0;i<5;i++)
      result.push_back(set[i]);
    return result;
  }

  else
  {
    std::vector<std::string> temp = permute(n-1);
    for (std::vector<std::string>::iterator it = temp.begin(); it != temp.end(); ++it)
    {
      //for (int i=0;i<62;i++)
      for (int i=0;i<5;i++)
        result.push_back(*it + set[i]);
    }
    return result;
  }
}

/* Input : string which we want to change, vector of position
 * Output : vector of changed string
 * example)
 * Input : string , 4->2
 * Output : st?i?g (? can be 0 to z) */
std::vector<std::string> change_char_at_pos(std::string str, std::vector<int> N) // 숫자 벡터가 idx를 0부터 카운트한다고 생각하고 함.
{
  std::vector<std::string> result;
  result.push_back(""); // If it is empty I can start so add empty string.
  std::string set[] = {"a","b","c","d","e"};
//  std::string set[] = {"0","1","2","3","4","5","6","7","8","9",
//    "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
//    "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

  int idx = 0;
  std::vector<int>::iterator pos = N.begin();

  while(pos != N.end()) // During iterations, when we confront pos change it from 0 to z.
  {
    if (str.length() - *pos > idx)
    {
      for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it)
        *it += str[idx];
      idx++;
    }

    else
    {
      std::vector<std::string> tempV;
      for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it)
      {
       // for (int i=0;i<62;i++)
        for (int i=0;i<5;i++)
          tempV.push_back(*it + set[i]);
      }
      result = tempV;
      tempV.clear();
      ++pos; idx++;
    }
  }

  if (idx < str.length()) // If while loop ends because we changed all, add left part of string.
  {
    while (idx < str.length())
    {
      for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it)
        *it += str[idx];
      idx++;
    }
  }

  return result;
}

/* Input : string
 * Output : vector of string which we recommend 
 * Algorithm :
 * Iterate by score
 * Divide score as score_by_length and score_by_modification
 * There are two way to recommend, One is Cut first and Modify, The other is Modify first and Add permutation */
std::vector<std::string> recommend_for_create(std::string str, AccountT Table)
{
  int score = 1;
  std::vector<std::string> result;

  while(1)
  {
    std::vector<std::string> temp;

    int s_len = 0;
    int s_mod; // score - s_len
    int p_cnt = 0;

    while (s_len <= score)
    {
      s_mod = score - s_len;

      std::vector<std::vector<int> > pos_set = get_pos_set(s_mod);

      // First step cut first and modify
      if (str.length() > p_cnt)
      {
        std::string temp_str = str.substr(0, str.length() - p_cnt);
        if (pos_set.size() == 0) // Add string only cut
          temp.push_back(temp_str);
        else
        {
          for (std::vector<std::vector<int> >::iterator it = pos_set.begin(); it != pos_set.end(); ++it)
          {
            if (it->front() <= temp_str.length())
            {
              std::vector<std::string> temp_c = change_char_at_pos(temp_str, *it);
              std::copy (temp_c.begin(),temp_c.end(),back_inserter(temp));
            }
          }
        }
      }

      // Second step modify first and add permutation
      std::vector<std::string> pmt = permute(p_cnt);

      if (pmt.size() != 0)
      {
        if (str.length() + p_cnt <= 100) // Length of ID should be less than 100
        {
          if (pos_set.size() == 0) // s_mod is 0
          {
            for (std::vector<std::string>::iterator it = pmt.begin(); it != pmt.end(); ++it)
              temp.push_back(str + *it);
          }
          else
          {
            for (std::vector<std::vector<int> >::iterator it = pos_set.begin(); it != pos_set.end(); ++it)
            {
              if (it->front() <= str.length())
              {
                std::vector<std::string> temp_c = change_char_at_pos(str, *it);

                for (std::vector<std::string>::iterator i_it = temp_c.begin(); i_it != temp_c.end(); ++i_it)
                  for (std::vector<std::string>::iterator ii_it = pmt.begin(); ii_it != pmt.end(); ++ii_it)
                    temp.push_back(*i_it + *ii_it);
              }
            }
          }
        }
      }

      p_cnt++;
      s_len += p_cnt;
    }

    std::sort(temp.begin(),temp.end());

    std::vector<std::string>::iterator it = temp.begin();
    while(result.size() < 10 && it != temp.end())
    {
      if (*it != str)
      {
        if (result.size() != 0 && *it != result.back())
        {
          if (Table.find(*it) == Table.end())
            result.push_back(*it);
        }
        else if (result.size() == 0)
        {
          if (Table.find(*it) == Table.end())
            result.push_back(*it);
        }
      }
      ++it;
    }

    if (result.size() == 10)
      break;

    score++;

    temp.clear();
  }

  return result;
}

std::vector<std::pair<int, std::string> > put_into_vector(int n, std::string str, std::vector<std::pair<int, std::string> > V)
{
  if (V.size() >= 10)
    if (V.back().first < n || (V.back().first == n && str.compare(V.back().second) >= 0))
      return V;

  if (V.size() == 0)
    V.push_back(std::make_pair(n, str));
  else
  {
    std::vector<std::pair<int, std::string> >::iterator it = V.begin();
    while (1)
    {
      if (it == V.end())
      {
        if (V.size() < 10) { V.push_back(std::make_pair(n, str)); break; }
      }
      else if (it->first > n || (it->first == n && str.compare(it->second) < 0))
      {
        it = V.insert(it, std::make_pair(n, str));
        if (V.size() >= 10) V.pop_back();
        break;
      }
      else
        ++it;
    }
  }

  return V;
}

std::vector<std::pair<int, std::string> > recommend_for_transfer(std::string input, std::string logedin, AccountT Table)
{
  std::vector<std::pair<int, std::string> > result;

  for (AccountT::iterator it = Table.begin(); it != Table.end(); ++it)
    if (it->first != logedin)
      result = put_into_vector(score(input, it->first), it->first, result);

  return result;
}

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
      {
        std::cout << "ID " << ID1 << " exists, ";
        std::vector<std::string> recV = recommend_for_create(ID1, T);
        for (int i=0;i<recV.size();i++)
        {
          std::cout << recV[i];
          if (i != recV.size() - 1)
            std::cout << ", ";
        }
        std::cout << std::endl;
      }
    }

    else if (op == "login") // log n
    {
      input >> ID1 >> PW1;
      AccountT::iterator id1_pos = T.find(ID1);
      if (id1_pos == T.end())
        std::cout << "ID " << ID1 << " not found\n";
      else if (id1_pos->second.password != md5(PW1))
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
      else if (id1_pos->second.password != md5(PW1))
        std::cout << "wrong password1\n";
      else if (id2_pos->second.password != md5(PW2))
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

        std::cout << "success, " << ID1 << " has " << id1_pos->second.balance << " dollars\n";
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
      {
        std::cout << "ID " << ID1 << " not found, ";
        std::vector<std::pair<int, std::string> > recV = recommend_for_transfer(ID1, logedinID, T);
        for (int i=0;i<recV.size();i++)
        {
          std::cout << recV[i].second;
          if (i != recV.size() - 1)
            std::cout << ", ";
        }
        std::cout << std::endl;
      }
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
          for (std::vector<int>::iterator it = lst.begin(); it != lst.end(); ++it) //여기 체크
            print_log(Hist[*it], logedinID);
        }
      }
    }


  }

  return 0;
}

