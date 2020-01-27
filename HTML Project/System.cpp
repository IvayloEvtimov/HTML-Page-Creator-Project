#include "System.h"
#include "Constant.h"
#include "Helper.h"
#include <iostream>

System::System() :filename(nullptr), tags(), divs(),fileLoaded(false), upToDate(true)
{
	this->filename = new char[1];
	this->filename[0] = 0;
}

System::~System()
{
	delete[] filename;
}

//Adds new Element to a of the containers based on the div description
void System::newElem(const char* type, const char* description, const char* content, const char* linkText, const bool& divEncountered, const char* divDescr)
{	
	if (!fileLoaded)
		throw std::invalid_argument("File must first be loaded");

	//There can't be a div inside another div
	if(divEncountered && strcmp(type,"div")==0)
		throw std::invalid_argument("You can't add a div tag inside a div tag");

	if (divEncountered) //Only if the user is working with a div
		divs.newElem(divDescr, type, description, content, linkText);
	else //For everything else it adds it to the regular container
		tags.newElem(type, description, content, linkText);

	if (strcmp(type, "div") == 0) //Creates a new div container
		divs.newDiv(description);

	std::cout << "New " << type << " has been added" << std::endl;

	upToDate = false; //Remainder to save the update the file before exit
}

void System::remove(const char* description, const bool& divEncountered, const char* divDescr)
{
	if (!fileLoaded)
		throw std::invalid_argument("File must first be loaded");

	if (!divEncountered) //For everything else it removes from to the regular container  
	{
		int toRemovePos = tags.tagExist(description); //Check to see if the tag with that description exist
		if(toRemovePos<0) //If it exist it should return its position in the container 
			throw std::invalid_argument("Post doesn't exist"); //If not it returns -1
		if (strcmp(tags.getDescr(toRemovePos), "div") == 0) //If the tag is of type div
			divs.removeDiv(description); //Remove all of its contents
		tags.remove(toRemovePos); //Then remove the tag from the container
	}
	else //Only if the user is working with a div
	{
		int toRemovePos = divs.tagExist(divDescr); //Check to see if div tag with that description exist
		if(toRemovePos<0) //If it exist it should return its position in the div container 
			throw std::invalid_argument("Post doesn't exist"); //If not it returns -1
		divs.removeTag(description, toRemovePos); //Then remove the tag from the div container
	}

	std::cout << "Tag " << description << " has been removed" << std::endl;

	upToDate = false; //Remainder to save the update the file before exit
}

void System::moveTo(const int&toMove, const char* description, const bool& divEncountered, const char* divDescr)
{
	if (!fileLoaded)
		throw std::invalid_argument("File must first be loaded");

	if (divEncountered)
	{
		int toFindPos = divs.tagExist(divDescr); //Check to see if div tag with that description exist
		if (toFindPos < 0) //If it exist it should return its position in the div container 
			throw std::invalid_argument("There is no div with that description"); //If not it returns -1
		divs.moveTo(toFindPos, description, toMove); //Then move the tag from the div container to the desired position
	}
	else
	{
		if (toMove >= tags.getSize()) //If the new position is bigger then the current size of the container 
			throw std::invalid_argument("Invalid position");

		int toFindPos = tags.tagExist(description); // //Check to see if div tag with that description exist
		if (toFindPos < 0) //Check to see if div tag with that description exist
			throw std::invalid_argument("There is no such tag"); //If not it returns -1
		tags.moveTo(toMove, toFindPos); //Then move the tag from the div container to the desired position
	}

	std::cout << "Tag " << description << " has been moved to position " << toMove << std::endl;

	upToDate = false; //Remainder to save the update the file before exit
}

void System::print(const bool& divEncountered, const char* divDescr) const
{
	if (!fileLoaded)
		throw std::invalid_argument("File must first be loaded");

	if (divEncountered) //If the User is working with a div tag
		divs.print(divDescr);
	else //For normal tags
	{
		std::cout << std::endl;
		for (int count = 0; count < tags.getSize(); ++count)
		{
			tags.print(count);
			std::cout << std::endl;
			if (strcmp(tags.returnType(count), "div") == 0) //If there is a div tag inside the container
				divs.print(tags.getDescr(count)); //Prints the contents inside the div tag 
		}
	}
}

