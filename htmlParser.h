#pragma once
#include "htmlTree.h"
#include <unordered_set>
constexpr bool notFound = false;
constexpr bool found = true;


class htmlParser{
public:
	htmlParser(std::string stuffs);
	htmlParser() = default;
	htmlTree getTree();
	
private:
	std::string m_data;
	std::vector<htmlNode*> m_nodeQueue;
	void addNode(std::string_view);
	std::unordered_map<std::string_view, int> m_usedTagNames;
	bool removeTillTagName(std::string_view tagName);
	size_t addAttributes(size_t);
	size_t foundNewNode(size_t);
	size_t foundEndTag(size_t);
	size_t foundTextNode(size_t);
	bool isImmediatelyEnddedTag(const std::string&)const;
};

