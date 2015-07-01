#include <iostream>
#include <algorithm>
#include <string>
#include <string.h>
#include <utility>
#include "data_vector.h"

/* Save all list of output for each score input
   TABLE[0] saves all score1 permutation
   TABLE[1] saves all score2 permutation
   TABLE[2] saves all score3 permutation, e.g. (3), (2,1)
   TABLE[3] saves all score4 permytation, e.g. (4), (3,1)
   TABLE[3] saves all score5 permytation, e.g. (5), (4,1), (3,2) */
b_vector TABLE; 

/* Saves all examples of permutation in change_char_at_pos funcitons */
str_vector RESULT_BY_CHANGE_CHAR;

void account_table::check(const std::string & id, std::pair<bool, int> & result)
{
	int head =0;
	int tail = this->account_list.size()-1;
	int mid = (head+tail)/2;
	while(head <= tail)
	{
		if( strcmp( id.c_str(), this->account_list.at(mid)->id.c_str() ) < 0 )  //search left part
		{
			tail = mid-1;
			mid = (head+tail)/2;
		}
		else if( strcmp( id.c_str(), this->account_list.at(mid)->id.c_str() ) > 0 ) //search right part
		{
			head = mid+1;
			mid = (head+tail)/2;
		}
		else
		{
			result = std::make_pair( true, mid );
			return;
		}
	}
	result = std::make_pair( false, mid );
	return;
}

void account_table::login(const std::string & id, const std::string & cpassword)
{
	std::pair<bool, int> output;  
	this->check( id, output );

	if(output.first)
	{
		if( this->account_list.at(output.second)->cpassword != cpassword)
			std::cout << "wrong password" << std::endl;
		else
		{
			this->last_successful_login_id_index = output.second;
			this->last_successful_login_id = id;
			std::cout << "success" << std::endl;
		}
	}
	else
		std::cout << "ID " << id << " not found" << std::endl; 
	return;
}

void account_table::create(const std::string & id, const std::string & cpassword)
{
	std::pair<bool, int> output;  
	this->check( id, output );

	if(output.first)
	{
		std::cout << "ID " << id << " exists, ";

		str_vector result; result.reserve(10);
		this->recommend(id, result, 10);
		for(unsigned int i=0; i<result.size(); ++i)
		{
			if(i==0)
				std::cout << result[i];
			else
				std::cout << "," << result[i];
		}
		std::cout << std::endl;
	}
	else //id doesn't exist, so creat a new one
	{
		unsigned int current_size = this->account_list.size();
		if (current_size == this->account_list.capacity())
			this->account_list.reserve(current_size*2);

		info* new_account= new info(id, cpassword, 0ULL);
		this->account_list.push_back(new_account);
		//std::cout << "this->account_list.size(): " << this->account_list.size() << std::endl;
		for(int i=this->account_list.size()-1; i>0; i--)
		{
			if( strcmp(id.c_str(), this->account_list.at(i-1)->id.c_str() ) < 0  )
			{
				//std::cout << "exchange " << i << " and " << i-1 << std::endl;
				//std::cout << "i-1 points to " << this->account_list[i-1] << std::endl;
				//std::cout << "i   points to " << this->account_list[i]   << std::endl;
				swap(this->account_list.at(i-1), this->account_list.at(i) );
				//std::cout << "After exchange " << i << " and " << i-1 << std::endl;
				//std::cout << "i-1 points to " << this->account_list[i-1] << std::endl;
				//std::cout << "i   points to " << this->account_list[i]   << std::endl;
			}
			else
				break;
		}
		std::cout << "success" << std::endl;		
	}
	return;
}

void account_table::del(const std::string & id, const std::string & cpassword)
{
	std::pair<bool, int> output;  
	this->check( id, output );

	if(output.first)
	{
		if( this->account_list.at(output.second)->cpassword != cpassword)
			std::cout << "wrong password" << std::endl;
		else
		{
			delete *(this->account_list.begin()+output.second);
			this->account_list.erase(this->account_list.begin()+output.second);
			std::cout << "success" << std::endl;
		}		
	}
	else
		std::cout << "ID " << id << " not found" << std::endl; 
	return;
}

