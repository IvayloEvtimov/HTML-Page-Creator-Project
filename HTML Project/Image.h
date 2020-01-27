#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include "Tags.h"

class Image :public Tags
{
public:
	Image(const char*, const char*);

	virtual void print() const;

	virtual void saveHTML(std::ostream&) const;

	virtual const char* returnType() const;

	virtual Tags* clone();
};

#endif // !IMAGE_H_INCLUDED
