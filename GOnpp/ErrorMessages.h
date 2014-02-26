#ifndef ERRRORMESSAGES_H_INCLUDED
#define ERRRORMESSAGES_H_INCLUDED


#define MSG_ERROR_HEAD _T("Error")
#define CONFIGURATION_ERROR _T("Configuration error")

#define NO_GO_EXE_ERROR _T("I couldn't find go.exe, which is necessary to run go commands!\nYou have neither the GOROOT nor the GOBIN environmement variables set and go.exe could not be found on your PATH environment variable either.\n\nPlease make sure you have GO installed on your computer and followed the steps described on: http://golang.org/doc/install. Thanks!")
#define CMD_CREATION_FAILED _T("failed to create commandline.")
#define OUT_OF_MEMORY _T("Out of memory, fuck!")

#define CONFIGFILE_ERROR _T("Error reading/writing configuration file.")

#endif