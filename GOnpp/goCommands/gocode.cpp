#include <sstream>

#include "CommandExec.h"

#include "gocode.h"


gocode::gocode(void)
{
        _execpath = tstring(_T("gocode.exe"));
        _flags = tstring(_T("-f=\"csv\""));
}

void gocode::init(tstring& file, int offset){
        std::basic_ostringstream<TCHAR> buf;
        buf
		<< _execpath << _T(" ")
		<< _flags << _T(" ")
		<< _T("-in=\"") << file << _T("\" ")
		<< _T("autocomplete") << _T(" ")
		<< offset;
        _cmdline = buf.str();
}

bool gocode::Run(void){
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

void gocode::splitString(tstring& in, vector<tstring>& into, TCHAR sep, int maxitems){
      std::basic_istringstream<TCHAR> outSS(in);

      while ( ! outSS.eof()){
        tstring tmp_line;
        getline(outSS, tmp_line, sep);
        into.push_back(tmp_line);
        if ((maxitems > 0 ) && (into.size() == maxitems) ){
                break;
        }
      }
}


bool gocode::getCompletions(vector<completion>& completions){
      vector<tstring> lines;
      splitString(_rawStdOut, lines, _T('\n'), 0);

      for (vector<tstring>::iterator i = lines.begin(); i!= lines.end(); ++i){
        vector<tstring> parts;
        splitString(*i, parts, _T(','), 5);
        if (parts.size() != 5){
                continue;
        }

        completion this_compl;
        this_compl.type = parts[0];
        this_compl.something = parts[1];
        this_compl.name = parts[2];
        this_compl.otherthing = parts[3];
        this_compl.signature = parts[4];
        completions.push_back(this_compl);
      }

     return true;
}
