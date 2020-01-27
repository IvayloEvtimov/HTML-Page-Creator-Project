#ifndef TAGS_H_INCLUDED
#define TAGS_H_INCLUDED

#include <fstream>

class Tags
{
public:
	Tags(const char*, const char*);
	Tags(const Tags&);
	Tags& operator=(const Tags&);
	virtual ~Tags();

	bool checkDescr(const char*) const; //Checks if the given Description matches with the member var description

	virtual void print() const = 0; //Print the type of the tag and its contents

	virtual void saveHTML(std::ostream&) const = 0; //Saves the tag into a file

	virtual const char* returnType() const = 0; //Return which type of tag it is
	const char* getDescr() const; //Return the value of the description member var

	virtual Tags* clone() = 0; //Clones the object
protected:
	char* description;
	char* content;
};

#endif // !TAGS_H_INCLUDED
