#pragma once

#include <vector>

#include "../tstring.h"


using namespace std;


struct completion {
        tstring type;
        tstring name;
        tstring signature;
};


class gocode
{
public:
        gocode();

        void init(tstring& file, int offset);
        bool run();

        DWORD _exitStatus;

        void get_completions(vector<completion>& completions);

protected:
        void split_string(const tstring& in, vector<tstring>& into, const tstring& sep);

        tstring _execpath;
        tstring _flags;
        tstring _filename;
        int _offset;

        tstring _cmdline;

        tstring _rawStdOut;
        tstring _rawStdErr;
};

