#pragma once
class CRectangle
{
public:
	CRectangle();
	CRectangle(int x1, int y1, int w1, int h1) { x = x1; y = y1; w = w1; h = h1; }
	~CRectangle();
	bool intersects(CRectangle r);

	int x, y, w, h;
};

