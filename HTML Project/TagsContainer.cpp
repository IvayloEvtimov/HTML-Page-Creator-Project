#include "TagsContainer.h"
#include "Constant.h"
#include "Helper.h"

#include <iostream>

TagsContainer::TagsContainer():size(0),maxSize(10),tags(nullptr)
{
	tags = new Tags*[maxSize];
}

TagsContainer::~TagsContainer()
{
	clean(size);
}

void TagsContainer::newElem(const char *type, const char *description, const char *content, const char *linkText)
{
	if (tagExist(description) >= 0) //Every tag must have unique description
		throw std::invalid_argument("Tag with that description already exists");

	if (size == maxSize) //Check for size
		resize();

	if (strcmp(type, "heading") == 0)
		tags[size++] = new Heading(description,content);
	if (strcmp(type, "image") == 0)
		tags[size++] = new Image(description, content);
	if (strcmp(type, "link")==0)
		tags[size++] = new Link(description, content, linkText);
	if (strcmp(type, "text") == 0)
		tags[size++] = new Text(description, content);
	if (strcmp(type, "video") == 0)
		tags[size++] = new Video(description, content);
	if (strcmp(type, "div") == 0)
		tags[size++] = new Div(description);
}

void TagsContainer::remove(const int &toRemove)
{
	if (toRemove >= size || toRemove < 0)
		throw std::invalid_argument("Invalid position");

	sort(toRemove);
}

void TagsContainer::moveTo(const int &toMove, const int &toFindPos)
{	//Moves a tag to a set position
	if (toFindPos > toMove || toFindPos<0) //If the tags current position is bigger then the new position
		moveHighToLow( toMove, toFindPos);
	else
		moveLowToHigh(toFindPos, toMove);
}

void TagsContainer::print(const int& toPrint) const
{
	if (toPrint >= size ||toPrint<0) //If the position set to be printed is not in the range of the container
		throw std::invalid_argument("Invalid position");

	std::cout << "Position: " << toPrint;
	tags[toPrint]->print();
}

void TagsContainer::destroyOldContent()
{	
	clean(size);
}

void TagsContainer::load(const char *buffer)
{
	int count = 0, //Used for buffer var
		countType = 0; //Used for findType var
	char findType[MAX_SIZE];

	char description[MAX_SIZE],
		content[MAX_SIZE],
		text[MAX_SIZE];

	char type = 0;
	char tempCh = 0;
	do
	{	//Skips the spaces and tabs in the buffer before the actual content
		tempCh = buffer[count++];
		if (tempCh == '\n' || tempCh == 0) //If newline or terminating char are met in the buffer 
		{								  //then there must an error
			clean(size);
			throw std::invalid_argument("File not valid");
		}
	} while (tempCh == ' ' || tempCh == '\t');

	findType[countType++] = tempCh;

	while (true)
	{	//Find what type of tag is contained in the buffer
		findType[countType++] = buffer[count++]; 
		findType[countType] = 0;

		if (strcmp(findType, "<h2 ") == 0)
		{
			type = 'h'; //Heading
			break;
		}
		if (strcmp(findType, "<p ") == 0)
		{
			type = 't'; //Text
			break;
		}

		if (strcmp(findType, "<img ") == 0)
		{
			type = 'i'; //Image
			break;
		}

		if (strcmp(findType, "<iframe ") == 0)
		{
			type = 'v'; //Video
			break;
		}

		if (strcmp(findType, "<a href=\"") == 0)
		{
			type = 'l'; //Link
			break;
		}

		if (buffer[count-1] == '\n' || buffer[count-1] == 0) //If newline or terminating char are met in the buffer
		{													//then there must an error
			//clean(size);
			throw std::invalid_argument("File not valid");
		}
	}

	if (type == 'h' || type == 't') //Heading or Text
	{
		if (!findPhrase(buffer, "descr=\"", count)) //Look for description in the buffer
			throw std::invalid_argument("File not valid");
		if(!getCharUntil(buffer, description, '\"', count)) //Gets the tags description in the buffer
			throw std::invalid_argument("File not valid");
		if(!getCharUntil(buffer, content, '<', count)) //Gets the tags content in the buffer
			throw std::invalid_argument("File not valid");

		if (type == 'h') //Create a new tag object
			newElem("heading", description, content);
		else if (type == 't')
			newElem("text", description, content);
	}

	if (type == 'i')
	{
		if (!findPhrase(buffer, "src=\"", count)) //Look for the image url in the buffer
			throw std::invalid_argument("File not valid");
		if(!getCharUntil(buffer, content, '\"', count)) //Gets the tag image url in the buffer 
			throw std::invalid_argument("File not valid");
		if (!findPhrase(buffer, "descr=\"", count)) //Look for the descriptions in the buffer
			throw std::invalid_argument("File not valid");
		if(!getCharUntil(buffer, description, '\"', count)) //Get the tag description from the buffer
			throw std::invalid_argument("File not valid");

		newElem("image", description, content); //Create a new image object
	}

	if (type == 'v')
	{
		if (!findPhrase(buffer, "src=\"", count)) //Look for the video url in the buffer
			throw std::invalid_argument("File not valid");
		if(!getCharUntil(buffer, content, '\"', count))  //Gets the tag video url from the buffer 
			throw std::invalid_argument("File not valid");
		if (!findPhrase(buffer, "descr=\"", count)) //Look for the descriptions in the buffer
			throw std::invalid_argument("File not valid");
		if(!getCharUntil(buffer, description, '\"', count)) //Get the tag description from the buffer
			throw std::invalid_argument("File not valid");

		newElem("video", description, content, nullptr); //Create a new video object
	}

	if (type == 'l')
	{
		if(!getCharUntil(buffer, content, '\"', count))  //Gets the tag link url from the buffer
			throw std::invalid_argument("File not valid");
		if(!findPhrase(buffer, "descr=\"", count)) // //Look for the descriptions in the buffer
			throw std::invalid_argument("File not valid");
		if(!getCharUntil(buffer, description, '\"', count)) //Get the tag description from the buffer
			throw std::invalid_argument("File not valid");
		if(!getCharUntil(buffer, text, '<', count)) //Get the tags content from the buffer
			throw std::invalid_argument("File not valid");

		newElem("link", description, content, text); //Create a new Link object
	}
}

