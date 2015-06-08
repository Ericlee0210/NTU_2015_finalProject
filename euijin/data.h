#ifndef DATA
#define DATA

#include <iostream>
#include <vector>
#include <map>

struct Log
{
  std::string from;
  std::string to;
  int amount;

  Log(std::string from, std::string to, int amount) : from(from), to(to), amount(amount) {}
};

struct Info
{
  std::string password;
  int balance;
  std::vector<int> log_idx;
};

typedef std::map<std::string, Info> AccountT;
typedef std::vector<Log> History;

void print_log(Log l, std::string ID);
void print_info(std::string ID, AccountT T, History H);
bool smatch(std::string str1, std::string str2);

#endif
