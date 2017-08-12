///////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - key/value pair in-memory database				 //
// Ver 1.0                                                       //
// Application: Source file for NoSqlDb operations               //
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
*   - NoSqlDb.h, NoSqlDb.cpp
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


#include "NoSqlDb.h"
#include "../XmlDocument/xmlElementParts/xmlElementParts.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../XmlDocument/xmlElementParts/Tokenizer.h"
#include "../XmlDocument/XmlParser/XmlParser.h"
#include "../Utilities/StrHelper.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <regex>
#include <algorithm> 
#include <iterator>
#include <time.h>
#include <iomanip>
#include <ctime>
#include <thread>
#include <chrono>
#include <windows.h>
#define persistAfterAdd 5
#define TIME_FORMAT "%Y%m%d_%H%M%S"
#define FILE_SIZE 20

using StrData = std::string;
using intData = int;
using vIntData = std::vector<int>;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
using Children = std::unordered_set<std::string>;
using SPtr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;



std::vector<std::string> queryNameDemo(NoSqlDb<StrData> &db, std::string pattern)
{
	std::vector<std::string> keyListName = db.queryKeysByName(pattern);
	std::cout << "Keys with 'name' matching pattern \"" << pattern << "\" :\n";
	std::cout << "[ ";
	if (!keyListName.empty())
	{
		for (Keys::const_iterator i = keyListName.begin(); i != keyListName.end(); i++)
		{
			std::cout << *i << " ";
		}
		std::cout << "]";
	}
	else
		std::cout << "nil ]";
	std::cout << "\n\n\n";
	return keyListName;
}

std::vector<std::string> queryNameDemoAgain(NoSqlDb<StrData> &db, std::string pattern1, std::vector<std::string> retKeys)
{
	std::vector<std::string> keyListname2 = db.queryKeysByNameAgain(pattern1, retKeys);
	std::cout << "Keys with 'name' matching pattern \"" << pattern1 << "\" :\n";
	std::cout << "[ ";
	if (!keyListname2.empty())
	{
		for (Keys::const_iterator i = keyListname2.begin(); i != keyListname2.end(); i++)
		{
			std::cout << *i << " ";
		}
		std::cout << "]";
	}
	else
		std::cout << "nil ]";
	std::cout << "\n\n\n";
	return keyListname2;
}

std::vector<std::string> queryCategoryDemo(NoSqlDb<StrData> &db, std::string pattern)
{
	std::cout << "regex by category pattern:\n\n";

	std::vector<std::string> keyListCat = db.queryKeysByCategory(pattern);

	std::cout << "Keys with 'category' matching pattern \"" << pattern << "\" :\n";
	std::cout << "[ ";
	if (!keyListCat.empty())
	{
		for (Keys::const_iterator i = keyListCat.begin(); i != keyListCat.end(); i++)
		{
			std::cout << *i << " ";
		}
		std::cout << "]";
	}
	else
		std::cout << "nil ]";
	std::cout << "\n\n\n";
	return keyListCat;
}

std::vector<std::string> queryCategoryAgainDemo(NoSqlDb<StrData> &db, std::vector<std::string> retKeys1)
{
	std::string pattern_category = "2$";
	std::vector<std::string> keyListCategory = db.queryKeysByCategoryAgain(pattern_category, retKeys1);
	std::cout << "Keys with 'category' matching pattern \"" << pattern_category << "\" :\n";
	std::cout << "[ ";
	if (!keyListCategory.empty())
	{
		for (Keys::const_iterator i = keyListCategory.begin(); i != keyListCategory.end(); i++)
		{
			std::cout << *i << " ";
		}
		std::cout << "]";
	}
	else
		std::cout << "nil]";
	std::cout << "\n\n\n";
	return keyListCategory;
}

void UnionDemo(std::vector<std::string> keyList1, std::vector<std::string> keyList2, NoSqlDb<StrData> &db)
{
	std::vector<std::string>::iterator it;
	std::vector<std::string> unionString;
	keyList1.push_back("elem1");
	keyList2.push_back("elem1");
	std::sort(keyList1.begin(), keyList1.end());
	std::sort(keyList2.begin(), keyList2.end());

	std::set_union(keyList1.begin(), keyList1.end(), keyList2.begin(), keyList2.end(), std::back_inserter(unionString));

	for (const auto &i : unionString) {
		std::cout << i << ' ';
	}
	std::cout << "\n\n\n";
}

void DeleteDemo(NoSqlDb<StrData> &db)
{
	Key deleteKey = "elem3";
	std::cout << "\nKey to be deleted: " << deleteKey;
	bool deleted = db.deleteElement(deleteKey);
	if (deleted)
		bool r = db.deleteChild(deleteKey);
	std::cout << "\nAfter Deletion\n";
}

