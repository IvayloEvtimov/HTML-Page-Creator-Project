#ifndef VIDEO_H_INCLUDED
#define VIDEO_H_INCLUDED

#include "Tags.h"

class Video : public Tags
{
public:
	Video(const char*, const char*);

	virtual void print() const;

	virtual void saveHTML(std::ostream&) const;

	virtual const char* returnType() const;

	virtual Tags* clone();
};

#endif // !VIDEO_H_INCLUDED
