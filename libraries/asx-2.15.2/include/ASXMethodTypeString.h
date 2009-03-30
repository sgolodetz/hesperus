/***
 * asx: ASXMethodTypeString.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXMETHODTYPESTRING
#define H_ASX_ASXMETHODTYPESTRING

#include "ASXTypeString.h"

//~~~~~~~~~~~~~~~~~
// Default template
//~~~~~~~~~~~~~~~~~

template <typename F> struct ASXMethodTypeString;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Specializations for member function pointer types
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// No arguments, non-const
template <typename T, typename R> struct ASXMethodTypeString<R (T::*)()>
{
	std::string name;

	explicit ASXMethodTypeString(const std::string& name_) : name(name_) {}

	std::string function_type()
	{
		return ASXTypeString<R ()>(name)();
	}

	std::string object_type()
	{
		return ASXTypeString<T>()();
	}
};

// No arguments, const (derived from the non-const version for brevity)
template <typename T, typename R> struct ASXMethodTypeString<R (T::*)() const>
:	ASXMethodTypeString<R (T::*)()>
{
	explicit ASXMethodTypeString(const std::string& name_)
	:	ASXMethodTypeString<R (T::*)()>(name_)
	{}
};

// 1 argument, non-const
template <typename T, typename R, typename Arg0> struct ASXMethodTypeString<R (T::*)(Arg0)>
{
	std::string name;

	explicit ASXMethodTypeString(const std::string& name_) : name(name_) {}

	std::string function_type()
	{
		return ASXTypeString<R (Arg0)>(name)();
	}

	std::string object_type()
	{
		return ASXTypeString<T>()();
	}
};

// 1 argument, const (derived from the non-const version for brevity)
template <typename T, typename R, typename Arg0> struct ASXMethodTypeString<R (T::*)(Arg0) const>
:	ASXMethodTypeString<R (T::*)(Arg0)>
{
	explicit ASXMethodTypeString(const std::string& name_)
	:	ASXMethodTypeString<R (T::*)(Arg0)>(name_)
	{}
};

// TODO: Member functions with more arguments

#endif
