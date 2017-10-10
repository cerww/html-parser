#include "htmlParser.h"
#include "randomThings.h"

htmlParser::htmlParser(std::string stuffs):m_data(stuffs+"     "){}

htmlTree htmlParser::getTree(){
	htmlTree retVal;
	m_nodeQueue.push_back(retVal.root.get());
	size_t current = 0;
	while (current < m_data.size() - 5){
		//skip white space
		while(m_data[current]==' '||m_data[current] == '\n'|| m_data[current]=='\t'){
			++current;
		}if(m_data[current]=='<'){
			if(m_data[current+1] =='/'){//end tag
				const size_t next = m_data.find('>', current + 1);
				removeTillTagName(std::string_view(&m_data[current + 2], next - current - 2));
				current = next + 1;
			}else if (isLetter(m_data[current + 1])) {
				size_t next = m_data.find_first_of(" >/", current + 1);
				addNode(std::string_view(&m_data[current + 1], next - current - 1));
				if (m_data[next] == '/')next = m_data.find('>', next + 1);
				//add attributes next
				while (m_data[next] != '>'){
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
				}if(m_data[next - 1]=='/')
					removeTillTagName(m_nodeQueue.back()->getTagName());
				current = next + 1;				
			}else if (m_data[current + 1] == '!'&&m_data[current + 2] == '-'&&m_data[current + 3] == '-') {//comment
				current = m_data.find("-->", current + 1) + 3;
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
	if (tagName.back() == '/') tagName.remove_suffix(1);
	m_nodeQueue.back()->data += "%{" + std::to_string(m_nodeQueue.back()->numChildren()) + "}";
	m_nodeQueue.push_back(m_nodeQueue.back()->addChild(std::make_unique<htmlNode>(std::string(tagName))));
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

