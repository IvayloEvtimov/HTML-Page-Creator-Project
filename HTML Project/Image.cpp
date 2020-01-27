#include "Image.h"

#include <iostream>

Image::Image(const char *description, const char *content):Tags(description,content)
{

}

void Image::print() const
{
	std::cout << " Type: Image" << std::endl;
	std::cout << "Description: " << description << std::endl;
	std::cout << "Content: " << content << std::endl;
}

void Image::saveHTML(std::ostream &output) const
{	//The syntaxis for the HTML tag for image
	output << "<img src=" << '\"' << content << '\"';
	output << " descr=" << '\"' << description << '\"' << ">" << std::endl;
	output << "<br>" << std::endl;
}

const char * Image::returnType() const
{
	return "image";
}

Tags * Image::clone()
{
	return new Image(*this);
}
