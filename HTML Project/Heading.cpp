#include "Heading.h"

#include <iostream>

Heading::Heading(const char *description, const char *content):Tags(description,content)
{

}

void Heading::print() const
{
	std::cout << " Type: Heading" << std::endl;
	std::cout << "Description: " << description << std::endl;
	std::cout << "Content: " << content << std::endl;
}

void Heading::saveHTML(std::ostream &output) const
{
	output << "<h2 descr=\"" << description << '\"' << '>';
	output << content << "</h2>" << std::endl;
	output << "<br>" << std::endl;
}

const char * Heading::returnType() const
{
	return "heading";
}

Tags * Heading::clone()
{
	return new Heading(*this);;
}
