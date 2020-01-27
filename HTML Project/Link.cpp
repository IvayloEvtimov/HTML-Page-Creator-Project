#include "Link.h"

#include <iostream>

Link::Link(const char *description, const char *content, const char *linkText):Tags(description,content),linkText(nullptr)
{
	try
	{
		if (linkText == nullptr)
		{
			this->linkText = new char[1];
			this->linkText[0] = 0;
		}
		else
		{
			this->linkText = new char[strlen(linkText) + 1];
			strcpy(this->linkText, linkText);
		}
	}
	catch (std::bad_alloc& ba)
	{
		delete[] content;
		delete[] description;
		delete[] linkText;
		throw;
	}
}

Link::Link(const Link &other):Tags(other),linkText(nullptr)
{
	try
	{
		this->linkText = new char[strlen(other.linkText) + 1];
		strcpy(this->linkText, other.linkText);
	}
	catch (std::bad_alloc& ba)
	{
		delete[] description;
		delete[] content;
		delete[] linkText;
		throw;
	}
}

Link & Link::operator=(const Link &other)
{
	if (this != &other)
	{
		Tags::operator=(other);
		delete[] linkText;
		try
		{
			this->linkText = new char[strlen(other.linkText) + 1];
			strcpy(this->linkText, other.linkText);
		}
		catch (std::bad_alloc& ba)
		{
			delete[] description;
			delete[] content;
			delete[] linkText;
			throw;
		}
	}
	return *this;
}

Link::~Link()
{
	delete[] linkText;
}

void Link::print() const
{
	std::cout << " Type: Link" << std::endl;
	std::cout << "Description: " << description << std::endl;
	std::cout << "Content: " << content << std::endl;
	std::cout << "Link text: " << linkText << std::endl;
}

void Link::saveHTML(std::ostream &output) const
{	//The syntaxis for the HTML tag for link
	output << "<a href=" << '\"' << content << '\"';
	output << " descr=" << '\"' << description << '\"';
	output << "> " << linkText << "</a>" << std::endl;
	output << "<br>" << std::endl;
}

const char * Link::returnType() const
{
	return "link";
}

Tags * Link::clone()
{
	return new Link(*this);
}
