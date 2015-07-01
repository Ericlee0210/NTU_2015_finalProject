#include <string>
#include <string.h>
#include <vector>
#include <utility>

#ifndef DATA_VECTOR_H
#define DATA_VECTOR_H

typedef unsigned long long int int_64; //define new type of int, let money won't overflow

/************  New Container definition for recommend function  ******/																    
  typedef std::vector<std::vector<std::vector<int> > > b_vector;    
  typedef std::vector<std::vector<int> >               m_vector;    
  typedef std::vector<int>                             s_vector;    
  typedef std::vector<std::string>                     str_vector;  
 /*********************************************************************/

#define account_size 400000 // to define vector size in account_table

class Log //every transactions
{
public:
	int flag; //0: no orders, 1: from go first, 2: to go first
	std::string from;
	std::string to;
	int_64 amount;
	Log(std::string _from, std::string _to, int_64 _amount): from(_from), to(_to), amount(_amount){flag=0;}
	Log();
};

class info //every account ID content
{
public:
	std::vector<int> t_index;
	std::string id;
	std::string cpassword;
	int_64 balance;
	info(std::string _id, std::string _cpassword, int_64 _balance):id(_id), cpassword(_cpassword), balance(_balance){ t_index.reserve(2000); }
	info(){ t_index.reserve(2000); }	
};

class account_table //bank system
{
public:
	std::vector<info*> account_list;
    std::vector<Log*> transactions;
	int_64 last_successful_login_id_index;
	std::string last_successful_login_id;

	account_table()
	{
		account_list.reserve(account_size);
		transactions.reserve(account_size); 
	}

	/**************************************  API for account_table(a.k.a bank sysyem)  ***********************************************/	
	void check(const std::string & id, std::pair<bool, int> & result);
	void login(const std::string & id, const std::string & cpassword);
	void create(const std::string & id, const std::string & cpassword);
	void del(const std::string & id, const std::string & cpassword);
	void merge(const std::string & id1, const std::string & cpassword1, const std::string & id2, const std::string & cpassword2);
	void deposit(const int_64 & a);
	void withdraw(const int_64 & a);
	void transfer(const std::string & id, const int_64 & a);
	void find(const std::string & wild_string);
	bool wildcmp(char *first, char * second);
	void search(const std::string & id);
	void recommend(const std::string & str, str_vector & result, const unsigned int & numbers);
	/*********************************************************************************************************************************/	
};

void swap(info* & a, info* & b); // For create function

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

/****  Functions for create, transfer functions, which enable us to recommend IDs ****/
void get_pos_set(const int & n);
str_vector permute(const int & n);
void change_char_at_pos(const std::string & str, const s_vector & N);
int score(const std::string & str1, const std::string & str2);
bool bs(const std::vector<int> & v, const int & target);
/*************************************************************************************/

/**************  Debugger  ****************/
void printInfo(const info * info_content);
void printLog(const Log * log_content);
/******************************************/

#endif