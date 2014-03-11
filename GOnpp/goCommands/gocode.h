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
        gocode(const tstring& exe_path);

        bool run_for(tstring& go_file, int offset);

        DWORD _exitStatus;

        void get_completions(vector<completion>& completions);

protected:
        tstring _exe_path;

        tstring _rawStdOut;
        tstring _rawStdErr;
};

