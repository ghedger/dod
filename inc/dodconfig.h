#pragma once

// This file centralizes the configuration management (save files, opts.ini)
//
// Copyright (C) 2017 Greg Hedger

#include "oslink.h"
#include <string>

class DODConfig
{
	public:
		// Constructor/Destructor
		DODConfig();
		~DODConfig() {};

		void ChangeToHomeDirectory();
		void ChangeToUserDirectory();
		bool ValidateUserDirectory();
		bool CopyFile(std::string source, std::string dest);
		const char *GetOptsFilePath();
	protected:
		std::string GetHomePath();
		std::string GetInstallPath();
	private:
		std::string m_optsFilePath;

};

