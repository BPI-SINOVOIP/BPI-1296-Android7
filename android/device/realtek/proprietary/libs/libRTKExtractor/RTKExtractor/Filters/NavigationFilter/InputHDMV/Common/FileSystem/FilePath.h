//-----------------------------------------------------------------------------
// FilePath.h
// Copyright (c) 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

//SR FS: Reviewed [DDT 20040916]
//SR FS: Reviewed [HBY 20040922] 

#ifndef __FilePath_h__
#define __FilePath_h__

class FilePath : public unicode::string8 // filepath specification
{

public:

	FilePath();
	explicit FilePath(const std::string & path);
	FilePath(const std::wstring & path);
	FilePath(const unicode::string8 & path);

	FilePath AppendExtension(const FilePath & extension) const;
	FilePath Canonical(bool caseSensitive) const;
	FilePath DeleteExtension() const;
	FilePath GetBase() const;
	FilePath GetExtension() const;
	FilePath GetName() const;
	bool IsEmpty() const;
	bool IsEqual(const FilePath & path, bool caseSensitive) const;
	bool IsMatch(const FilePath & match, bool caseSensitive) const;
	FilePath LowerCase() const;
	FilePath UpperCase() const;

	static const value_type * separators; // recognized pathname separators
	static const value_type * wildcards; // recognized match wildcards
};

#if defined _WIN32

typedef std::wstring NativePath;

#else

typedef std::string NativePath;

#endif

#endif // __FilePath_h__