void persist(NoSqlDb<StrData> &db)
{
	static char file_name[FILE_SIZE];
	time_t currTime = time(0);
	strftime(file_name, sizeof(file_name), TIME_FORMAT, localtime(&currTime));
	std::ofstream *file1 = new std::ofstream;
	strcat(file_name, ".xml");
	file1->open(file_name);
	db.saveToXml(file1);
	file1->close();
	std::cout << "Database persisted to: " << file_name << "\n";
}

void timeTestDemo(NoSqlDb<StrData> &db)
{
	Keys keyListTime;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::string timeA = "01/04/17 23:50:52";
	std::string timeB = "now"; 
	keyListTime = db.queryKeysByTime(timeA, timeB);
	std::cout << "Keys with 'timeDate' between " << timeA << " and " << timeB << " :\n";
	std::cout << "[ ";
	if (!keyListTime.empty())
	{
		for (Keys::const_iterator i = keyListTime.begin(); i != keyListTime.end(); i++)
		{
			std::cout << *i << " ";
		}
		std::cout << "]";
	}
	else
		std::cout << "nil ]";
	std::cout << "\n\n";
}

void queryChildrenDemo(NoSqlDb<StrData> &db)
{
	Children childKey;

	std::cout << "\n Query for Children of elem1 returned:\n";
	childKey = db.queryChildren("elem1");

	std::cout << "[ ";
	if (!childKey.empty())
	{
		for (auto i = childKey.begin(); i != childKey.end(); i++)
		{
			std::cout << *i << " ";
		}
		std::cout << "]";
	}
	else
		std::cout << "nil ]";
	std::cout << "\n\n";

	std::cout << "\n Query for Children of elem4 returned:\n";
	childKey = db.queryChildren("elem4");

	std::cout << "[ ";
	if (!childKey.empty())
	{
		for (auto i = childKey.begin(); i != childKey.end(); i++)
		{
			std::cout << *i << " ";
		}
		std::cout << "]";
	}
	else
		std::cout << "nil ]";
	std::cout << "\n\n";
}

void queryKeyDemo(NoSqlDb<StrData> &db)
{
	Keys keyList;

	std::string pattern = "^e*";
	keyList = db.queryKeysByPattern(pattern);
	std::cout << "[ ";
	if (!keyList.empty())
	{
		for (Keys::const_iterator i = keyList.begin(); i != keyList.end(); i++)
		{
			std::cout << *i << " ";
		}
		std::cout << "]";
	}
	else
		std::cout << "nil ]";
	std::cout << "\n\n";

	pattern = "test";
	keyList = db.queryKeysByPattern(pattern);
	std::cout << "\n Query returned Keys matching pattern \"" << pattern << "\" :\n";
	std::cout << "[ ";
	if (!keyList.empty())
	{
		for (Keys::const_iterator i = keyList.begin(); i != keyList.end(); i++)
		{
			std::cout << *i << " ";
		}
		std::cout << "]";
	}
	else
		std::cout << "nil ]";
	std::cout << "\n\n";
}

void checkPersistence(NoSqlDb<StrData> &db)
{
	db.increasePersistCounter();
	if (db.getpersistCounter() == persistAfterAdd)
	{
		db.setpersistCounter(0);
		static char file_name[FILE_SIZE];
		time_t currTime = time(0);
		strftime(file_name, sizeof(file_name), TIME_FORMAT, localtime(&currTime));
		std::ofstream *file = new std::ofstream;
		strcat(file_name, ".xml");
		file->open(file_name);
		db.saveToXml(file);
		file->close();
		std::cout << "---------------------------Requirement 6-----------------------------------";
		std::cout << "\n  Number of writes to the database reached '5' since last persist.\n";
		std::cout << "\n  Database persisted to the file: " << file_name << "\n\n";
	}
}

void showDbDemo(NoSqlDb<StrData> &db)
{
	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";
	std::cout << "\n  size of db = " << db.count() << "\n";
	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
	std::cout << "\n\n";
}

void addChildren(NoSqlDb<StrData> &db, std::vector<SPtr> found, std::vector<Element<StrData>> elements, int numofElements)
{
	for (int i = 0; i < numofElements; i++)
	{
		checkPersistence(db);

		if (found[i]->children()[4]->children().size() > 0)
		{
			std::string childNames = found[i]->children()[4]->children()[0]->value();
			childNames = trim(childNames);
			std::istringstream ss(childNames);
			std::string token;

			while (std::getline(ss, token, ','))
			{
				db.addChild(elements.at(i).name, token);
			}
		}
	}
}

