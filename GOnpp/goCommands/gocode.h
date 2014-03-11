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
        bool run_for(const tstring &go_file, int offset, vector<completion> &completions);
        DWORD _exitStatus;

protected:
        tstring _exe_path;
        void parse_completions(const tstring &console, vector<completion>& completions);
};

