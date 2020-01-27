#include "Text.h"

#include <iostream>

Text::Text(const char *description, const char *content):Tags(description,content)
{

}

void Text::print() const
{
	std::cout << " Type: Text" << std::endl;
	std::cout << "Description: " << description << std::endl;
	std::cout << "Content: " << content << std::endl;
}

void Text::saveHTML(std::ostream &output) const
{	//The syntaxis for the HTML tag for text
	output << "<p descr=\"" << description << "\">";
	output << content << "</p>" << std::endl;
	output << "<br>" << std::endl;
}

const char * Text::returnType() const
{
	return "text";
}

Tags * Text::clone()
{
	return new Text(*this);
}
