#pragma once
#include "htmlNode.h"

class htmlTree
{
public:
	htmlTree():root(std::make_unique<htmlNode>("document")){};
	std::unique_ptr<htmlNode> root = nullptr;
};