std::vector<Element<StrData>> loadFromXML(NoSqlDb<StrData> &db, std::string filepath)
{
	time_t rawtime;
	struct tm * timeinfo;
	char  timeBuffer[80];
	std::vector<Element<StrData>> elements;

	std::string xmlSrc = filepath;
	XmlProcessing::XmlParser parser(xmlSrc);
	//Parsing compressed XML
	XmlProcessing::XmlDocument* pDoc = parser.buildDocument();
	XmlProcessing::XmlDocument doc(pDoc->toString(), XmlProcessing::XmlDocument::str);
	//Find number of elements
	int numofElements = doc.elements("DBValues").select().size();
	std::vector<SPtr> found = doc.elements("DBValues").select();
	for (int i = 0; i < numofElements; i++)
	{
		std::string nameTag = found[i]->children()[0]->children()[0]->value();
		nameTag = trim(nameTag);

		std::string categoryTag = found[i]->children()[1]->children()[0]->value();
		categoryTag = trim(categoryTag);

		std::string dataTag = found[i]->children()[2]->children()[0]->value();
		dataTag = trim(dataTag);

		if (found[i]->children()[3]->children().size() > 0)
		{
			std::string timeTag = found[i]->children()[3]->children()[0]->value();
			timeTag = trim(timeTag);
			timeTag.copy(timeBuffer, 80);
		}
		else
		{
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			strftime(timeBuffer, 80, "%m/%d/%y %X", timeinfo);
		}

		Element<StrData> elem;
		elem.name = nameTag;
		elem.category = categoryTag;
		elem.data = dataTag;
		elem.timeDate = timeBuffer;
		elements.push_back(elem);
		db.save(elements.at(i).name, elements.at(i));
	}
	addChildren(db, found, elements, numofElements);
	return elements;
}

void modifyDemo(NoSqlDb<StrData> &db, std::vector<Element<StrData>> dbElements)
{
	db.modifyEntry(dbElements.at(0).name, 1, "newCategory");
	checkPersistence(db);
	std::cout << db.queryValue("elem1").show();
	db.modifyEntry(dbElements.at(1).name, 2, "elem2 MODIFIED data");
	checkPersistence(db);
	std::cout << "\n querying for elem2 after modify:\n";
	std::cout << db.queryValue("elem2").show();
}

void showCustomDBDemo(NoSqlDb<vIntData> &vDB)
{
	std::cout << "\n  Retrieving elements from NoSqlDb<vector<int>>";
	std::cout << "\n ------------------------------------------\n";
	std::cout << "\n  size of db = " << vDB.count() << "\n";
	Keys custKey = vDB.keys();
	for (Key key : custKey)
	{
		std::cout << "\n  " << key << ":";
		std::cout << vDB.value(key).customShow();
	}
	std::cout << "\n\n";
}

void customAddChildren(NoSqlDb<vIntData> &vDB, std::vector<SPtr> found_v, std::vector<Element<vIntData>> custItem, int numItems)
{
	for (int i = 0; i < numItems; i++)
	{
		if (found_v[i]->children()[4]->children().size() > 0)
		{
			std::string custChild = found_v[i]->children()[4]->children()[0]->value();
			custChild = trim(custChild);
			std::istringstream v_ss(custChild);
			std::string v_token;

			while (std::getline(v_ss, v_token, ',')) {
				vDB.addChild(custItem.at(i).name, v_token);
			}

		}
	}
}

std::vector<Element<vIntData>> loadCustomFromXML(NoSqlDb<vIntData> &vDB, std::string filepath)
{
	time_t rawtime;
	struct tm * timeinfo;
	char  timeBuffer[80];
	std::vector<Element<vIntData>> custItem;
	std::string vIntXmlSrc = filepath;
	XmlProcessing::XmlParser parser_v(vIntXmlSrc);
	//Parsing compressed XML
	XmlProcessing::XmlDocument* pDoc_v = parser_v.buildDocument();
	XmlProcessing::XmlDocument doc_v(pDoc_v->toString(), XmlProcessing::XmlDocument::str);
	//Find number of elements
	int numItems = doc_v.elements("DBValues").select().size();
	std::vector<SPtr> found_v = doc_v.elements("DBValues").select();
	for (int i = 0; i < numItems; i++)
	{
		std::string custName = found_v[i]->children()[0]->children()[0]->value();
		custName = trim(custName);
		std::string custCat = found_v[i]->children()[1]->children()[0]->value();
		custCat = trim(custCat);
		std::vector<int> custData;
		std::string dataString = found_v[i]->children()[2]->children()[0]->value();
		dataString = trim(dataString);
		std::istringstream dataStream(dataString);
		std::string dataToken;
		while (std::getline(dataStream, dataToken, ','))
		{
			custData.push_back(std::stoi(dataToken, nullptr, 10));
		}
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(timeBuffer, 80, "%m/%d/%y %X", timeinfo);
		Element<vIntData> item;
		item.name = custName;
		item.category = custCat;
		item.data = custData;
		item.timeDate = timeBuffer;
		custItem.push_back(item);
		vDB.save(custItem.at(i).name, custItem.at(i));
	}
	customAddChildren(vDB, found_v, custItem, numItems);
	return custItem;
}

