#include <cstdio>
#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
#include "data.h"

b_vector TABLE; 
str_vector RESULT_BY_CHANGE_CHAR;

void account_table::login(const std::string & id, const std::string & cpassword)
{
	iter_m i = this->account_list.find(id);
	if( i== this->account_list.end() ) //not found
		printf("ID %s not found\n", id.c_str() );
	else if( i->second->cpassword != cpassword)
		printf("wrong password\n");
	else
	{
		this->last_successful_login_id = id;
		printf("success\n");		
	}
	return;
}

void account_table::create(const std::string & id, const std::string & cpassword)
{
	iter_m i = this->account_list.find(id);
	if( i== this->account_list.end() ) //not found
	{
		info* new_account= new info(cpassword, 0);
		this->account_list.insert( std::pair<std::string, info*>(id, new_account) );
		printf("success\n");	
	}
	else //existing
	{
		printf("ID %s exists, ", id.c_str());
		str_vector result; result.reserve(10);
		this->recommend(id, result, 10);
		for(unsigned int j=0; j<result.size(); ++j)
		{
			if(j==0)  printf("%s" , result[j].c_str());
			else      printf(",%s", result[j].c_str());
		}
		printf("\n");
	}
	return;
}

void account_table::del(const std::string & id, const std::string & cpassword)
{
	iter_m i = this->account_list.find(id);
	if( i== this->account_list.end() ) //not found
		printf("ID %s not found\n", id.c_str());
	else
	{
		if( i->second->cpassword != cpassword)
			printf("wrong password\n");
		else //delete account
		{
			delete i->second;
			this->account_list.erase(i);
			printf("success\n");
		}	
	}
	return;
}

void account_table::merge(const std::string & id1, const std::string & cpassword1, const std::string & id2, const std::string & cpassword2)
{
	iter_m i1 = this->account_list.find(id1);
	iter_m i2 = this->account_list.find(id2);

	if( i1== this->account_list.end() )
		printf("ID %s not found\n", id1.c_str());
	else if( i2== this->account_list.end() )
		printf("ID %s not found\n", id2.c_str());
	else if( i1->second->cpassword != cpassword1 )
		printf("wrong password1\n");
	else if( i2->second->cpassword != cpassword2 )
		printf("wrong password2\n");
	else
	{
		int_u & tmp = i1->second->balance;
		tmp += i2->second->balance;
		i2->second->balance = 0;

		//change all the ID of transactions of id2 to id1
		for(iter_v i = i2->second->t_index.begin(); i!=i2->second->t_index.end(); ++i)
		{
			if(this->transactions[*i]->to == id1 && this->transactions[*i]->from == id2)
			{
				this->transactions[*i]->flag = 1;
				this->transactions[*i]->from = id1;
			}
			else if(this->transactions[*i]->from == id1 && this->transactions[*i]->to == id2)
			{
				this->transactions[*i]->flag = 2;
				this->transactions[*i]->to   = id1;
			}
			else
			{
				if(this->transactions[*i]->from == id2)  this->transactions[*i]->from = id1;
				if(this->transactions[*i]->to   == id2)  this->transactions[*i]->to   = id1;				
			}

			bool exist = bs(i1->second->t_index, *i);
			if(!exist)
				i1->second->t_index.push_back(*i);
		}
		std::sort(i1->second->t_index.begin(), i1->second->t_index.end());

		delete i2->second;
		this->account_list.erase(i2);
		printf("success, %s has %d dollars\n", id1.c_str(), tmp);
	}
	return;
}

void account_table::deposit(const int_u & a)
{
	info* i = this->account_list[this->last_successful_login_id];
		
	int_u & tmp = i->balance;
	tmp += a;
	printf("success, %d dollars in current account\n", tmp);
	return;
}

void account_table::withdraw(const int_u & a)
{
	info* i = this->account_list[this->last_successful_login_id];

	int_u & tmp = i->balance;
	if(a > tmp)
		printf("fail, %d dollars only in current account\n", tmp);
	else
	{
		tmp -= a;
		printf("success, %d dollars left in current account\n", tmp);
	}
	return;
}

