#ifndef TAGSCONTAINER_H_INCLUDED
#define TAGSCONTAINER_H_INCLUDED

#include "Heading.h"
#include "Text.h"
#include "Video.h"
#include "Link.h"
#include "Image.h"
#include "Div.h"

#include <fstream>

class TagsContainer //A polymorphic container for Tag
{
public:
	TagsContainer();
	TagsContainer(TagsContainer&) = delete;
	TagsContainer& operator=(const TagsContainer&) = delete;
	~TagsContainer();

	void newElem(const char*, const char*, const char*, const char* = nullptr); //Adds new Element
	void remove(const int&); //Removes existing tag
	void moveTo(const int&, const int&); //Moves a tag to a different position
	void print(const int&) const; //Print the contents of the container

	void destroyOldContent(); //Clearing the container because of a new file or error in loading the current file
	void load(const char*); //Load a different file
	void save(std::ostream&,const int&) const; //Saves the current one

	int tagExist(const char*)const; //Returns the position of a Tag in the container if it exists
	int getSize()const; //Return the number of Tags in the containers
	const char* getDescr(const int&) const; //Returns the Description by giving its position
	const char* returnType(const int&) const; //Return the type of a Tag by giving its position
private:
	void clean(const int&);
	void resize();
	void sort(const int&);
	void moveLowToHigh(const int&, const int&);
	void moveHighToLow(const int&, const int&);
private:
	int size;
	int maxSize;
	Tags** tags;
};

#endif // !TAGSCONTAINER_H_INCLUDED
