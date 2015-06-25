#include <iostream>
#include <sstream>
#include <vector>
#include "data.h"
#include "math.h"

typedef std::vector<std::vector<std::vector<int> > > b_vector;
typedef std::vector<std::vector<int> >               m_vector;
typedef std::vector<int>                             s_vector;
typedef std::vector<std::string>                     str_vector;

/* Save all list of output for each score input
   TABLE[0] saves all score1 permutation
   TABLE[1] saves all score2 permutation
   TABLE[2] saves all score3 permutation, e.g. (3), (2,1)
   TABLE[3] saves all score4 permytation, e.g. (4), (3,1)
   TABLE[3] saves all score5 permytation, e.g. (5), (4,1), (3,2) */
b_vector TABLE; 

/* Saves all examples of permutation in change_char_at_pos funcitons */
str_vector RESULT_BY_CHANGE_CHAR;

/* Input : score
 * Output : vector of int vector, each int vector is set of position which make sum as score 
 * example)
 * Input : 5
 * Output : (5) -> (4->1) -> (3->2) */
void get_pos_set(const int & n)
{
  static bool flag = 1;
  m_vector temp_lst;
  s_vector temp_set;

  if (n == 0) return;// output has no content so that we can check with size() funtion

  if(flag)
  {
    temp_set.push_back(1);
    temp_lst.push_back(temp_set);
    TABLE.push_back(temp_lst);
    temp_set.clear();
    temp_lst.clear();

    temp_set.push_back(2);
    temp_lst.push_back(temp_set);
    TABLE.push_back(temp_lst);
    temp_set.clear();
    temp_lst.clear();

    flag=0;
  }
  if (n==1 || n == 2) return;

  for (int i = 3; i <= n; i++)
  {
    /* If get_pos_set function runs a lot of times in the future,
     * TABLE[2]~TABLE[s] (2<=s<=n) may have already been calculated.
     * There is no need to calculate it again. */
    if(TABLE[i-1].size()!=0)  continue;  

    temp_set.push_back(i);
    temp_lst.push_back(temp_set);
    temp_set.clear();

    int cnt = i-1;
    while(cnt > i/2)
    {
      for (m_vector::iterator it = TABLE[i-cnt-1].begin(); it != TABLE[i-cnt-1].end(); ++it)
      {
        temp_set.push_back(cnt);

        for(s_vector::iterator inner_it = it->begin(); inner_it != it->end(); ++inner_it)
          temp_set.push_back(*inner_it);

        temp_lst.push_back(temp_set);
        temp_set.clear();
      }
      cnt--;
    }
    TABLE.push_back(temp_lst);
    temp_lst.clear();
  }
  return;
}

/* Input : length we want to make
 * Output : vector of permuted string
 * example)
 * Input : 3 -> Output : 000 to zzz */
