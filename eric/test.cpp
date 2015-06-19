#include <vector>
#include <iostream>

struct log
{
	int b;
	log(int a): b(a){}
};

void printLog(log * s)
{
	std::cout << s->b << std::endl;
}

int main()
{
	std::vector<log> test;
	log a(33333);
	test.push_back(a);
	printLog(test[0]);
	return 0;
}

