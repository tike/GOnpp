#include <sstream>

#include "CommandExec.h"
#include "tokens.h"

#include "gocode.h"


gocode::gocode()
{
        _execpath = tstring(_T("gocode.exe"));
        _flags = tstring(_T("-f=\"csv\""));
}

void gocode::init(tstring& file, int offset)
{
        std::basic_ostringstream<TCHAR> buf;
        buf
		<< _execpath << _T(" ")
		<< _flags << _T(" ")
		<< _T("-in=\"") << file << _T("\" ")
		<< _T("autocomplete") << _T(" ")
		<< offset;
        _cmdline = buf.str();
}

bool gocode::run()
{
        CommandExec exec(_cmdline, tstring(_T("C:\\")));

        if ( ! exec.Start()) {
		return false;
	}
	if ( ! exec.Wait()) {
		return false;
	}
	if ( ! exec.ReadOutput()) {
		return false;
	}

        _exitStatus = exec.ExitStatus();
	_rawStdOut = exec.GetStdOut();
	_rawStdErr = exec.GetStdErr();
        return true;
}

void gocode::get_completions(vector<completion>& completions)
{
	for (Tokens line(_rawStdOut, _T("\n")); line.next(); ) {
	        vector<tstring> parts;
		for (Tokens part(line.get(), _T(",,")); part.next(); ) {
			parts.push_back(part.get());
		}
		if (parts.size() != 3) {
			continue;
		}

		completion c;
		c.type = parts[0];
		c.name = parts[1];
		c.signature = parts[2];
		completions.push_back(c);
	}
}
