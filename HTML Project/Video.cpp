#include "Video.h"

#include <iostream>

Video::Video(const char *description, const char *content):Tags(description,content)
{

}

void Video::print() const
{
	std::cout << " Type: Video" << std::endl;
	std::cout << "Description: " << description << std::endl;
	std::cout << "Content: " << content << std::endl;
}

void Video::saveHTML(std::ostream &output) const
{	//The syntaxis for the HTML tag for video
	output << "<iframe src=" << '\"' << content << '\"';
	output << " descr=" << '\"' << description << '\"';
	output << "></iframe>" << std::endl;
	output << "<br>" << std::endl;
}

const char * Video::returnType() const
{
	return "video";
}

Tags * Video::clone()
{
	return new Video(*this);
}