void TagsContainer::save(std::ostream& output,const int& toSave) const
{
		tags[toSave]->saveHTML(output); //Save everything from the container to the HTML file
}

int TagsContainer::tagExist(const char *description) const
{	//Look if a tag with the diven description exist
	for (int count = 0; count < size; ++count) 
		if (tags[count]->checkDescr(description))
			return count;
	return -1;
}

int TagsContainer::getSize() const
{
	return size; //Return the number of tags in the container
}

const char * TagsContainer::getDescr(const int &toGet) const
{
	if (toGet >= size)
		throw std::invalid_argument("Position is too big");
	return tags[toGet]->getDescr(); //Returns the description of a tag based on its position
}

const char * TagsContainer::returnType(const int &toGet) const
{
	if (toGet >= size)
		throw std::invalid_argument("Position is too big");
	return tags[toGet]->returnType(); //Returns which type of tag based on its position
}

void TagsContainer::clean(const int &num) //Clear the container from all of its data
{
	for (int count = 0; count < num; ++count)
		delete tags[count];
	delete[] tags;

	tags = nullptr;
	size = 0;
	maxSize = 0;
}

void TagsContainer::resize() //Resizes the container by increasing its maximal size by 2
{
	if (maxSize == 0)
	{
		tags = new Tags*[maxSize + 10];
		maxSize = 10;
	}
	else
	{
		Tags** temp = new Tags*[maxSize * 2]; //Creates a new temp pointer
		for (int count = 0; count < size; ++count)
			temp[count] = tags[count]; //Sets the new pointer to point to the old data

		delete[] tags;
		tags = temp;
		maxSize *= 2;
	}
}

void TagsContainer::sort(const int &toRemove) //
{
	delete tags[toRemove];
	for (int count = toRemove; count < size-1; ++count)
	{	//Swap the tags until the one set to be deleted is at the end of the container
		Tags* temp = tags[count];
		tags[count] = tags[count - 1];
		tags[count - 1] = temp;
	}
	size -= 1;
}

//Maybe a mistake in sorting
void TagsContainer::moveLowToHigh(const int &startPos, const int &endPos)
{
	for (int count = startPos; count < endPos; ++count)
	{
		Tags* temp = tags[count];
		tags[count] = tags[count + 1];
		tags[count + 1]=temp;
	}
}

void TagsContainer::moveHighToLow(const int &startPos, const int &endPos)
{
	for (int count = startPos; count>endPos; --count)
	{
		Tags* temp = tags[count];
		tags[count] = tags[count - 1];
		tags[count - 1] = temp;
	}
}
