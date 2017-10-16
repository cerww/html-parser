#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class htmlNode
{
public:
	htmlNode(std::string);
	htmlNode*				 addChild(std::unique_ptr<htmlNode>&&);
	std::string_view		 getID()const;
	std::vector<std::string> getClasses()const;
	bool				     hasClass(std::string)const;
	void					 addClass();
	const std::string&	     getTagName()const { return m_tagName; };
	std::string			     data = "";
	void				     setAttribute(const std::string&,std::string);
	void				     removeAttribute(std::string_view);
	std::string_view	     getAttribute(const std::string&);
	bool				     hasAttribute(const std::string&)const;
	size_t				     numChildren()const; 
	htmlNode*			     getElementByID(std::string id);
	std::vector<htmlNode*>   getElementsByClassName(std::string name);
	std::vector<htmlNode*>   getElementsByTagName(std::string name);
	std::vector<htmlNode*>   getChildren();
	htmlNode*				 getParent();
	//template<typename string_t>
	bool					 matchesSelector(const std::string&)const;
	//template<typename string_t>
	std::vector<htmlNode*>   querySelectorAll(const std::string&);
	//template<typename string_t>
	htmlNode*				 querySelector(const std::string&);
private:
	std::vector<std::unique_ptr<htmlNode>> m_children = {};
	std::unordered_map<std::string, std::string> m_attributes;
	const std::string m_tagName;
	static std::unique_ptr<htmlNode> s_notFoundNode;
	htmlNode* m_parent;
};


