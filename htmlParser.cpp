#include "htmlParser.h"
#include "randomThings.h"

htmlParser::htmlParser(std::string stuffs):m_data(stuffs+"     "){}

htmlTree htmlParser::getTree(){
	htmlTree retVal;
	m_nodeQueue.push_back(retVal.root.get());
	size_t current = 0;
	while (current < m_data.size() - 5){
		if(m_data[current]=='<'){
			if(m_data[current+1] =='/'){//end tag
				const size_t next = m_data.find('>', current + 1);
				removeTillTagName(std::string_view(&m_data[current + 2], next - current - 2));
				current = next + 1;
			}else if (isLetter(m_data[current + 1])) {
				size_t next = m_data.find_first_of(" >", current + 1);
				addNode(std::string_view(&m_data[current + 1], next - current - 1));
				//add attributes next
				while (m_data[next] != '>'){
					next = find_first_if(m_data, next + 1, isLetter, isNumber, [](char let) {return let == '>'; });
					if (m_data[next] != '>') {//attribute
						size_t endOfAttributeName = find_first_if_not(m_data, next + 1, isLetter, isNumber);//[next,endOfAttributeName) = attributeName
						size_t startOfAttributeData = m_data.find_first_of("\"\'", endOfAttributeName + 1);
						size_t endOfAttributeData = m_data.find(m_data[startOfAttributeData], startOfAttributeData + 1);
						m_nodeQueue.back()->setAttribute(std::string(&m_data[next], endOfAttributeData - next), std::string(&m_data[startOfAttributeData + 1], endOfAttributeData - startOfAttributeData - 1));
						next = endOfAttributeData + 1;
					}
				}current = next + 1;
			}
		}else {//text node
			const size_t next = m_data.find('<', current);
			m_nodeQueue.back()->data += std::string(&m_data[current], next - current);
			current = next + 1;
		}
	}m_nodeQueue.clear();
	return retVal;
}

void htmlParser::addNode(std::string_view tagName){//new node's parent is the last node
	m_nodeQueue.back()->data += "%{" + std::to_string(m_nodeQueue.back()->numChildren()) + "}";
	htmlNode* newNode = new htmlNode(std::string(tagName));
	m_nodeQueue.back()->addChild(std::unique_ptr<htmlNode>(newNode));
	m_nodeQueue.push_back(newNode);
	++m_usedTagNames[m_nodeQueue.back()->getTagName()];
}

bool htmlParser::removeTillTagName(std::string_view tagName){
	if(!m_usedTagNames[std::string(tagName)]) return notFound;
	while(m_nodeQueue.back()->getTagName()!=tagName){
		--m_usedTagNames[m_nodeQueue.back()->getTagName()];
		m_nodeQueue.pop_back();
	}--m_usedTagNames[m_nodeQueue.back()->getTagName()];
	m_nodeQueue.pop_back();
	return found;
}

