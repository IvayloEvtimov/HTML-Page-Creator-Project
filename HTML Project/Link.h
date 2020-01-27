#ifndef LINK_H_INCLUDED
#define LINK_H_INCLUDED

#include "Tags.h"

class Link :public Tags
{
public:
	Link(const char*, const char*, const char*);
	Link(const Link&);
	Link& operator=(const Link&);
	virtual ~Link();

	virtual void print() const;

	virtual void saveHTML(std::ostream&) const;

	virtual const char* returnType() const;

	virtual Tags* clone();
private:
	char* linkText;
};

#endif // !LINK_H_INCLUDED
