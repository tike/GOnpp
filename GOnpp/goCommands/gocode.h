#pragma once

#include <vector>

#include "../tstring.h"


using namespace std;


struct completion {
        tstring type;
        tstring something;
        tstring name;
        tstring otherthing;
        tstring signature;
};


class gocode
{
public:
        gocode();

        void init(tstring& file, int offset);
        bool Run();

        DWORD _exitStatus;

        bool getCompletions(vector<completion>& completions);

protected:
        void splitString(tstring& in, vector<tstring>& into, TCHAR sep, int maxitems);

        tstring _execpath;
        tstring _flags;
        tstring _filename;
        int _offset;

        tstring _cmdline;

        tstring _rawStdOut;
        tstring _rawStdErr;
};

