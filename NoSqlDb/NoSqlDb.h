#pragma once
#pragma warning(disable : 4996)
///////////////////////////////////////////////////////////////////
// NoSqlDb.h - key/value pair in-memory database                 //
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
* This package implements all the operations of the database including
* addition, modification and deletion.
* This package also supports persistence and regex query capability.
* Addition, modification and deletion for a custom type in this case,
* vector<int> is also supported.
*
* Required Files:
* ---------------
*   - NoSqlDb.h, NoSqlDb.cpp,
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

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <regex>
#include <iomanip>
#include <ctime>
#include "../CppProperties/CppProperties.h"

/////////////////////////////////////////////////////////////////////
// Element class represents a data record in our NoSql database
// - in our NoSql database that is just the value in a key/value pair
// - it needs to store child data, something for you to implement
//

template<typename Data>
class Element
{
public:
	using Name = std::string;
	using Category = std::string;
	using TimeDate = std::string;
	using Children = std::unordered_set<std::string>;

	Property<Name> name;				// metadata
	Property<Category> category;		// metadata
	Property<TimeDate> timeDate;		// metadata
	Property<Data> data;				// data
	Property<Children> children;		// metadata

	std::string show();
	std::string customShow();
};

template<typename Data>
std::string Element<Data>::show()
{
	// show children when you've implemented them

	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(8) << "name" << " : " << name;
	out << "\n    " << std::setw(8) << "category" << " : " << category;
	out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
	out << "\n    " << std::setw(8) << "data" << " : " << data;
	out << "\n    " << std::setw(8) << "child keys" << " : " << "[ ";
	if (!children.getValue().empty())
	{
		for (auto i = children.getValue().begin(); i != children.getValue().end(); i++)
		{
			out << *i << " ";
		}
		out << " ]";
	}
	else
		out << "- ]";
	out << "\n";
	return out.str();
}

template<typename Data>
std::string Element<Data>::customShow()
{
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(8) << "name" << " : " << name;
	out << "\n    " << std::setw(8) << "category" << " : " << category;
	out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
	out << "\n    " << std::setw(8) << "vector<int> data" << " : " << "[ ";
	for (auto i = data.getValue().begin(); i != data.getValue().end(); i++)
	{
		out << *i << " ";
	}
	out << " ]";

	out << "\n    " << std::setw(8) << "child keys" << " : " << "[ ";
	if (!children.getValue().empty())
	{
		for (auto i = children.getValue().begin(); i != children.getValue().end(); i++)
		{
			out << *i << " ";
		}
		out << " ]";
	}
	else
		out << "- ]";

	out << "\n";

	return out.str();
}

/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database
// - stores and retrieves elements
// - you will need to add query capability
//   That should probably be another class that uses NoSqlDb
//
template<typename Data>
class NoSqlDb
{

public:
	using Key = std::string;
	using Keys = std::vector<Key>;
	time_t rawtime;
	char buffer[80];
	struct tm * timeinfo;

	Keys keys();
	bool save(Key key, Element<Data> elem);
	bool addChild(Key key, std::string child);
	bool modifyEntry(Key key, int flag, std::string newValue);

	Element<Data> value(Key key);
	size_t count();

	Element<Data> queryValue(Key key);
	std::unordered_set<std::string> queryChildren(Key key);
	Keys queryKeysByPattern(std::string pattern); //std::vector<std::string> queryKeysByPattern(std::string pattern);
	Keys queryKeysByName(std::string pattern);
	Keys queryKeysByCategory(std::string pattern);
	Keys queryKeysByNameAgain(std::string pattern, Keys newList);
	Keys queryKeysByCategoryAgain(std::string pattern, Keys newList);
	Keys queryKeysByTime(std::string atime, std::string btime);
	bool deleteElement(Key element);
	bool deleteChild(std::string child);
	int getpersistCounter();
	void setpersistCounter(int count);
	void increasePersistCounter();
	void saveToXml(std::ofstream *stream);
private:
	int persistCounter;
	using Item = std::pair<Key, Element<Data>>;

	std::unordered_map<Key, Element<Data>> store;

};

template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
	Keys keys;
	for (Item item : store)
	{
		keys.push_back(item.first);
	}
	return keys;
}

template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
{
	if (store.find(key) != store.end())
		return false;
	store[key] = elem;
	return true;
}

template<typename Data>
bool NoSqlDb<Data>::addChild(Key key, std::string child)
{
	if (store.find(child) != store.end())
	{
		auto search = store[key].children.getValue().find(child);
		if (search == store[key].children.getValue().end())
		{
			store[key].children.getValue().insert(child);
			return true;
		}
		else
		{
			std::cout << child << " already exists. Skipping.\n";
			return false;
		}
	}
	else
		return false;
}

template<typename Data>
bool NoSqlDb<Data>::modifyEntry(Key key, int flag, std::string newValue)
{
	if (store.find(key) == store.end())
		return false;
	if (flag == 1)
		store[key].category = newValue;
	if (flag == 2)
		store[key].data = newValue;
	return true;
}

template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
	if (store.find(key) != store.end())
		return store[key];
	return Element<Data>();
}

template<typename Data>
size_t NoSqlDb<Data>::count()
{
	return store.size();
}

template<typename Data>
int NoSqlDb<Data>::getpersistCounter()
{
	return persistCounter;
}

template<typename Data>
void NoSqlDb<Data>::setpersistCounter(int count)
{
	persistCounter = count;
}

template<typename Data>
void NoSqlDb<Data>::increasePersistCounter()
{
	persistCounter++;
}

