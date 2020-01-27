#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

void start(); //Starts the main program
bool checkFileExtension(const char*, const char*); //Checks the file extension
void inputFirst(char*); //Gets the first input needed to know the command 
void inputRest(const char* ,char*, char*, char*, char*, int&,char*); //Gets the rest of the input needed to complete the command
bool getCharUntil(const char*, char*, const char&, int&); //Fills a char array until a given delim char is met
bool findPhrase(const char*, const char*, int&); //Look for a given phrase in a char array

#endif // !HELPER_H_INCLUDED
