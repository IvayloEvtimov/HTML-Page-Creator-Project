#include "Helper.h"
#include "System.h"
#include "Constant.h"


#include <iostream>

void start()
{
	char filename[MAX_SIZE], //Used for storing the filename
		command[MAX_SIZE], //Used for storing the command
		content[MAX_SIZE], //Used for storing the content of each tag like text or image url
		description[MAX_SIZE], //Used for storing the tag description
		linkDescription[MAX_SIZE], //Used for storing the description to the link url
		divDescr[MAX_SIZE]; //Used for storing the description of the div

	int pos = 0; //Used for storing a position 

	bool exit = false; //Check if the program should exit

	bool divEncountered = false; //Check if div tag is in use

	System S1;

	while (!exit)
	{
		try
		{
			std::cout << "Enter command: " << std::endl;
			inputFirst(command); //Inputs the first part of the command

					//Commands like print,exit,deselect don't need other input
			if (strcmp(command, "print") != 0 && strcmp(command, "exit") != 0 && strcmp(command, "deselect") != 0)
				inputRest(command, description, linkDescription, content, filename, pos, divDescr);

			if (strcmp(command, "add heading") == 0) //Heading
				S1.newElem("heading", description, content, nullptr, divEncountered, divDescr);
			else if (strcmp(command, "add text") == 0) //Text
				S1.newElem("text", description, content, nullptr, divEncountered, divDescr);
			else if (strcmp(command, "add image") == 0) //Image
				S1.newElem("image", description, content, nullptr, divEncountered, divDescr);
			else if (strcmp(command, "add video") == 0) //Video
				S1.newElem("video", description, content, nullptr, divEncountered, divDescr);
			else if (strcmp(command, "add link") == 0) //Link
				S1.newElem("link", description, content, linkDescription, divEncountered, divDescr);
			else if (strcmp(command, "add div") == 0)
				S1.newElem("div", description, nullptr, nullptr);
			else if (strcmp(command, "remove") == 0) //Remove tag
				S1.remove(description, divEncountered, divDescr);
			else if (strcmp(command, "print") == 0) //Prints content
				S1.print(divEncountered, divDescr);
			else if (strcmp(command, "moveTo") == 0) //Moves a tag to position
				S1.moveTo(pos, description, divEncountered, divDescr);
			else if (strcmp(command, "save") == 0) //Save
				S1.save(filename);
			else if (strcmp(command, "load") == 0) //Loads a file into the System
			{
				if (!checkFileExtension(filename, ".html")) //The files are required to have .html extension 
					throw std::invalid_argument("File does not have .html extension");
				S1.load(filename);
			}
			else if (strcmp(command, "select") == 0) //Selects a div tag 
			{
				if (!S1.divTagExist(divDescr)) //Check if a div with that description exists in the System
					throw std::invalid_argument("The tag with that description isn't a div tag");
				std::cout << "Working with div tag with description: " << divDescr << std::endl;
				divEncountered = true;
			}
			else if (strcmp(command, "deselect") == 0) //Deselects a div tag
			{										   //And continues to work with the System
				if (!divEncountered) //Error if there isn't any div tag selected
					throw std::invalid_argument("You are not working with any div tag");
				std::cout << "Deselecting div tag with description: " << divDescr << std::endl;
				divEncountered = false;
			}
			else if (strcmp(command, "exit") == 0) //Exits the program
			{
				S1.exit();
				exit = true;
			}
			else //If neither of the other ifs were triggered then the command must be invalid
				throw std::invalid_argument("Wrong command");
		}
		catch (std::invalid_argument& ia)
		{
			std::cerr << ia.what() << std::endl;
		}
		catch (std::bad_alloc& ba)
		{
			std::cerr << "Memory allocation problem" << std::endl;
		}
	}
}

bool checkFileExtension(const char* filename, const char* extension)
{
	int countFile = strlen(filename) - 1;
	int countExtn = strlen(extension) - 1;

	while (countExtn >= 0)
	{
		if (filename[countFile--] != extension[countExtn--]) //Compares the last char of each variable to see wheather the extension of valid
			return false;
	}
	return true;
}