template<typename Data>
Element<Data> NoSqlDb<Data>::queryValue(Key key)
{
	if (store.find(key) != store.end())
		return store[key];
	return Element<Data>();
}

template<typename Data>
std::unordered_set<std::string> NoSqlDb<Data>::queryChildren(Key key)
{
	if (store.find(key) != store.end())
		if (!store[key].children.getValue().empty())
			return store[key].children.getValue();
	return std::unordered_set<std::string>();
}

template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::queryKeysByPattern(std::string pattern)
{
	Keys keyList; //std::vector<std::string> keyList;
	for (auto i = store.begin(); i != store.end(); i++)
	{
		std::string s = i->first; //i->second.name.getValue();
		std::regex pattern_(pattern);
		if (std::regex_search(s, pattern_))
			keyList.push_back(s);
	}
	return keyList;
}


template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::queryKeysByName(std::string pattern)
{
	Keys keyList;

	for (auto i = store.begin(); i != store.end(); i++)
	{
		std::string s = i->second.name.getValue();
		std::regex pattern_(pattern);
		if (std::regex_search(s, pattern_))
			keyList.push_back(s);
	}

	return keyList;
}

template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::queryKeysByCategory(std::string pattern)
{
	Keys keyList;

	for (auto i = store.begin(); i != store.end(); i++)
	{
		std::string s = i->second.category.getValue();
		std::regex pattern_(pattern);
		if (std::regex_search(s, pattern_))
			keyList.push_back(i->second.name.getValue());
	}

	return keyList;
}

template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::queryKeysByNameAgain(std::string pattern, Keys origList)
{
	std::vector<std::string> newList;

	for (auto i = origList.begin(); i != origList.end(); i++)
	{
		if (store.find(*i) != store.end())
		{
			std::string s = store.find(*i)->second.name.getValue();
			std::regex pattern_(pattern);
			if (std::regex_search(s, pattern_))
				newList.push_back(s);
		}
	}

	return newList;
}

template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::queryKeysByCategoryAgain(std::string pattern, Keys origList)
{
	std::vector<std::string> newList;

	for (auto i = origList.begin(); i != origList.end(); i++)
	{
		if (store.find(*i) != store.end())
		{
			std::string s = store.find(*i)->second.category.getValue();
			std::regex pattern_(pattern);
			if (std::regex_search(s, pattern_))
				newList.push_back(s);
		}
	}

	return newList;
}

template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::queryKeysByTime(std::string atime, std::string btime)
{
	time_t time_a, time_b, time_key;
	std::vector<std::string> keyList;

	struct std::tm time_temp; // = NULL;
	std::istringstream ss(atime);
	ss >> std::get_time(&time_temp, "%m/%d/%y %X");
	time_a = mktime(&time_temp);

	if (btime == "now")
	{
		time(&time_b);
		localtime(&time_b);
	}
	else
	{
		std::istringstream ss(btime);
		ss >> std::get_time(&time_temp, "%m/%d/%y %X");
		time_b = mktime(&time_temp);
	}

	for (auto i = store.begin(); i != store.end(); i++)
	{
		std::istringstream ss(i->second.timeDate.getValue());
		ss >> std::get_time(&time_temp, "%m/%d/%y %X");
		time_key = mktime(&time_temp);

		int t_a = difftime(time_key, time_a);
		int t_b = difftime(time_b, time_key);
		if (difftime(time_key, time_a) > 1 && difftime(time_key, time_b) < 0)
			keyList.push_back(i->first);
	}

	return keyList;
}

template<typename Data>
bool NoSqlDb<Data>::deleteChild(std::string child)
{
	for (auto i = store.begin(); i != store.end(); i++)
	{
		//Element<Data> elem_1 = i->second;
		if (i->second.children.getValue().size() > 0)
		{
			i->second.children.getValue().erase(child);

		}
	}
	return true;
}

template<typename Data>
bool NoSqlDb<Data>::deleteElement(Key element)
{
	if (store.find(element) == store.end())
		return false;
	store.erase(element);
	return true;
}

template<typename Data>
void NoSqlDb<Data>::saveToXml(std::ofstream *stream)
{

	*stream << "<?xml version=\"1.0\" encoding =\"utf-8\"?>";
	*stream << "\n";
	*stream << "<DBValues>";
	*stream << "\n";

	for (auto i = store.begin(); i != store.end(); i++)
	{
		*stream << "\t<Key name=\"" << i->second.name.getValue() << "\"" << ">";
		*stream << "\n";
		*stream << "\t\t<name>" << i->second.name.getValue() << "</name>";
		*stream << "\n";
		*stream << "\t\t<category>" << i->second.category.getValue() << "</category>";
		*stream << "\n";
		*stream << "\t\t<description>" << i->second.data.getValue() << "</description>";
		*stream << "\n";
		*stream << "\t\t<timeDate>" << trim(i->second.timeDate.getValue()) << "</timeDate>";
		*stream << "\n";
		*stream << "\t\t<children>";

		if (i->second.children.getValue().size() > 0)
		{
			int count = 0;
			for (auto j = i->second.children.getValue().begin(); j != i->second.children.getValue().end(); j++)
			{
				*stream << *j;
				count++;
				if (count != i->second.children.getValue().size())
					*stream << ",";
			}
			//std::vector<string> = i->second.children.getValue();
			//*stream << i->second.children.getValue();// << " ";
		}

		*stream << "</children>";
		*stream << "\n\t</Key>";
		*stream << "\n";
	}
	*stream << "<DBValues>";
}

