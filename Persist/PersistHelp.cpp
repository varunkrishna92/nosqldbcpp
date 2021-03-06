///////////////////////////////////////////////////////////////////
// PersistHelp.cpp								                 //
// Ver 1.0                                                       //
// Application: Header file for NoSqlDb operations               //
// Platform:    Windows 10, Visual Studio 2015				     //
// Author:		Varun Krishna Peruru Vanaparthy					 //	
//				487765001, vperuruv@syr.edu                      //
// Source:      Jim Fawcett, jfawcett@twcny.rr.com               //
///////////////////////////////////////////////////////////////////

#ifdef TEST_PERSISTHELP
/*
 * - You can define and undefine using 
 *   project > Properties > C/C++ > Preprocessor > Processor Definitions
 * - or using command line
 *   cl /DTEST_PERSISTHELP persist.cpp [plus all the other cpp files needed]
 */

#include "PersistHelp.h"
#include <iostream>
#include "../Utilities/Utilities.h"

int main()
{
  Title("Demonstrate Persisting to and from XML");
  putLine();

  subTitle("Persisting a string");
  StringWrapper sw("Hello World");
  sw = "hello world";     // using assignment overload
  std::string temp = sw;  // using implicit cast
  std::cout << "\n  StringWrapper state = " << static_cast<std::string>(sw);
  StringWrapper::Xml xml = sw.save();
  std::cout << "\n  XML saved from StringWrapper = " << xml;

  StringWrapper newSw;
  newSw.restore(xml);

  std::cout << "\n  id of restored StringWrapper = " << newSw.id();
  std::cout << "\n  restored StringWrapper state = " << static_cast<std::string>(newSw);
  putLine();

  subTitle("Persisting a double");
  DoubleWrapper dw(3.1415927);
  DoubleWrapper::Xml xmlD = dw.save();
  std::cout << "\n  DoubleWrapper state = " << static_cast<double>(dw);
  std::cout << "\n  XML saved from DoubleWrapper = " << xmlD;
  
  DoubleWrapper newDw;
  newDw.restore(xmlD);

  std::cout << "\n  id of restored DoubleWrapper = " << newDw.id();
  std::cout << "\n  restored DoubleWrapper state = " << static_cast<double>(newDw);
  putLine();

  subTitle("Persisting a Widget");
  Widget wdgt;
  wdgt.wint = 42;
  wdgt.wstr = "Fred";
  WidgetWrapper wwdgt(wdgt);
  std::cout << "\n  WidgetWrapper state:";
  std::cout << "\n    wint = " << static_cast<Widget>(wwdgt).wint;
  std::cout << "\n    wstr = " << static_cast<Widget>(wwdgt).wstr;
  WidgetWrapper::Xml xmlW = wwdgt.save();
  std::cout << "\n  XML saved from WidgetWrapper:" << xmlW;

  WidgetWrapper newWwdgt;
  newWwdgt.restore(xmlW);
  std::cout << "\n  id of restored WidgetWrapper = " << newWwdgt.id();
  std::cout << "\n  WidgetWrapper state:";
  std::cout << "\n    wint = " << static_cast<Widget>(newWwdgt).wint;
  std::cout << "\n    wstr = " << static_cast<Widget>(newWwdgt).wstr;
  putLines(2);
}

#endif