void System::load(const char *filename)
{
	if (fileLoaded) //If it is not the first load
	{
		if (strcmp(this->filename,filename)==0) //Check if the file is already loaded
			throw(std::invalid_argument("File already loaded"));
		newFile(); //Save the content of the previous file and clean up the containers
	}

	std::ifstream input(filename);

	//Quick check if the file exists
	//If it doesn't there is no need for loading
	if (!input.is_open())
	{	//Tries to create a new file
		std::cout << "File doesn't exist. Creating a new one\n";
		std::ofstream createFile(filename);
		if (!createFile.is_open()) //If the creation failed exits the loading
			std::cout << "File couldn't be created" << std::endl;
		createFile.close();
		//After the new file has been created saves its name for fture use
		changeFilename(filename);
		return;
	}

	bool divEncountered = false; //If a "<div>" line has been encountered
								//Means that a div tag is inside
	char buffer[MAX_SIZE];
	char divDescr[MAX_SIZE];

	//Read the contents before the "<body>" line
	do
	{
		input.getline(buffer, MAX_SIZE, '\n');
		if (input.eof())
			throw std::invalid_argument("File not valid");
	} while (strcmp(buffer, "<body>") != 0);	//And checks if there is a "<body>" line

	try
	{
		while (true)
		{
			int countBuffer = 0;
			input.getline(buffer, MAX_SIZE, '\n'); //Gets a line from the file

			if (strcmp(buffer, "</body>") == 0 || input.eof()) //If it reaches to "</body>" then all the tags in the file have been loaded
				break;

			if (findPhrase(buffer, "<div descr=\"", countBuffer)) //Looks if there is a div tag in the file
			{
				if (getCharUntil(buffer, divDescr, '\"', countBuffer)) //Tries to get the divs description
				{
					divEncountered = true;
					tags.newElem("div", divDescr, nullptr); //Adds the div in the regular container
					divs.newDiv(divDescr); //And adds it into the container for divs
				}
			}
			else if (strcmp(buffer, "</div>") == 0) //Signals that the div has ended
			{
				divEncountered = false;
			}
			else if (strcmp(buffer, "<br>") != 0) //Everything except <br>  
			{
				if (divEncountered)
					divs.load(buffer, divDescr);
				else
					tags.load(buffer);
			}
		}
	}
	catch (std::invalid_argument& ia)
	{	//If error is encountered when loading
		//The entire  file read is considered invalid and everything saved up to this point is deleted
		tags.destroyOldContent();
		divs.destroyOldContent();
		fileLoaded = false;
		throw;
	}

	changeFilename(filename); //After the loading has passed without error save the name of the file
	fileLoaded = true; //First loading has passed so at every other loading the contents must be saved in the file and the containers to be cleared
	upToDate = true;
	input.close(); 

	std::cout << "File loaded" << std::endl;
}

void System::save(const char *filename)
{
	if (!fileLoaded)
		throw std::invalid_argument("File must first be loaded");

	if (strcmp(this->filename,filename)!=0) //Checks if the same file is loaded into the system
		throw (std::invalid_argument("Different file loaded"));

	std::ofstream output(filename);
	if (!output.is_open())
		throw (std::logic_error("File couldn't be opened"));

	//Outputs the first part of a HTML file
	output << "<!DOCTYPE html>" << std::endl;
	output << "<html>" << std::endl;
	output << "<head></head>" << std::endl;
	output << "<body>" << std::endl;
	
	for (int count = 0; count < tags.getSize(); ++count)
	{	//Save every tag from the regular container one by one
		tags.save(output, count);
		if (strcmp(tags.returnType(count), "div") == 0) //If there are div tag in the regular container
			divs.save(output, tags.getDescr(count)); //Save every one of their items as well
	}

	//Outputs the last part of a HTML file
	output << "</body>" << std::endl;
	output << "</html>" << std::endl;

	output.close();

	upToDate = true; //Signals that every change made so far has been saved 
	std::cout << "File saved" << std::endl;
}

void System::exit()
{
	if (!upToDate) //Check if every change made is saved
		save(filename);
}

void System::newFile()
{	//When loading a new file the old one must be saved and the contents of the containers to be clear 
	std::cout << "New file loaded. Saving the contents of the previous one" << std::endl;
	if(!upToDate)
		save(filename);
	tags.destroyOldContent();
	divs.destroyOldContent();
}

bool System::divTagExist(const char *divDescr)
{	//Checks if a tag exists in the container
	int tagPos =tags.tagExist(divDescr);
	if (tagPos < 0)
		throw std::invalid_argument("There is no such tag with that description");
	if (strcmp(tags.returnType(tagPos), "div") == 0)
		return true;
	return false;
}

void System::changeFilename(const char *filename)
{
	delete[] this->filename;
	this->filename = new char[strlen(filename) + 1];
	strcpy(this->filename, filename);
}