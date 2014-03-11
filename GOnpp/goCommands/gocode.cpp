#include <sstream>

#include "CommandExec.h"
#include "tokens.h"

#include "gocode.h"


gocode::gocode(const tstring &exe_path)
	: _exe_path(exe_path)
{
}

bool gocode::run_for(tstring& go_file, int offset)
{
        std::basic_ostringstream<TCHAR> cmdline;
	//FIXME: quote/escape paths below as appropriate
        cmdline
		<< _exe_path << _T(" ")
		<< _T("-f=\"csv\"") << _T(" ")
		<< _T("-in=\"") << go_file << _T("\" ")
		<< _T("autocomplete") << _T(" ")
		<< offset;

	// FIXME: don't use C:\ below, but find out some real, safe, existing directory (not all computers have C:\)
	CommandExec exec(cmdline.str(), _T("C:\\"));

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
