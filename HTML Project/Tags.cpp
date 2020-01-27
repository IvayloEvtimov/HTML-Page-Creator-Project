#include "Tags.h"

#include <iostream>

Tags::Tags(const char *description, const char *content) :description(nullptr), content(nullptr)
{
	try
	{
		if (description == nullptr)
		{
			this->description = new char[1];
			this->description[0] = 0;
		}
		else
		{
			this->description = new char[strlen(description) + 1];
			strcpy(this->description, description);
		}

		if (content == nullptr)
		{
			this->content = new char[1];
			this->content[0] = 0;
		}
		else
		{
			this->content = new char[strlen(content) + 1];
			strcpy(this->content, content);
		}
	}
	catch (std::bad_alloc& ba)
	{
		delete[] description;
		delete[] content;
		throw;
	}
}

Tags::Tags(const Tags &other):description(nullptr),content(nullptr)
{
	try
	{
		this->content = new char[strlen(other.content) + 1];
		strcpy(this->content, other.content);

		this->description = new char[strlen(other.description) + 1];
		strcpy(this->description, other.description);
	}
	catch (std::bad_alloc& ba)
	{
		delete[] description;
		delete[] content;
		throw;
	}
}

Tags & Tags::operator=(const Tags &other)
{
	if (this != &other)
	{
		delete[] description;
		delete[] content;
		try
		{
			description = new char[strlen(other.description) + 1];
			strcpy(description, other.description);

			content = new char[strlen(other.content) + 1];
			strcpy(content, other.content);
		}
		catch (std::bad_alloc& ba)
		{
			delete[] description;
			delete[] content;
			throw;
		}
	}
	return *this;
}

Tags::~Tags()
{
	delete[] description;
	delete[] content;
}

bool Tags::checkDescr(const char *description) const
{
	if (strcmp(this->description, description) == 0) //Checks if the 2 char array  are the same
		return true;
	return false;
}

const char * Tags::getDescr() const
{
	return description;
}
