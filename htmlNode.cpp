#include "htmlNode.h"
#include "randomThings.h"

htmlNode::htmlNode(std::string t_tagName):m_tagName(t_tagName){
	m_attributes["id"] = "";
	m_attributes["class"] = "";
}

void htmlNode::addChild(std::unique_ptr<htmlNode>&& newNode){
	m_children.push_back(std::move(newNode));
}

std::string_view htmlNode::getID() const{
	const auto it = m_attributes.find("id");
	return std::string_view(it->second.c_str(), it->second.size());
}

std::vector<std::string> htmlNode::getClasses() const{
	const auto it = m_attributes.find("class");
	return split(it->second, ' ');
}

void htmlNode::setAttribute(const std::string & name, std::string val){
	m_attributes[name] = val;
}

void htmlNode::removeAttribute(std::string_view name){
	m_attributes.erase(std::string(name));
}

std::string_view htmlNode::getAttribute(const std::string& name){
	if (const auto it = m_attributes.find(name);
		it!=m_attributes.end())
		return std::string_view(it->second);
	return "";
}

bool htmlNode::hasAttribute(const std::string& name)const {
	return m_attributes.find(name) != m_attributes.end();
}

size_t htmlNode::numChildren() const{
	return m_children.size();
}

std::vector<htmlNode*> htmlNode::getChildren()
{
	return std::vector<htmlNode*>();
}

htmlNode* htmlNode::getElementByID(std::string id){
	std::vector<htmlNode*> queue(1,this);
	while(queue.size()){
		std::vector<htmlNode*> next;
		for(auto& i:queue){
			
		}
	}
}

std::vector<htmlNode*> htmlNode::getElementsByClassName(std::string name)
{
	return std::vector<htmlNode*>();
}

std::vector<htmlNode*> htmlNode::getElementsByTagName(std::string name)
{
	return std::vector<htmlNode*>();
}