void inputFirst(char* str)
{
	char temp[MAX_SIZE];
	int countTemp = 0;
	while (temp[countTemp - 1] != ' ' && temp[countTemp - 1] != '\n' && temp[countTemp - 1] != '\t') //Gets a single char until one the delim char are met
		std::cin.get(temp[countTemp++]);

	char lastChar = temp[countTemp - 1]; //Saves the last char with the delim char
	temp[countTemp - 1] = 0;

	if (strcmp(temp, "print") == 0 || strcmp(temp, "exit") == 0 || strcmp(temp, "deselect") == 0) //For commands like print,deselect,exit any further input is not required
	{
		if (lastChar != '\n')
		{
			char ch = 0;
			do
			{ //Checks if there is any other input after the commands
				std::cin.get(ch);
				if (ch == '\n')
					break;
				if (ch != '\n' && ch != ' ' && ch != '\t') //Spaces and tabs are considered valid input 
				{
					std::cin.ignore(MAX_SIZE, '\n');
					throw std::invalid_argument("Wrong input");
				}
			} while (ch == ' ' || ch == '\t');
		}
		strcpy(str, temp);
		return;
	}

	if (lastChar == '\n')
		throw std::invalid_argument("Wrong command");

	temp[countTemp -1] = lastChar;
	temp[countTemp] = 0;

	if (strcmp(temp, "add ") == 0) //If the command is for adding new tags
	{
		do
		{
			std::cin.get(temp[countTemp++]);
		} while (temp[countTemp - 1] != ' ' && temp[countTemp - 1] != '\n' && temp[countTemp - 1] != '\t');
	}

	if (temp[countTemp - 1] == '\n') //The last char can't be newline because more data should be added
		throw(std::invalid_argument("Incomplete input"));

	temp[countTemp - 1] = 0;
	strcpy(str, temp);
}

void inputRest(const char* command,char* description, char* linkDescription, char* content, char* filename, int& pos, char* divDescr)
{										//For heading,text,image,video
	if (strcmp(command, "add text") == 0 || strcmp(command, "add heading") == 0 || strcmp(command, "add image") == 0 || strcmp(command, "add video") == 0 || strcmp(command, "add link") == 0)
	{
		char ch = 0;
		while (true)
		{	//Ignores spaces and tabs until '<' is met
			std::cin.get(ch);
			if (ch == '<')
				break;
			if (ch == '\n') //If newline is met then the input is invalid
				throw std::invalid_argument("Input too short");
			if (ch != ' ' && ch != '\t') //If any other char is not space or tab then the input is invalid
			{
				std::cin.ignore(MAX_SIZE, '\n');
				throw std::invalid_argument("Wrong command");
			}
		}

		int countDescr = 0; //Gets the tag Description
		while (description[countDescr - 1] != '>' && description[countDescr - 1] != '\n')
			std::cin.get(description[countDescr++]);

		//If the last char is newline then the input is invalid because the content of the tag must be inputed
		if (description[countDescr - 1] == '\n')
			throw std::invalid_argument("Invalid input");

		description[countDescr - 1] = 0;

		do
		{   //Look ahead to see if there is a newline while skipping spaces and tabs
			std::cin.get(ch);
			if (ch == '\n')
				throw std::invalid_argument("Wrong command");
		} while (ch == ' ' || ch == '\t');

		int countCont = 0;
		content[countCont++] = ch;

		do
		{
			std::cin.get(content[countCont++]);
		} while (content[countCont - 1] != '\n');

		content[countCont - 1] = 0;

			//Continuation of link input
		if (strcmp(command, "add link") == 0)
		{	//For the link tag there is another input which is the description of the link url
			int countLink = 0;
			do
			{
				std::cin.get(linkDescription[countLink++]);
			} while (linkDescription[countLink - 1] != '\n');
			linkDescription[countLink - 1] = 0;
		}
		return;
	}

			//For adding div tag
	if (strcmp(command, "add div") == 0)
	{
		char ch = 0;

		while (ch != '<')
		{	//Ignores spaces and tabs until '<' is met
			std::cin.get(ch);
			if (ch == '\n') //If newline is met then the input is invalid
				throw std::invalid_argument("Input too short");
			if (ch != ' ' && ch != '\t') //If any other char is not space or tab then the input is invalid
			{
				std::cin.ignore(MAX_SIZE, '\n');
				throw std::invalid_argument("Wrong command");
			}
		}

		//For the div tag only a Description a needed
		int countDescr = 0;
		do
		{
			std::cin.get(description[countDescr++]);
			if (description[countDescr - 1] == '\n')
				throw std::invalid_argument("Wrong input");
		} while (description[countDescr - 1] != '>');
		description[countDescr - 1] = 0;

		do
		{ //Checks if there is any other input after the Descriptions
			std::cin.get(ch);
			if (ch == '\n')
				break;
			if (ch != '\n' && ch != ' ' && ch != '\t') //Spaces and tabs are considered valid input 
			{
				std::cin.ignore(MAX_SIZE, '\n');
				throw std::invalid_argument("Wrong input");
			}
		} while (ch == ' ' || ch == '\t');
		return;
	}

			//Remove post
	if (strcmp(command, "remove") == 0)
	{
		//When removing posts only their Description is needed
		int countDescr = 0;
		do
		{
			std::cin.get(description[countDescr++]);
		} while (description[countDescr - 1] != '\n');

		description[countDescr - 1] = 0;
		return;
	}

			//For moving to Pos
	if (strcmp(command, "moveTo") == 0)
	{
		std::cin >> pos; //The new move position 

		if (std::cin.fail()) //If a number is not entered then the cin flag is triggered
		{
			std::cin.clear();
			std::cin.ignore(MAX_SIZE, '\n');
			throw std::invalid_argument("Wrong input");
		}

		char tempCh = 0;
		do
		{   //Ignores spaces and tabs until something other is met 
			std::cin.get(tempCh);
			if (tempCh == '\n') //Unless it is a newline
				throw std::invalid_argument("Input too short");
		} while (tempCh == ' ' || tempCh == '\t');

		int countDescr = 0;
		description[countDescr++] = tempCh; 

		do
		{	//Gets the tag Description 
			std::cin.get(description[countDescr++]);
		} while (description[countDescr - 1] != '\n');

		description[countDescr - 1] = 0;
		return;

	}

						//For saving and loading
	if (strcmp(command, "save") == 0 || strcmp(command, "load") == 0)
	{
		char tempCh = 0;
		do
		{   //Ignores spaces and tabs until something other is met 
			std::cin.get(tempCh);
			if (tempCh == '\n') //Unless it is a newline
				throw std::invalid_argument("Input too short");
		} while (tempCh == ' ' || tempCh == '\t');

		int countFile = 0;
		filename[countFile++] = tempCh;
		do
		{	//Gets the name of the file
			std::cin.get(filename[countFile++]);
		} while (filename[countFile - 1] != '\n');

		filename[countFile - 1] = 0;
		return;
	}

		//For selecting a div tag
	if (strcmp(command, "select") == 0 )
	{
		char tempCh = 0;
		do
		{   //Ignores spaces and tabs until something other is met 
			std::cin.get(tempCh);
			if (tempCh == '\n') //Unless it is a newline
				throw std::invalid_argument("Input too short");
		} while (tempCh == ' ' || tempCh == '\t');

		int countDiv = 0;
		divDescr[countDiv++] = tempCh;
		do
		{
			std::cin.get(divDescr[countDiv++]);
		} while (divDescr[countDiv - 1] != '\n');
		divDescr[countDiv - 1] = 0;
		return;
	}

	//If the command isn't valid
	std::cin.ignore(MAX_SIZE, '\n');
	throw std::invalid_argument("Wrong command");
}

