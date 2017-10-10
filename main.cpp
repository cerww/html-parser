#include <iostream>
#include "htmlParser.h"
#include "randomThings.h"

int main(){

	htmlParser b("<body><aaa lopy = \"awesome\" class = \"blue\"><!-- --><abc/></aaa></body>");
	auto c = b.getTree();
	while (true);


	int asdasdi;
	std::cin >> asdasdi;
	return 0;
}