void account_table::merge(const std::string & id1, const std::string & cpassword1, const std::string & id2, const std::string & cpassword2)
{
	std::pair<bool, int> output1;  
	this->check( id1, output1 );

	std::pair<bool, int> output2;  
	this->check( id2, output2 );

	if(!output1.first)
		std::cout << "ID " << id1 << " not found" << std::endl; 
	else if(!output2.first)
		std::cout << "ID " << id2 << " not found" << std::endl;
	else if( this->account_list.at(output1.second)->cpassword != cpassword1 )
		std::cout << "wrong password1" << std::endl;
	else if( this->account_list.at(output2.second)->cpassword != cpassword2 )
		std::cout << "wrong password2" << std::endl;
	else
	{
		int_64 & tmp = this->account_list.at(output1.second)->balance;
		tmp += this->account_list.at(output2.second)->balance;
		this->account_list.at(output2.second)->balance = 0;

		//change all the ID of transactions of id2 to id1
		for(std::vector<int>::iterator it = this->account_list[output2.second]->t_index.begin(); it!=this->account_list[output2.second]->t_index.end(); ++it)
		{
			if(this->transactions[*it]->to == id1 && this->transactions[*it]->from == id2)
			{
				this->transactions[*it]->flag =1;
				this->transactions[*it]->from = id1;
			}
			else if(this->transactions[*it]->from == id1 && this->transactions[*it]->to == id2)
			{
				this->transactions[*it]->flag =2;
				this->transactions[*it]->to = id1;
			}
			else
			{
				if(this->transactions[*it]->from == id2)
					this->transactions[*it]->from = id1;
				if(this->transactions[*it]->to == id2)
					this->transactions[*it]->to = id1;				
			}

			bool exist = bs(this->account_list[output1.second]->t_index, *it);
			if(!exist)
			{
				this->account_list[output1.second]->t_index.push_back(*it);
			}
		}
		std::sort(account_list[output1.second]->t_index.begin(), account_list[output1.second]->t_index.end());

		delete this->account_list[output2.second];
		this->account_list.erase(this->account_list.begin()+output2.second);
		std::cout << "success, " << id1 << " has " << tmp << " dollars" << std::endl;
	}
	return;
}

void account_table::deposit(const int_64 & a)
{
	if( last_successful_login_id_index > this->account_list.size()-1 ||
	    this->account_list.at(last_successful_login_id_index)->id != this->last_successful_login_id )
	{
		std::pair<bool, int> output;
		this->check(this->last_successful_login_id, output);
		this->last_successful_login_id_index = output.second;
	}
		
	int_64 & tmp = this->account_list.at(last_successful_login_id_index)->balance;
	tmp += a;
	std::cout << "success, " << tmp  << " dollars in current account" << std::endl;
	return;
}

void account_table::withdraw(const int_64 & a)
{
	if( last_successful_login_id_index > this->account_list.size()-1 ||
	    this->account_list.at(last_successful_login_id_index)->id != this->last_successful_login_id )
	{
		std::pair<bool, int> output;
		this->check(this->last_successful_login_id, output);
		this->last_successful_login_id_index = output.second;
	}

	int_64 & tmp = this->account_list.at(last_successful_login_id_index)->balance;
	if( a > tmp )
		std::cout << "fail, " << tmp  << " dollars only in current account" << std::endl;
	else
	{
		tmp -= a;
		std::cout << "success, "<< tmp << " dollars left in current account" << std::endl;
	}
	return;
}

