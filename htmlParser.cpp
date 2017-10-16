#include "htmlParser.h"
#include <iostream>
#include "randomThings.h"

htmlParser::htmlParser(std::string stuffs):m_data(stuffs+"     "){}

htmlTree htmlParser::getTree(){
	htmlTree retVal;
	m_nodeQueue.push_back(retVal.root.get());
	size_t current = 0;
	while (current < m_data.size() - 5){
		//skip white space
		while (m_data[current] == ' ' || m_data[current] == '\n' || m_data[current] == '\t')
			++current;
		if(m_data[current]=='<'){
			if (m_data[current + 1] == '/') 
				current = foundEndTag(current);			
			else if (isLetter(m_data[current + 1])) 
				current = foundNewNode(current);			
			else if (m_data[current + 1] == '!'&&m_data[current + 2] == '-'&&m_data[current + 3] == '-') //comment
				current = m_data.find("-->", current + 1) + 3;
			else
				current = foundTextNode(current);
		}else {
			current = foundTextNode(current);
		}
	}m_nodeQueue.clear();
	return retVal;
}

void htmlParser::addNode(std::string_view tagName){//new node's parent is the last node
	if (tagName.back() == '/') tagName.remove_suffix(1);
	m_nodeQueue.back()->data += "%{" + std::to_string(m_nodeQueue.back()->numChildren()) + "}";
	m_nodeQueue.push_back(m_nodeQueue.back()->addChild(std::make_unique<htmlNode>(std::string(tagName))));
	++m_usedTagNames[m_nodeQueue.back()->getTagName()];
}
bool htmlParser::removeTillTagName(std::string_view tagName){
	//std::cout << std::string(tagName) << std::endl;
	if(!m_usedTagNames[std::string(tagName)]) return notFound;
	while(m_nodeQueue.back()->getTagName()!=tagName){
		--m_usedTagNames[m_nodeQueue.back()->getTagName()];
		m_nodeQueue.pop_back();
	}--m_usedTagNames[m_nodeQueue.back()->getTagName()];
	m_nodeQueue.pop_back();
	return found;
}
size_t htmlParser::addAttributes(size_t next){
	while (m_data[next] != '>') {
		next = find_first_if(m_data, next + 1, isLetter, isNumber, [](char let) {return let == '>'; });
		if (m_data[next] != '>') {//attribute
			const size_t endOfAttributeName = find_first_if_not(m_data, next + 1, isLetter, isNumber);//[next,endOfAttributeName) = attributeName
			const size_t startOfAttributeData = m_data.find_first_of("\"\'", endOfAttributeName + 1);
			size_t endOfAttributeData = m_data.find(m_data[startOfAttributeData], startOfAttributeData + 1);
			while (m_data[endOfAttributeData - 1] == '\\')
				endOfAttributeData = m_data.find(m_data[startOfAttributeData], endOfAttributeData + 1);
			m_nodeQueue.back()->setAttribute(std::string(&m_data[next], endOfAttributeName - next), std::string(&m_data[startOfAttributeData + 1], endOfAttributeData - startOfAttributeData - 1));
			next = endOfAttributeData + 1;
		}
	}return next;
}

size_t htmlParser::foundTextNode(size_t current){
	const size_t next = m_data.find_first_of("< ", current+1);
	m_nodeQueue.back()->data += std::string(&m_data[current], next - current);
	return next;
}

bool htmlParser::isImmediatelyEnddedTag(const std::string& name) const{
	static std::vector<std::string> names = {"hr","input"};
	for(int i = 0;i<names.size();++i){
		if (names[i] == name) return true;
	}return false;
}

size_t htmlParser::foundNewNode(size_t current){
	size_t next = m_data.find_first_of(" >/\n\t", current + 1);
	addNode(std::string_view(&m_data[current + 1], next - current - 1));
	if (m_data[next] == '/')
		next = m_data.find('>', next + 1);
	next = addAttributes(next);
	if (m_data[next - 1] == '/', isImmediatelyEnddedTag(m_nodeQueue.back()->getTagName()))
		removeTillTagName(m_nodeQueue.back()->getTagName());
	return next + 1;

}

size_t htmlParser::foundEndTag(size_t current){
	const size_t next = m_data.find('>', current + 1);
	removeTillTagName(std::string_view(&m_data[current + 2], next - current - 2));
	return next + 1;
}

