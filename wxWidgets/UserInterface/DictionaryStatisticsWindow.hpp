/* 
 * File:   DictionaryStatisticsWindow.hpp
 * Author: mr.sys
 *
 * Created on 13. Oktober 2014, 23:22
 */

#ifndef DICTIONARYSTATISTICSWINDOW_HPP
#define	DICTIONARYSTATISTICSWINDOW_HPP

#include <wx/dialog.h> //base class wxDialog

class DictionaryStatisticsWindow
  : public wxDialog
{
public:
  DictionaryStatisticsWindow();
  DictionaryStatisticsWindow(const DictionaryStatisticsWindow& orig);
  virtual ~DictionaryStatisticsWindow();
private:

};

#endif	/* DICTIONARYSTATISTICSWINDOW_HPP */

