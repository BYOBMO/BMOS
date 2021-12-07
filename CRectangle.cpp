#include "CRectangle.h"



CRectangle::CRectangle()
{
}


CRectangle::~CRectangle()
{
}


bool CRectangle::intersects(CRectangle r)
{
	if ((x >= r.x + r.w) || (x+w) <= r.x ||
		y >= (r.y+r.h) || (y+h) <= r.y) 
	{

		// No intersection
		return(false);
	}

	return(true);

}