#include <iostream>
#include <sstream>
#include <vector>
#include "data.h"
#include "math.h"

// AccountT = std::map<std::string, Info>

//int score(std::string str1, std::string str2)
//{
//  int min = std::min(str1.length(), str2.length());
//  int max = std::max(str1.length(), str2.length());
//  int temp = (max - min) * (max - min + 1) / 2;
//  for (int i = 0; i < min; i++)
//    if (str1[i] != str2[i])
//      temp += min - i;
//  return temp;
//}

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
std::vector<std::string> recommend(std::string str)
{
  int score = 1;
  std::vector<std::string> result;

  while (1)
  {
    std::vector<std::string> temp; // 임시로 모아둠

    int s_len = 0;
    int s_mod; // score - s_len
    int p_cnt = 0;

//    std::cout << "**************************\nSCORE : " << score << "\n*********************\n";
    while (s_len <= score)
    {
      s_mod = score - s_len;

      std::vector<std::vector<int> > pos_set = get_pos_set(s_mod);

//      std::cout << "SCORE : " << score << " s_len : " << s_len << " s_mod : " << s_mod << std::endl;

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

//      for (std::vector<std::string>::iterator it = temp.begin(); it != temp.end(); ++it)
//        std::cout << *it << std::endl;

      p_cnt++;
      s_len += p_cnt;
    }
//    std::cout << "*****SCORE " << score << " *******\n";

    std::sort(temp.begin(),temp.end());

    std::vector<std::string>::iterator it = temp.begin();
    while(result.size() < 1000000000 && it != temp.end())
    {
      if (*it != str)
      {
        if (result.size() != 0 && *it != result.back())
          result.push_back(*it);
        else if (result.size() == 0)
          result.push_back(*it);
      }
      ++it;
    }

//    std::cout << "TEMP\n";
//    for (int i=0;i<temp.size();i++)
//      std::cout << temp[i] << std::endl;
//    std::cout << "RESULT\n";
//    for (int i=0;i<result.size();i++)
//      std::cout << result[i] << std::endl;

    if (result.size() == 1000000000)
      break;

    score++;

    temp.clear();
  }

  return result;
}

int main()
{
  std::string row;
  std::string str1, str2;
  str1 = "abc";
  Info info;
  info.password = "123";
  info.balance = 3;
  AccountT Table;
  Table["abc"] = info;

  std::vector<std::string> result = recommend(str1);
  std::cout << "*********************\nRESULT\n************************\n";
  for (int i=0;i<result.size();i++)
  {
    std::cout << result[i] << std::endl;
  }

//  std::vector<int> temp;
//  temp.push_back(3);
//  temp.push_back(1);
//  std::vector<std::string> V = change_char_at_pos(str1, temp);
//
//  for (int i=0;i<V.size();i++)
//  {
//    std::cout << V[i] << std::endl;
//  }

  return 0;
}