str_vector permute(const int & n) // If we put those function inside main function, is it okay with memory?
{
  str_vector result;
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
    str_vector temp = permute(n-1);
    for (str_vector::iterator it = temp.begin(); it != temp.end(); ++it)
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
void change_char_at_pos(const std::string & str, const s_vector & N) // 숫자 벡터가 idx를 0부터 카운트한다고 생각하고 함.
{
  RESULT_BY_CHANGE_CHAR.push_back(""); // If it is empty I can start so add empty string.
  std::string set[] = {"a","b","c","d","e"};
//  std::string set[] = {"0","1","2","3","4","5","6","7","8","9",
//  "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
//  "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

  int idx = 0;
  s_vector::const_iterator pos = N.begin();

  while(pos != N.end()) // During iterations, when we confront pos change it from 0 to z.
  {
    if (str.length() - *pos > idx) //Maintain character positions that vector N doesn't mention.
    {
      for (str_vector::iterator it = RESULT_BY_CHANGE_CHAR.begin(); it != RESULT_BY_CHANGE_CHAR.end(); ++it)
        *it += str[idx];
      idx++;
    }
    else //Change charactor positions that vector N mention.
    {
      str_vector tempV;
      for (str_vector::iterator it = RESULT_BY_CHANGE_CHAR.begin(); it != RESULT_BY_CHANGE_CHAR.end(); ++it)
      {
      //for (int i=0;i<62;i++)
        for (int i=0;i<5;i++)
          tempV.push_back(*it + set[i]);
      }
      RESULT_BY_CHANGE_CHAR = tempV; //already modify all the elements in RESULT_BY_CHANGE_CHAR, and give them back to itself.
      tempV.clear();
      ++pos; idx++;
    }
  }

  while (idx < str.length()) // If while loop ends because we changed all, add left part of string.
  {
    for (str_vector::iterator it = RESULT_BY_CHANGE_CHAR.begin(); it != RESULT_BY_CHANGE_CHAR.end(); ++it)
      *it += str[idx];
    idx++;
  }

  return;
}

/* Input : string
 * Output : vector of string which we recommend 
 * Algorithm :
 * Iterate by score
 * Divide score as score_by_length and score_by_modification
 * There are two way to recommend, One is Cut first and Modify, The other is Modify first and Add permutation */
str_vector recommend(const std::string & str)
{
  int score = 1;
  str_vector result;

  while (1)
  {
    str_vector temp;

    int s_len = 0;
    int s_mod; // score - s_len
    int p_cnt = 0;

    while (s_len <= score)
    {
      s_mod = score - s_len;
      get_pos_set(s_mod); //calculate score(s_mod) permutation set

      // First step cut first and modify
      if (str.length() > p_cnt)
      {
        std::string temp_str = str.substr(0, str.length() - p_cnt); //cut length

        if (s_mod == 0) temp.push_back(temp_str); // Add string, which is only cut
        else //Add string; there is modification
        {
          for (m_vector::iterator it = TABLE[s_mod-1].begin(); it != TABLE[s_mod-1].end(); ++it)
          {
            if (it->front() <= temp_str.length()) //Need to check, whether the modification target position is within string.
            {
              change_char_at_pos(temp_str, *it);
              std::copy (RESULT_BY_CHANGE_CHAR.begin(),RESULT_BY_CHANGE_CHAR.end(),back_inserter(temp));
              RESULT_BY_CHANGE_CHAR.clear();
            }
          }
        }
      }

      // Second step modify first and add permutation
      str_vector pmt = permute(p_cnt);
      if (pmt.size() != 0)
      {
        if (str.length() + p_cnt <= 100) // Length of ID should be less than 100
        {
          if (s_mod == 0) // s_mod is 0
            for (str_vector::iterator it = pmt.begin(); it != pmt.end(); ++it)
              temp.push_back(str + *it);
          else
          {
            for (m_vector::iterator it = TABLE[s_mod-1].begin(); it != TABLE[s_mod-1].end(); ++it)
            {
              if (it->front() <= str.length())
              {
                change_char_at_pos(str, *it);

                for (str_vector::iterator i_it = RESULT_BY_CHANGE_CHAR.begin(); i_it != RESULT_BY_CHANGE_CHAR.end(); ++i_it)
                  for (std::vector<std::string>::iterator ii_it = pmt.begin(); ii_it != pmt.end(); ++ii_it)
                    temp.push_back(*i_it + *ii_it);
              }
              RESULT_BY_CHANGE_CHAR.clear();
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

    std::cout << "success" << std::endl;

    str_vector::iterator it = temp.begin();

    while(result.size() < 10000 && it != temp.end())
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

    if (result.size() == 10000)
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

  get_pos_set(10);
  //for(m_vector::iterator it=TABLE[9].begin(); it!=TABLE[9].end(); ++it)
  //{
  //  for(s_vector::iterator iit=it->begin(); iit!=it->end(); ++iit)
  //    std::cout << *iit << " ";
  //  std::cout << std::endl;
  //}

  //str_vector tmp = permute(2);
  //for(str_vector::iterator it=tmp.begin(); it!=tmp.end(); ++it)
  //  std::cout << *it << std::endl;

  //change_char_at_pos("string", TABLE[4][1]);
  //for(str_vector::iterator it=RESULT_BY_CHANGE_CHAR.begin(); it!=RESULT_BY_CHANGE_CHAR.end(); ++it)
  //  std::cout << *it << std::endl;

  std::vector<std::string> result = recommend(str1);
  std::cout << "*********************\nRESULT\n************************\n";
  for (int i=0;i<result.size();i++)
  {
    std::cout << result[i] << std::endl;
  }

  return 0;
}