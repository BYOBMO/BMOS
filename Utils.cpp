#include"Utils.h"

std::string RandomSize()
{
	char str[100];
	char str2[100];
	int r1, r2;

	r1 = std::rand();
	r1 = (r1 % 100) + 1;


	r2 = std::rand();
	r2 = (r2 % 3);

	switch (r2)
	{
	case 0:
		sprintf(str2, "%d KB", r1);
		break;
	case 1:
		sprintf(str2, "%d MB", r1);
		break;
	default:
		sprintf(str2, "%d GB", r1);
		break;
	}

	std::string s = str2;

	return(s);
}