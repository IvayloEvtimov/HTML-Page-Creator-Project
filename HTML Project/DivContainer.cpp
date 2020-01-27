#include "DivContainer.h"

#include <iostream>

DivContainer::DivContainer():descriptions(nullptr),size(0),maxSize(5),tags(nullptr)
{
	try
	{
		descriptions = new char*[maxSize];
		tags = new TagsContainer*[maxSize];
	}
	catch (std::bad_alloc& ba)
	{
		delete[] descriptions;
		delete[] tags;
		throw;
	}
}

DivContainer::~DivContainer()
{
	clean(size);
}

void DivContainer::newDiv(const char *divDescr)
{
	if (size == maxSize)
		resize();

	if (tagExist(divDescr) >= 0) //Check for existing div
		throw std::invalid_argument("A div tag with that Description already exist");
	//Copy the decsr of the div tag and create a new container 
	descriptions[size] = new char[strlen(divDescr) + 1];
	strcpy(descriptions[size], divDescr);
	tags[size++] = new TagsContainer();
}

void DivContainer::newElem(const char *divDescr, const char *tagType, const char *tagDescr, const char *tagCont, const char *tagLink)
{	//Check for size
	if (size == maxSize)
		resize();
	//Check for existing div tag
	int tagPos = tagExist(divDescr);
	if (tagPos < 0)
		throw std::invalid_argument("There isn't a div tag with that description");
	else  //And add it to that container 
		tags[tagPos]->newElem(tagType, tagDescr, tagCont, tagLink);
}

void DivContainer::load(const char *buffer, const char *divDescr)
{	//Check for existing div tag
	int tagPos = tagExist(divDescr);
	if (tagPos < 0)
		throw std::invalid_argument("There is not such div tag");
	tags[tagPos]->load(buffer); //Start loading from stream to that container
}

void DivContainer::save(std::ostream &output, const char *divDescr) const
{	//Check for existing div tag
	int tagPos = tagExist(divDescr);
	if(tagPos<0)
		throw std::invalid_argument("There isn't a div tag with that description");

	for (int count = 0; count < tags[tagPos]->getSize(); ++count)
		tags[tagPos]->save(output, count); //Save every one of the elements of that container

	output << "</div>" << std::endl;
}

void DivContainer::print(const char *divDescr) const
{
	int tagPos = tagExist(divDescr);
	if (tagPos < 0)
		throw std::invalid_argument("There is no such div with that description");

	std::cout << "Begining of Div tag with description: " << divDescr << std::endl;
	std::cout << std::endl;
	for (int count = 0; count < tags[tagPos]->getSize(); ++count)
	{
		tags[tagPos]->print(count);	//Print every one of the elements of that container
		std::cout << std::endl;
	}
	//std::cout<<std::endl;
	std::cout << "End of Div tag with description: " << divDescr << std::endl;
	std::cout << std::endl;
}

void DivContainer::destroyOldContent()
{
	clean(size);
}

void DivContainer::moveTo(const int & divPos, const char *tagDescr, const int &toMove)
{

	int tagPos = tags[divPos]->tagExist(tagDescr);
	if (tagPos < 0)
		throw std::invalid_argument("There isn't a tag with that description in this div");
	
	if (toMove >= tags[divPos]->getSize()) //If the new position is bigger then the current size of the container 
		throw std::invalid_argument("Invalid position");

	tags[divPos]->moveTo(tagPos, toMove);
}

void DivContainer::removeTag(const char *tagDescr, const int &toRemoveDivPos)
{
	int tagPos = tags[toRemoveDivPos]->tagExist(tagDescr);
	if(tagPos<0 || tagPos!=toRemoveDivPos)
		throw std::invalid_argument("There isn't a tag with that description in this div");
	tags[toRemoveDivPos]->remove(tagPos);	//Removes it
}

void DivContainer::removeDiv(const char *divDescr)
{
	int tagPos = tagExist(divDescr);
	if(tagPos<0)
		throw std::invalid_argument("There is no such div tag");
	
	sort(tagPos);
}

int DivContainer::tagExist(const char *divDescr) const
{
	for (int count = 0; count < size; ++count)
		if (strcmp(descriptions[count], divDescr) == 0)
			return count;

	return -1; //If there isn't such tag
}

void DivContainer::clean(const int &num)
{
	for (int count = 0; count < num; ++count)
	{
		delete[] descriptions[count];
		delete tags[count];
	}
	delete[] descriptions;
	delete[] tags;
	
	descriptions = nullptr;
	tags = nullptr;

	size = 0;
	maxSize = 0;
}

void DivContainer::resize()
{
	if (maxSize == 0)
	{
		descriptions = new char*[maxSize + 5];
		tags = new TagsContainer*[maxSize + 5];
		maxSize += 5;
	}
	else
	{
		char** tempChar = new char*[maxSize * 2];
		TagsContainer** tempTag = new TagsContainer*[maxSize * 2];

		for (int count = 0; count < size; ++count)
		{
			tempChar[count] = descriptions[count];
			tempTag[count] = tags[count];
		}

		delete[] descriptions;
		delete[] tags;

		descriptions = tempChar;
		tags = tempTag;
		maxSize *= 2;
	}
}

void DivContainer::sort(const int &toSort)
{
	for (int count = toSort; count < size - 1; ++count)
	{
		char* tempDescr = descriptions[count];
		descriptions[count] = descriptions[count + 1];
		descriptions[count + 1] = tempDescr;

		TagsContainer* tempTag = tags[count];
		tags[count] = tags[count + 1];
		tags[count + 1] = tempTag;
	}

	delete[] descriptions[size];
	delete tags[size];
	size -= 1;
}