bool getCharUntil(const char *buffer, char *toLoad, const char &delim, int &count)
{
	char tempCh = 0;
	do
	{	//Skips the spaces,tabs and arrow before the actual content
		tempCh = buffer[count++];
		if (tempCh == '\n' || tempCh == 0) //If a newline or terminating char is encountered
			return false; 	//Then there must be an error
	} while (tempCh == ' ' || tempCh == '\t' || tempCh == '<' || tempCh == '>');

	int countLoad = 0;
	toLoad[countLoad++] = tempCh;

	while (true)
	{	//Starts loading from the buffer until a delim char is met
		toLoad[countLoad++] = buffer[count++];
		if (buffer[count - 1] == delim)
		{
			toLoad[countLoad - 1] = 0;
			break;
		}

		if (buffer[count - 1] == '\n' || buffer[count - 1] == 0) //If a newline or terminating char is encountered 	
			return false;										//Then there must be an error
	}
	return true;
}

bool findPhrase(const char *buffer, const char *toMatch, int &count)
{
	char tempCh = 0;
	do
	{	//Skips the spaces and tabs before the actual content
		tempCh = buffer[count++];
		if (tempCh == '\n' || tempCh == 0) //If a newline or terminating char is encountered 
		{								//Then there must be an error
			//throw std::invalid_argument("File not valid");
			return false;
		}
	} while (tempCh == ' ' || tempCh == '\t');

	char newBuffer[MAX_SIZE];
	int countLoad = 0;
	newBuffer[countLoad++] = tempCh;

	while (true)
	{	//Starts loading from the buffer until a delim phrase is met
		newBuffer[countLoad++] = buffer[count++];
		newBuffer[countLoad] = 0;

		if (strcmp(newBuffer, toMatch) == 0)
			return true;

		if (strlen(newBuffer) > strlen(toMatch))
			return false;

		if (buffer[count - 1] == '\n' || buffer[count - 1] == 0)
			return false;
	}
}
