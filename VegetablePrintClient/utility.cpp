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


char *substr(char *str, char ch)
{
    int total_length = strlen(str);

    int startLoc;
    for(startLoc = 0; startLoc < total_length; ++ startLoc)
    {
        if(str[startLoc] == ch)
            break;
    }
    ++ startLoc;
    int real_length =  total_length - startLoc + 1;
    char *tmp;
    if (NULL == (tmp=(char*) malloc(real_length * sizeof(char)))) {
        printf("Memory overflow . \n");
        exit(0);
    }
    strncpy(tmp, str + startLoc, real_length - 1);
    tmp[real_length - 1] = '\0';

    return tmp;

}
