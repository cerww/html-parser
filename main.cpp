#include <iostream>
#include "htmlParser.h"
#include "randomThings.h"
using namespace std::string_literals;

int main(){

	//htmlParser b("<body><aaa lopy = \"awesome\" class = \"blue\"><!-- --><abc/></aaa></body>");
	htmlParser b(getFileContents("test2.txt"));
	auto c = b.getTree();
	std::vector<htmlNode*> o = c.root->querySelectorAll("input"s);
	std::cout << o.size();
	while (true);


	int asdasdi;
	std::cin >> asdasdi;
	return 0;
}