void account_table::transfer(const std::string & id, const int_64 & a)
{
	if( last_successful_login_id_index > this->account_list.size()-1 ||
	    this->account_list.at(last_successful_login_id_index)->id != this->last_successful_login_id )
	{
		std::pair<bool, int> output1;
		this->check(this->last_successful_login_id, output1);
		this->last_successful_login_id_index = output1.second;
	}
	//std::cout << "last_successful_login_id      : " << this->last_successful_login_id << std::endl;
	//std::cout << "last_successful_login_id_index: " << this->last_successful_login_id_index << std::endl;
	int_64 & tmp = this->account_list.at(last_successful_login_id_index)->balance;

	std::pair<bool, int> output2;  
	this->check( id, output2 );
	//std::cout << "output2.first: " << output2.first << std::endl;
	if(!output2.first)
	{
		std::cout << "ID "<< id <<" not found, ";

		std::vector<myclass> score_list;
		score_list.reserve(this->account_list.size());
		//std::cout << "this->account_list.size(): "<< this->account_list.size()  << std::endl;
		//recommend for existing id
		for(unsigned int it = 0; it<this->account_list.size(); ++it )
		{
			//std::cout << "it= " << it << std::endl;
			int s = score(id, this->account_list[it]->id);
			myclass tmp(s, this->account_list[it]->id);
			score_list.push_back(tmp);
		}
		std::sort( score_list.begin(), score_list.end() );
		
		//for(int ii=0; ii<10 ; ++ii)
		//{
		//	std::cout << "score_list["<<ii<<"].score : " << score_list[ii].score << std::endl;
		//	std::cout << "score_list["<<ii<<"].string: " << score_list[ii].stringValue<< std::endl;
		//}

		for(int ii = 0; ii < 10 ; ++ii)
		{
			if(ii==0)
				std::cout << score_list[ii].stringValue;
			else
				std::cout << "," << score_list[ii].stringValue;
		}
		std::cout << std::endl;
	}
	else if(tmp < a)
		std::cout << "fail, " << tmp << " dollars only in current account" << std::endl;
	else
	{
		tmp -= a;
		this->account_list[output2.second]->balance += a ;

		Log* record = new Log(this->last_successful_login_id, id, a);
		//to record the transaction in each id account
		unsigned int index = this->transactions.size();
		this->account_list[last_successful_login_id_index]->t_index.push_back(index); 
		this->account_list[output2.second]->t_index.push_back(index);

		this->transactions.push_back(record);
		std::cout << "success, "<< tmp <<" dollars left in current account" << std::endl;
	}
	//for(unsigned int aaa=0; aaa<this->transactions.size(); ++aaa)
	//{
	//	std::cout << "transactions[" << aaa << "]" << std::endl;
	//	printLog(this->transactions[aaa]);
	//}
	return;
}

void account_table::find(const std::string & wild_string)
{
	int_64 count=0;
	for(unsigned int i=0; i<this->account_list.size(); i++)
	{
		unsigned int c_str_size = wild_string.size();
		char * a = new char [c_str_size+1];
		strncpy(a, wild_string.c_str(), c_str_size+1);

		c_str_size = this->account_list[i]->id.size();
		char * b = new char [c_str_size+1];
		strncpy(b, this->account_list[i]->id.c_str(), c_str_size+1);

		bool flag = this->wildcmp(a, b);
		if(flag && count==0)
		{
			std::cout << this->account_list[i]->id;
			count++;
		}
		else if(flag && count!=0)
			std::cout << "," <<this->account_list[i]->id;
		delete []a;
		delete []b;
	}
	std::cout << std::endl;
	return;
}

bool account_table::wildcmp(char *first, char * second)
{
    if (*first == '\0' && *second == '\0')	return true;  // If we reach at the end of both strings, we are done
    if(*first == '?' && *second == '\0')  	return false;
	if(*first == '*' && *second == '\0' && *(first+1) == '*')   return wildcmp(first+1,second);
    if (*first == '*' && *(first+1) != '\0' && *second == '\0') return false;
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
	if( this->last_successful_login_id_index > this->account_list.size()-1 ||
	    this->account_list.at(last_successful_login_id_index)->id != this->last_successful_login_id )
	{
		std::pair<bool, int> output;
		this->check(this->last_successful_login_id, output);
		this->last_successful_login_id_index = output.second;
	}
	int count=0; 
	if(id == this->last_successful_login_id)
	{
		for(std::vector<int>::iterator i=this->account_list[last_successful_login_id_index]->t_index.begin(); i!=this->account_list[last_successful_login_id_index]->t_index.end(); i++)
		{
			//printLog(this->transactions[*i]);
			if(this->transactions[*i]->flag==0) continue;
			if(this->transactions[*i]->flag==1)
			{
				std::cout << "From " << this->last_successful_login_id << " "<< this->transactions[*i]->amount << std::endl;
				std::cout << "To " << this->last_successful_login_id << " "<< this->transactions[*i]->amount << std::endl;
			}
			else if(this->transactions[*i]->flag==2)
			{
				std::cout << "To " << this->last_successful_login_id << " "<< this->transactions[*i]->amount << std::endl;
				std::cout << "From " << this->last_successful_login_id << " "<< this->transactions[*i]->amount << std::endl;				
			}
			count++;
			
		}
	}
	else //(1)last id give some money to others, (2)accept some money from others
	{
		for(std::vector<int>::iterator i=this->account_list[last_successful_login_id_index]->t_index.begin(); i!=this->account_list[last_successful_login_id_index]->t_index.end(); i++)
		{
			//printLog(this->transactions[*i]);
			if(this->transactions[*i]->flag!=0) continue;
			if(this->transactions[*i]->to != id && this->transactions[*i]->from != id) continue;
			count++;
			if(this->transactions[*i]->to == id) //(1)
				std::cout << "To " << id << " "<< this->transactions[*i]->amount << std::endl;
			else if(this->transactions[*i]->from == id)
				std::cout << "From " << id << " "<< this->transactions[*i]->amount << std::endl;
		}	
	}
	if(count==0)
		std::cout << "no record" << std::endl;	
	return;
}

