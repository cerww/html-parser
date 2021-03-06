#include "htmlNode.h"
#include "randomThings.h"
using namespace std::string_literals;
std::unique_ptr<htmlNode> htmlNode::s_notFoundNode = std::make_unique<htmlNode>("elementNotFound");

htmlNode::htmlNode(std::string t_tagName):m_tagName(t_tagName){
	m_attributes["id"] = "";
	m_attributes["class"] = "";
}

htmlNode* htmlNode::addChild(std::unique_ptr<htmlNode>&& newNode){
	newNode->m_parent = this;
	return m_children.emplace_back(std::move(newNode)).get();
}

std::string_view htmlNode::getID() const{
	const auto it = m_attributes.find("id");
	return std::string_view(it->second.c_str(), it->second.size());
}

std::vector<std::string> htmlNode::getClasses() const{
	const auto it = m_attributes.find("class");
	return split(it->second, ' ');
}

bool htmlNode::hasClass(std::string className) const{
	const auto& names = m_attributes.at("class");
	return names.find(className) != std::string::npos;
}

void htmlNode::setAttribute(const std::string & name, std::string val){
	m_attributes[name] = val;
}

void htmlNode::removeAttribute(std::string_view name){
	m_attributes.erase(std::string(name));
}

std::string_view htmlNode::getAttribute(const std::string& name){
	if (const auto it = m_attributes.find(name); it!=m_attributes.end())
		return std::string_view(it->second);
	return "";
}

bool htmlNode::hasAttribute(const std::string& name)const {
	return m_attributes.find(name) != m_attributes.end();
}

size_t htmlNode::numChildren() const{
	return m_children.size();
}

std::vector<htmlNode*> htmlNode::getChildren(){
	std::vector<htmlNode*> retVal(m_children.size());
	//for(auto& child:m_children)
	for (int i = 0; i < m_children.size(); ++i)
		retVal[i] = m_children[i].get();
	return retVal;
}

htmlNode * htmlNode::getParent(){
	return m_parent;
}

htmlNode* htmlNode::getElementByID(std::string id){
	std::vector<htmlNode*> queue(1,this);
	while(queue.size()){
		std::vector<htmlNode*> next;
		for(auto& node:queue){
			if(node->getID() == id){
				return node;
			}for(auto& i:node->getChildren()){
				next.push_back(i);				
			}
		}queue = next;
	}return s_notFoundNode.get();
}

std::vector<htmlNode*> htmlNode::getElementsByClassName(std::string name){
	std::vector<htmlNode*> retVal;
	std::vector<htmlNode*> queue(1, this);
	while (queue.size()) {
		std::vector<htmlNode*> next;
		for (auto& node : queue) {
			if (node->hasClass(name)) {
				retVal.push_back(node);
			}for (auto& i : node->getChildren()) {
				next.push_back(i);				
			}
		}queue = next;
	}return retVal;
}

std::vector<htmlNode*> htmlNode::getElementsByTagName(std::string name){
	return breadthFirstSearchMultiple(this, [](htmlNode* node) {return node->getChildren(); }, [&](htmlNode* node) {return node->getID() == name; });
}

bool htmlNode::matchesSelector(const std::string& selector)const {
	const auto temp = std::string_view(&selector[1], selector.size() - 1);//everything except the first char
	return m_tagName == selector || m_attributes.at("id") == temp && selector[0]=='#' || hasClass(std::string(temp)) && selector[0]=='.';
}

//template<typename string_t>
std::vector<htmlNode*> htmlNode::querySelectorAll(const std::string& selector){
	//BFS_PAR(this, [](htmlNode* n) { return n->getChildren(); }, [&](htmlNode* node) {return node->matchesSelector(selector); });
	return breadthFirstSearchMultiple(this, [](htmlNode* n) { return n->getChildren(); }, [&](htmlNode* node) {return node->matchesSelector(selector); });
	//return BFS_Parallel(this, [](htmlNode* n) { return n->getChildren(); }, [&](htmlNode* node) {return node->matchesSelector(selector); });
}

//template<typename string_t>
htmlNode* htmlNode::querySelector(const std::string& selector){
	auto temp = breadthFirstSearch(this, [](htmlNode* n) { return n->getChildren(); }, [&](htmlNode* node) {return node->matchesSelector(selector); });
	return temp ? temp: s_notFoundNode.get();
}