void account_table::transfer(const std::string & id, const int_u & a)
{
	// i1: last id, i2:target id
	iter_m i1 = this->account_list.find(this->last_successful_login_id);	
	iter_m i2 = this->account_list.find(id);

	int_u & tmp = i1->second->balance;

	if(i2 == this->account_list.end())
	{
		printf("ID %s not found, ", id.c_str());
		std::vector<myclass> score_list;
		score_list.reserve(this->account_list.size());
		for(iter_m it=this->account_list.begin(); it!=this->account_list.end(); ++it )
		{
			int s = score(id, it->first);
			myclass tmp(s, it->first);
			score_list.push_back(tmp);
		}
		std::sort( score_list.begin(), score_list.end() );

		for(int j = 0; j < 10 ; ++j)
		{
			if(j==0) printf("%s",  score_list[j].stringValue.c_str());
			else      printf(",%s", score_list[j].stringValue.c_str());
		}
		printf("\n");
	}
	else if(tmp < a)
		printf("fail, %d dollars only in current account\n", tmp );
	else
	{
		tmp -= a;
		i2->second->balance += a ;

		Log* record = new Log(this->last_successful_login_id, id, a);
		
		//to record the transaction in each id account
		unsigned int index = this->transactions.size();
		i1->second->t_index.push_back(index); 
		i2->second->t_index.push_back(index);

		this->transactions.push_back(record);
		printf("success, %d dollars left in current account\n", tmp);
	}
	return;
}

void account_table::find(const std::string & wild_string)
{
	int_u count=0;
	for(iter_m i=this->account_list.begin(); i!=this->account_list.end(); ++i)
	{
		unsigned int c_str_size = wild_string.size();
		char * a = new char [c_str_size+1];
		strncpy(a, wild_string.c_str(), c_str_size+1);

		c_str_size = i->first.size();
		char * b = new char [c_str_size+1];
		strncpy(b, i->first.c_str(), c_str_size+1);

		bool flag = this->wildcmp(a, b);
		if(flag && count==0)
		{
			printf("%s", i->first.c_str());
			count++;
		}
		else if(flag && count!=0)
			printf(",%s",i->first.c_str());
		delete []a;
		delete []b;
	}
	printf("\n");
	return;
}

bool account_table::wildcmp(char *first, char * second)
{
    if(*first == '\0' && *second == '\0')	return true;  // If we reach at the end of both strings, we are done
    if(*first == '?' && *second == '\0')  	return false;
	if(*first == '*' && *second == '\0' && *(first+1) == '*')   return wildcmp(first+1,second);
    if(*first == '*' && *(first+1) != '\0' && *second == '\0') return false;
    /*********************************************************************
     * If the first string contains '?', or current characters of both   *
     * strings match                                                     *
     *********************************************************************/
    if (*first == '?' || *first == *second)  return wildcmp(first+1, second+1);
    /*********************************************************************
     * If there is *, then there are two possibilities                   *
     * a) We consider current character of second string                 *
     * b) We ignore current character of second string.                  *
     *********************************************************************/
    if (*first == '*')  return wildcmp(first+1, second) || wildcmp(first, second+1);
    return false;
}

void account_table::search(const std::string & id)
{
	iter_m i = this->account_list.find(last_successful_login_id);
	int count=0; 
	if(id == this->last_successful_login_id)
	{
		for(iter_v j=i->second->t_index.begin(); j!=i->second->t_index.end(); j++)
		{
			//printLog(this->transactions[*j]);
			if(this->transactions[*j]->flag==0) continue;
			if(this->transactions[*j]->flag==1)
			{
				printf("From %s %d\n", this->last_successful_login_id.c_str(), this->transactions[*j]->amount);
				printf("To %s %d\n",   this->last_successful_login_id.c_str(), this->transactions[*j]->amount);
			}
			else if(this->transactions[*j]->flag==2)
			{
				printf("To %s %d\n", this->last_successful_login_id.c_str(), this->transactions[*j]->amount);
				printf("From %s %d\n",   this->last_successful_login_id.c_str(), this->transactions[*j]->amount);			
			}
			count++;
		}
	}
	else //(1)last id give some money to others, (2)accept some money from others
	{
		for(iter_v j=i->second->t_index.begin(); j!=i->second->t_index.end(); j++)
		{
			//printLog(this->transactions[*j]);
			if(this->transactions[*j]->flag!=0) continue;
			if(this->transactions[*j]->to != id && this->transactions[*j]->from != id) continue;
			count++;
			if(this->transactions[*j]->to == id) 
				printf("To %s %d\n"  , id.c_str(), this->transactions[*j]->amount);
			else if(this->transactions[*j]->from == id)
				printf("From %s %d\n", id.c_str(), this->transactions[*j]->amount);
		}	
	}
	if(count==0)
		printf("no record\n");
	return;
}

