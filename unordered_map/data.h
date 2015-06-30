#include <vector>
#include <string>
#include <string.h>
#include <unordered_map>

#ifndef DATA_H
#define DATA_H

typedef unsigned int int_u;

/************  New Container definition for recommend function  ******/																    
  typedef std::vector<std::vector<std::vector<int> > > b_vector;    
  typedef std::vector<std::vector<int> >               m_vector;    
  typedef std::vector<int>                             s_vector;    
  typedef std::vector<std::string>                     str_vector;  
 /*********************************************************************/



class Log //every transactions
{
public:
	int flag; //0: no orders, 1: from go first, 2: to go first
	std::string from;
	std::string to;
	int_u amount;
	Log(std::string _from, std::string _to, int_u _amount): from(_from), to(_to), amount(_amount){flag=0;}
};

class info //every account ID content
{
public:
	std::vector<int> t_index; //for storing related transactions index
	std::string cpassword;
	int_u balance;
	info(std::string _cpassword, int_u _balance): cpassword(_cpassword), balance(_balance){ t_index.reserve(2000); }
};
class account_table
{
public:
	std::unordered_map<std::string, info*> account_list;
	std::vector<Log*> transactions;
	std::string last_successful_login_id;
	account_table(){transactions.reserve(400000);}

	void login(const std::string & id, const std::string & cpassword);
	void create(const std::string & id, const std::string & cpassword);
	void del(const std::string & id, const std::string & cpassword);
	void merge(const std::string & id1, const std::string & cpassword1, const std::string & id2, const std::string & cpassword2);
	void deposit(const int_u & a);
	void withdraw(const int_u & a);
	void transfer(const std::string & id, const int_u & a);
	void find(const std::string & wild_string);
	bool wildcmp(char *first, char * second);
	void search(const std::string & id);
	void recommend(const std::string & str, str_vector & result, const unsigned int & numbers);

};

class myclass
{
public:
    int score;
    std::string stringValue;

    myclass(int _score, std::string _stringValue) : score(_score), stringValue(_stringValue) {}

	//to overload operator < for std::sort
    bool operator < (const myclass& str) const 
    { 
    	if(score < str.score)
    		return 1;
    	else if(score > str.score)
    		return 0;
    	else
    	{
    		if( strcmp( stringValue.c_str(), str.stringValue.c_str() ) < 0 )
    			return 1;
    		else
    			return 0;
    	}
    }  
};

int score(const std::string & str1, const std::string & str2);
void get_pos_set(const int & n);
str_vector permute(const int & n);
void change_char_at_pos(const std::string & str, const s_vector & N);
bool bs(const std::vector<int> & v, const int & target);

typedef std::unordered_map<std::string, info*>::iterator iter_m;
typedef std::vector<int>::iterator iter_v;

#endif