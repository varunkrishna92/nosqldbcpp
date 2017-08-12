#pragma once
///////////////////////////////////////////////////////////////////
// Utilities.h  								                 //
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
* This package implements lambdas that provide mildly useful local processing.
*
* Required Files:
* ---------------
*   - Utilities.h, Utilities.cpp,
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

#include <functional>
#include <iostream>


std::function<void()> putLine = []() { std::wcout << "\n"; };

std::function<void(size_t)> putLines = [](size_t n) {
  for (size_t i = 0; i < n; ++i)
    putLine();
};

std::function<void(const std::string&, char)> titleCore = [](const std::string& msg, char underline = '-') {
  std::cout << "\n  " << msg.c_str();
  std::wcout << "\n " << std::string(msg.size() + 2, underline).c_str();
};

std::function<void(const std::string&)> Title = [](const std::string& msg) { titleCore(msg, '='); };

std::function<void(const std::string&)> subTitle = [](const std::string& msg) { titleCore(msg, '-'); };
