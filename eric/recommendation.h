#include <vecotr>
#include <string>

#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H	

typedef std::vector<std::vector<std::vector<int> > > b_vector;
typedef std::vector<std::vector<int> >               m_vector;
typedef std::vector<int>                             s_vector;
typedef std::vector<std::string>                     str_vector;

void get_pos_set(const int & n);
str_vector permute(const int & n);
void change_char_at_pos(const std::string & str, const s_vector & N);
void recommend(const std::string & str, str_vector & result, const int & numbers);

#endif