void account_table::recommend(const std::string & str, str_vector & result, const unsigned int & numbers)
{
	int score = 1;
	while (1)
	{
		str_vector temp;

		int s_len = 0;
		int s_mod; // score - s_len
		unsigned int p_cnt = 0;

		while ( s_len <= score )
		{
			s_mod = score - s_len;
			get_pos_set(s_mod); //calculate score(s_mod) permutation set

			// First step cut first and modify
			if (str.length() > p_cnt)
			{
			  std::string temp_str = str.substr(0, (int)str.length() - p_cnt); //cut length

				if (s_mod == 0) temp.push_back(temp_str); // Add string, which is only cut
				else //Add string; there is modification
			  	{
			    	for (m_vector::iterator it = TABLE[s_mod-1].begin(); it != TABLE[s_mod-1].end(); ++it)
			    	{
			      		if (it->front() <= (int)temp_str.length()) //Need to check, whether the modification target position is within string.
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
				if ((int)str.length() + p_cnt <= 100) // Length of ID should be less than 100
				{
					if (s_mod == 0) // s_mod is 0
						for (str_vector::iterator it = pmt.begin(); it != pmt.end(); ++it)
					    	temp.push_back(str + *it);
					else
					{
						for (m_vector::iterator it = TABLE[s_mod-1].begin(); it != TABLE[s_mod-1].end(); ++it)
						{
							if (it->front() <= (int)str.length())
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
    	p_cnt++;
    	s_len += p_cnt;
    }

    std::sort( temp.begin(), temp.end() );
    str_vector::iterator it = temp.begin();
    while( result.size() < numbers && it != temp.end() )
    {
    	iter_m i = this->account_list.find(*it);
    	if (*it != str && i==this->account_list.end()) //not equal to original string, and it can't already exist
    	{
    		if (result.size() != 0 && *it != result.back()) result.push_back(*it);
    	 	else if (result.size() == 0)                    result.push_back(*it);
    	}
     	++it;
    }
    if (result.size() == numbers) break;

    score++;
    temp.clear();
  }
  return;
}

int score(const std::string & str1, const std::string & str2)
{
  int str1_l = (int)str1.length();
  int str2_l = (int)str2.length();
  int min = std::min(str1_l, str2_l);
  int delta = (str1_l-str2_l>0)?(str1_l-str2_l):(str2_l-str1_l);
  int temp = delta * (delta+1) / 2;
  for (int i = 0; i < min; i++)
    if (str1[i] != str2[i])
      temp += (min - i);
  return temp;
}

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

str_vector permute(const int & n) // If we put those function inside main function, is it okay with memory?
{
  str_vector result;
  std::string set[] = {"0","1","2","3","4","5","6","7","8","9",
  "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
  "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

  if (n == 0)
    return result;

  if (n == 1)
  {
    for (int i=0;i<62;i++)
      result.push_back(set[i]);
    return result;
  }
  else
  {
    str_vector temp = permute(n-1);
    for (str_vector::iterator it = temp.begin(); it != temp.end(); ++it)
    {
      for (int i=0;i<62;i++)
        result.push_back(*it + set[i]);
    }
    return result;
  }
}

void change_char_at_pos(const std::string & str, const s_vector & N)
{
  RESULT_BY_CHANGE_CHAR.push_back(""); // If it is empty I can start so add empty string.
  std::string set[] = {"0","1","2","3","4","5","6","7","8","9",
  "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
  "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

  int idx = 0;
  s_vector::const_iterator pos = N.begin();

  while(pos != N.end()) // During iterations, when we confront pos change it from 0 to z.
  {
    if ((int)str.length() - *pos > idx) //Maintain character positions that vector N doesn't mention.
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
      	for (int i=0;i<62;i++)
      		tempV.push_back(*it + set[i]);
      }
      RESULT_BY_CHANGE_CHAR = tempV; //already modify all the elements in RESULT_BY_CHANGE_CHAR, and give them back to itself.
      tempV.clear();
      ++pos; idx++;
    }
  }

  while (idx < (int)str.length()) // If while loop ends because we changed all, add left part of string.
  {
    for (str_vector::iterator it = RESULT_BY_CHANGE_CHAR.begin(); it != RESULT_BY_CHANGE_CHAR.end(); ++it)
      *it += str[idx];
    idx++;
  }

  return;
}

bool bs(const std::vector<int> & v, const int & target)
{
	int head =0;
	int tail = v.size()-1;
	int mid = (head+tail)/2;
	while(head<=tail)
	{
		if     ( target > v[mid] ) { head=mid+1; mid = (head+tail)/2;}
		else if( target < v[mid] ) { tail=mid-1; mid = (head+tail)/2;}
		else  return true;
	}
	return false;
}