#pragma once
///////////////////////////////////////////////////////////////////
// StrHelper.h -								                 //
// Ver 1.0                                                       //
// Application: Header file for NoSqlDb operations               //
// Platform:    Windows 10, Visual Studio 2015				     //
// Author:		Varun Krishna Peruru Vanaparthy					 //	
//				487765001, vperuruv@syr.edu                      //
// Source:      Jim Fawcett, jfawcett@twcny.rr.com               //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the trim operations that includes trimming from beginning,
* end and both sides.
*
* Required Files:
* ---------------
*   - StrHelper.h, StrHelper.cpp,
*
* Build Process:
* --------------
*   devenv KeyValue.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 - 02/07/2017
*
*
*/

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <string>

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	try { return ltrim(rtrim(s)); }
	catch (...) { return std::string("trimming error"); }
}
