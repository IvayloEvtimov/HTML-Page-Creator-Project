#ifndef HEADING_H_INCLUDED
#define HEADING_H_INCLUDED

#include "Tags.h"

class Heading : public Tags
{
public:
	Heading(const char*, const char*);

	virtual void print() const;

	virtual void saveHTML(std::ostream&) const;

	virtual const char* returnType() const;

	virtual Tags* clone();
private:

};

#endif // !HEADING_H_INCLUDED
