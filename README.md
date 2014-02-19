GOnpp
=====
A go plugin for Notepad++
-------------------------
This plugin allows you to run some features of the go command
from within notepad++. Currently the following subcommands are
supported:
* go fmt
* go test
* go install
* go run

All open files are being saved upon running a go cmd. Any output
they produce is reported in the dockable Dialog, so you don't have
to switch windows anymore while debugging your build errors.

As a plus it sets your GOPATH from the current .go file, if you
don't have the GOPATH environment Variable set.

Plans for Version 1.x
---------------------
* limit autosave and reload to .go files.
* Make behaviour configurable by the user.
* Offer automatic go fmt upon saving of .go files.
* Offer running produced executables (after install).
* Refine the GOPATH checking to allow configurable appending or replacing.
* Allow setting of GOOS and GOARCH from within Notepad++