/* Input : string
 * Output : vector of string which we recommend 
 * Algorithm :
 * Iterate by score
 * Divide score as score_by_length and score_by_modification
 * There are two way to recommend, One is Cut first and Modify, The other is Modify first and Add permutation */
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
		/********************************** Debugger *******************************************
    	 *  for (std::vector<std::string>::iterator it = temp.begin(); it != temp.end(); ++it) *
         *		std::cout << *it << std::endl; 												   *
    	 **************************************************************************************/
    	p_cnt++;
    	s_len += p_cnt;
    }

    std::sort( temp.begin(), temp.end() );
    str_vector::iterator it = temp.begin();
    while( result.size() < numbers && it != temp.end() )
    {
    	std::pair<bool, int> output( true, 1 );  
		this->check( *it, output );
    	if (*it != str && output.first==false) //not equal to original string, and it can't already exist
    	{
    		if (result.size() != 0 && *it != result.back())
    	 		result.push_back(*it);
    	 	else if (result.size() == 0)
    	   		result.push_back(*it);
    	}
     	++it;
    }
    if (result.size() == numbers)
      break;

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
  //std::string set[] = {"a","b","c","d","e"};
  std::string set[] = {"0","1","2","3","4","5","6","7","8","9",
  "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
  "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

  if (n == 0)
    return result;

  if (n == 1)
  {
    for (int i=0;i<62;i++)
    //for (int i=0;i<5;i++)
      result.push_back(set[i]);
    return result;
  }
  else
  {
    str_vector temp = permute(n-1);
    for (str_vector::iterator it = temp.begin(); it != temp.end(); ++it)
    {
      for (int i=0;i<62;i++)
      //for (int i=0;i<5;i++)
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
void change_char_at_pos(const std::string & str, const s_vector & N)
{
  RESULT_BY_CHANGE_CHAR.push_back(""); // If it is empty I can start so add empty string.
//  std::string set[] = {"a","b","c","d","e"};
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
      //for (int i=0;i<5;i++)
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

void swap(info* & a, info* & b)
{
	info* c = a;
	a = b;
	b = c;
	return;
}

bool bs(const std::vector<int> & v, const int & target)
{
	int head =0;
	int tail = v.size()-1;
	int mid = (head+tail)/2;
	while(head<=tail)
	{
		if( target > v[mid] )
		{
			head=mid+1;
			mid = (head+tail)/2;
		}
		else if( target < v[mid] )
		{
			tail=mid-1;
			mid = (head+tail)/2;
		}
		else
			return true;
	}
	return false;
}

void printInfo(const info* info_content)
{
	std::cout << "id       : " << info_content->id        << std::endl;
	std::cout << "cpassword: " << info_content->cpassword << std::endl;
	std::cout << "balance  : " << info_content->balance   << std::endl;
	return;
}

void printLog(const Log * log_content)
{
	std::cout << "           flgg: " << log_content->flag    << std::endl;
	std::cout << "             to: " << log_content->to    << std::endl;
	std::cout << "           from: " << log_content->from      << std::endl;
	std::cout << "         amount: " << log_content->amount << std::endl; 
	return;
}