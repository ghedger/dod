#Compiler and Linker
CC          := g++

#The Target Binary Program
TARGET      := dod

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := inc
BUILDDIR    := obj
TARGETDIR   := bin
RESDIR      := res
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

INSTDIR = /usr/local/bin/$(EXE)
CONFDIR = ~/$(TARGET)

#Flags, Libraries and Includes

SDL_CONFIG ?= sdl-config
SDL_CFLAGS = $(shell $(SDL_CONFIG) --cflags)
SDL_LIBS = $(shell $(SDL_CONFIG) --libs)

CFLAGS      := -Wall -c -DLINUX -O3
CFLAGS      += $(SDL_CFLAGS)

LIB 				:= $(SDL_LIBS) -lSDL_mixer -lGL -lGLU
INC         := -I$(INCDIR)
INCDEP      := -I$(INCDIR)

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Defauilt Make
all: resources $(TARGET)

#Remake
remake: cleaner all

resources: directories
		@cp -rf $(RESDIR)/* $(TARGETDIR)/

#Make the Directories
directories:
		@mkdir -p $(TARGETDIR)
		@mkdir -p $(BUILDDIR)

install: all
	@echo " o Creating install directory '$(INSTDIR)'"
	mkdir -p "$(INSTDIR)"
	chmod 777 "$(INSTDIR)"
	@echo " o Creating additional directories 'conf' and 'ftp' in '$(INSTDIR)'"
	mkdir "$(CONFDIR)/conf"
	mkdir -p "$(CONFDIR)/sound"

#Clean only Objecst
clean:
		@$(RM) -rf $(BUILDDIR)
		@$(RM) -rf $(TARGETDIR)

#Full Clean, Objects and Binaries
cleaner: clean
		@$(RM) -rf $(TARGETDIR)

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link

$(TARGET): $(OBJECTS)
		$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
		@mkdir -p $(dir $@)
		$(CC) $(CFLAGS) $(INC) -c -o $@ $<
		@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
		@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
		@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
		@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
		@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

#Non-File Targets
.PHONY: all remake clean cleaner resources

