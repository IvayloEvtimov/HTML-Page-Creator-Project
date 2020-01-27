#include "Div.h"

#include <iostream>

Div::Div(const char *description, const char *content):Tags(description,content)
{
}

void Div::print() const
{
	std::cout << " Type: Div" << std::endl;
	std::cout << "Contents of div " << description<<std::endl ;
}

void Div::saveHTML(std::ostream &output) const
{
	output << "<div descr=\"" << description << "\">"<<std::endl;
}

const char * Div::returnType() const
{
	return "div";
}

Tags * Div::clone()
{
	return new Div(*this);
}
