#ifndef DIV_H_INCLUDED
#define DIV_H_INCLUDED

#include "Tags.h"

class Div :public Tags
{
public:
	Div(const char*, const char* = nullptr);

	void print() const;
	void saveHTML(std::ostream&) const;

	virtual const char* returnType() const;

	virtual Tags* clone();
};

#endif // DIV_H_INCLUDED
