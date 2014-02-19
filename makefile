# ##### BEGIN LICENSE BLOCK #####
# Version: MIT/X11 License
# 
# Copyright (c) 2013 Diego Casorran
# Copyright (c) 2014 Mateusz Czapliński
# 
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
# 
# Contributor(s):
#   Diego Casorran <dcasorran@gmail.com> (Original Author)
#   Mateusz Czapliński <czapkofan@gmail.com> (adjusted for GOnpp, added
#     dependencies tracking http://stackoverflow.com/a/2394668/98528)
# 
# ##### END LICENSE BLOCK #####

ARCH    =  mingw32
PROGRAM =  GOnpp
TARGET  =  $(PROGRAM).dll
CC      =  $(ARCH)-gcc
CXX     =  $(ARCH)-g++
WINDRES =  windres -O coff
CFLAGS  =  -O2 -mtune=i686 -DBUILD_DLL -W -Wall -gstabs -mwindows \
           $(foreach dir,$(SRC_DIRS),-I$(dir)) \
           -DUNICODE -D_UNICODE
LIBS    =  -lws2_32 -lz -lm -Wl,-Map,$@.map,--cref -static-libgcc
LDFLAGS =  -Wl,--out-implib,$(TARGET) -shared -lshlwapi $(CFLAGS)
OBJDIR  =  .objs

#INSTALL_PATH = "C:\\Program Files\\Notepad++\\plugins\\"

SRC_DIRS = \
	GOnpp \
	GOnpp/DockingFeature \
	GOnpp/goCommands \


SRCS = \
	$(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp)) \
	$(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c)) \

RCS = \
	$(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.rc)) \


##-- utils

ifeq ($(OS),Windows_NT)
	RM-R    = if exist $(subst /,\,$1)/nul      rmdir /q /s $(subst /,\,$1)
	RM      = if exist $(subst /,\,$1)          del $(subst /,\,$1)
	MKDIR-P = if not exist $(subst /,\,$1)\nul  mkdir $(subst /,\,$1)
else
	RM-R    = rm -rf $1
	RM      = rm -f $1
	MKDIR-P = mkdir -p $1
endif

dirname = $(patsubst %/,%,$(dir $1))

##--

OBJS = \
	$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS)) \
	$(patsubst %.rc,$(OBJDIR)/%.res,$(RCS)) \

all: depend $(TARGET)

$(OBJDIR)/%.o: %.c
	@echo Compiling $@
	@$(call MKDIR-P,$(call dirname,$@))
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.cpp
	@echo Compiling $@
	@$(call MKDIR-P,$(call dirname,$@))
	@$(CXX) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@echo Linking $@
	@$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

$(OBJDIR)/%.res: %.rc %.ico %.h
	@$(call MKDIR-P,$(call dirname,$@))
	@$(WINDRES) -o $@ $<

##-- automatic dependencies tracking

depend: .depend

.depend: $(SRCS)
	$(call RM,./.depend)
	$(CC) $(CFLAGS) -MM $^ >> ./.depend

include .depend

##--
    
#install: $(OBJDIR) $(TARGET)
#	@echo Installing $(TARGET)...
#	@cp $(TARGET) $(INSTALL_PATH)

clean:
	$(call RM-R,$(OBJDIR))
	$(call RM,./.depend)
	$(call RM,./*.dll)
	$(call RM,./*.map)
