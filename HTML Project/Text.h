#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "Tags.h"

class Text :public Tags
{
public:
	Text(const char*, const char*);

	virtual void print() const;

	virtual void saveHTML(std::ostream&) const;

	virtual const char* returnType() const;

	virtual Tags* clone();

};
#endif // !TEXT_H_INCLUDED