int main()
{
	std::cout << "\t\t\t\t Varun Krishna Peruru Vanparthy" << "\n";
	std::cout << "\t\t\t\t\t Key Value Database" << "\n";
	NoSqlDb<StrData> db;	NoSqlDb<vIntData> vDB;
	std::cout << "\n  Creating NoSqlDB of string-data (StrData)" << "\n";
	std::cout << "---------------------------Requirement 3 and 5-----------------------------------";
	std::cout << "\n  Loading data from  \"inputData.xml\"" << "\n";
	std::string fileName = "../inputData.xml";
	std::vector<Element<StrData>> dbElements = loadFromXML(db, fileName); 	showDbDemo(db);
	std::cout << "---------------------------Requirement 3-----------------------------------";
	std::cout << "\n  Demonstrating deletion of keys from the database" << "\n";
	DeleteDemo(db);
	showDbDemo(db);
	std::cout << "---------------------------Requirement 4-----------------------------------";
	std::cout << "\n Editing data present in DB\n";
	modifyDemo(db, dbElements);
	std::cout << "---------------------------Requirement 7-----------------------------------";
	std::cout << "\n  Querying DB for Children of a specified key" << "\n\n";
	queryChildrenDemo(db);
	std::cout << "---------------------------Requirement 7-----------------------------------";
	std::cout << "\n  Querying DB for keys that have matching keys using regex" << ":\n\n";
	queryKeyDemo(db);
	std::string patternName = "^\\w{4}[1-4]$";
	std::cout << "---------------------------Requirement 7-----------------------------------";
	std::cout << "\n  Querying DB for keys that have matching names using regex" << ":\n\n";
	std::vector<std::string> keyListName = queryNameDemo(db, patternName);
	std::cout << "---------------------------Requirement 7-----------------------------------";
	std::cout << "\n  Querying DB for keys that have matching categories using regex" << ":\n\n";
	std::string patternCat1 = "[cat]\\s2$";
	std::string patternCat2 = "[cat]\\s1$";
	std::vector<std::string> keyListCat1 = queryCategoryDemo(db, patternCat1);
	std::vector<std::string> keyListCat2 = queryCategoryDemo(db, patternCat2);
	std::cout << "---------------------------Requirement 7-----------------------------------";
	std::cout << "\n  Querying DB for keys inserted between the specified timestamp" << ":\n\n";
	timeTestDemo(db);
	std::string patternNameAg = "2$";
	std::cout << "---------------------------Requirement 8-----------------------------------";
	std::cout << "\n  Querying keys returned from the previous query for keys that have matching names using regex" << ":\n\n";
	std::vector<std::string> retKeys_name2 = queryNameDemoAgain(db, patternNameAg, keyListName);
	std::cout << "---------------------------Requirement 8-----------------------------------";
	std::cout << "\n  Querying keys returned from the previous query for keys that have matching categories using regex" << ":\n\n";
	std::vector<std::string> retKeysCatAg = queryCategoryAgainDemo(db, keyListCat1);
	std::cout << "---------------------------Requirement 9-----------------------------------";
	std::cout << "\n Demonstrating union of keys returned from previous queries" << ":\n\n";
	UnionDemo(keyListCat1, keyListCat2, db);
	std::cout << "---------------------------Requirement 5-----------------------------------";
	std::cout << "\n Demonstrating persistence of keys to an xml file" << ":\n\n"; 	persist(db);
	std::cout << "\n\n";
	std::cout << "\n  Creating NoSqlDB with custom data of type vector<int>..\n";
	std::cout << "\n  Requirement 3: Adding Keys to DB by loading data from file \"customData.xml\"\n";
	fileName = "../customData.xml";
	std::vector<Element<vIntData>> customElements = loadCustomFromXML(vDB, fileName); 	showCustomDBDemo(vDB);
}


