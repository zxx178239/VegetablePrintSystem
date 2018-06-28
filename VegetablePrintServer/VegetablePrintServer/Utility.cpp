#include "Utility.h"


vector<string> split(char *str, char ch)
{
	vector<string> sVec;
	for (int i = 0; i < strlen(str);)
	{
		string s;
		int j = 0;
		for (j = i; j < strlen(str) && str[j] != ch; ++ j)
		{
			s = s + str[j];
		}
		i = j + 1;
		
		sVec.push_back(s);
	}
	return sVec;
}