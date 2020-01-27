#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include "TagsContainer.h"
#include "DivContainer.h"

class System
{
public:
	System();
	System(const System&) = delete;
	System& operator=(const System&) = delete;
	~System();

	void newElem(const char*, const char*, const char*, const char*, const bool& = false, const char* = nullptr);
	void remove(const char*,  const bool& = false, const char* = nullptr);
	void moveTo(const int&, const char*, const bool& = false, const char* = nullptr);
	void print(const bool& = false, const char* = nullptr) const;
	void load(const char*);
	void save(const char*);
	void exit();

	bool divTagExist(const char*); //Checks if there is a div tag with Given description
private:
	void newFile();
	void changeFilename(const char*);
private:
	char* filename;
	TagsContainer tags;
	DivContainer divs;
	bool fileLoaded;
	bool upToDate;
};

#endif // !SYSTEM_H_INCLUDED
