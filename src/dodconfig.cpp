// Central class for managing user configuration files and locations
//
// Copyright (C) 2017 Greg Hedger


#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <cstdio>

#include "dodconfig.h"


using namespace std;




DODConfig::DODConfig()
{
}

const char *DODConfig::GetOptsFilePath()
{
	m_optsFilePath = GetHomePath() + USER_DIRECTORY_NAME  +  CONF_DIRECTORY_NAME + OPTS_FILE_NAME;
	return m_optsFilePath.c_str();
}

void DODConfig::ChangeToHomeDirectory()
{
	if(chdir(GetHomePath().c_str()))
	{
		// TODO: LOG ERROR
	}
}

void DODConfig::ChangeToUserDirectory()
{
	if(chdir((GetHomePath() + USER_DIRECTORY_NAME).c_str()))
	{
		// TODO: LOG ERROR
	}
}

// Simple POSIX file copy
bool DODConfig::CopyFile(std::string srcFile, std::string destFile)
{
	bool bRet = true;
	char buf[BUFSIZ];
	size_t size;

	// Attempt to open files
	int source = open(srcFile.c_str(), O_RDONLY, 0);
	int dest = open(destFile.c_str(), O_WRONLY | O_CREAT /*| O_TRUNC*/, 0644);

	if (source && dest)
	{
		// Copy
		while ((size = read(source, buf, BUFSIZ)) > 0) {
			if(0 >= write(dest, buf, size)) {
				// Handle error;
				bRet = false;
				break;
			}
		}

		// Close files
		if(source) {
			close(source);
		}
		if(dest) {
			close(dest);
		}
	} else {
		bRet = false;
	}
	return bRet;
}

// ValidateUserDirectory
// Checks for presence of user directory structure for configuration files
bool DODConfig::ValidateUserDirectory()
{
	bool bResult = false;
	struct stat buffer;
	std::string userDir = GetHomePath();
	userDir += USER_DIRECTORY_NAME;
	std::string installDir = GetInstallPath();

	// Check that the entire subtree exists
	bResult = (stat (userDir.c_str(), &buffer) == 0);
	bResult &= (stat ((userDir + CONF_DIRECTORY_NAME).c_str(), &buffer) == 0);
	bResult &= (stat ((userDir + SAVED_DIRECTORY_NAME).c_str(), &buffer) == 0);
	bResult &= (stat (GetOptsFilePath(), &buffer) == 0);
	if (!bResult) {
		// Directory is absent.  This means we need to create it and copy over
		// defaults from the install location.
		mkdir(userDir.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		mkdir((userDir + CONF_DIRECTORY_NAME).c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		mkdir((userDir + SAVED_DIRECTORY_NAME).c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

		// Copy config options file
		CopyFile(
			(GetInstallPath() + CONF_DIRECTORY_NAME + OPTS_FILE_NAME),
			GetOptsFilePath()
		);

		cout << "Creating directory\n";
	}
	return bResult;
}

// BuildUserDirectory
// Builds out user directory structure for congiruation files
// Entry: -
// Exit: true == success
bool DODConfig::BuildUserDirectory()
{
	bool bRet = true;       // assume success


	return bRet;
}

std::string DODConfig::GetInstallPath()
{
	return INSTALL_DIRECTORY_NAME;
}

std::string DODConfig::GetHomePath()
{
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;
	std::string path = homedir;
	return path;
}
