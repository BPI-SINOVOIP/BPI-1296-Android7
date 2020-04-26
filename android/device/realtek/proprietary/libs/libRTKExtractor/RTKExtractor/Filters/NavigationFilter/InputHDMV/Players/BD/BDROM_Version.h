//-----------------------------------------------------------------------------
// BDROM_Version.h
// Copyright (c) 2002-2009, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef BDROM_VERSION_H
#define BDROM_VERSION_H

#include	"BDJModuleTypes.h"

class BDROM_Version
{
public:
	BDROM_Version();
	~BDROM_Version() {}

	void GetVersion(BDROMVersion &version) const;
	void GetBDROMProfileVersion(std::string &version) const;
	void GetBDROMSpecVersion(std::string &version) const;
	void GetBuildNumber(std::string &version) const;

protected:
	const BDROMVersion	*m_pVersion;
};

#endif	// BDROM_VERSION_H
