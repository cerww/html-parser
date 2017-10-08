#include <iostream>
#include "htmlParser.h"
#include "randomThings.h"

int main(){
	std::string aew = "abacD";
	std::cout << find_first_if_not(aew, 0, [](char c) {return c == 'a'; }, [](char c) {return c == 'b'; });

	htmlParser b("<body><aaa l = \"awesome\"></aaa></body>");
	auto c = b.getTree();
	while (true);


	int asdasdi;
	std::cin >> asdasdi;
	return 0;
}