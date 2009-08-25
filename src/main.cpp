//
//
// C++ Implementation: cpp
//
// Description: main application
//
//
// Author:  <claudiuc@kde.org>, (C) 2001, 2002, 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <qtextcodec.h>
#include <qtranslator.h>
#include <qapplication.h>

#include "quax.h"


int main(int argc, char ** argv) {

  QApplication a(argc,argv);  

  // translation file for Qt
  QTranslator qt(0);
  qt.load(QString("qt_")+QTextCodec::locale(),".");
  a.installTranslator(&qt);
  
  // translation file for my application
  QTranslator myapp(0);
  qt.load(QString("quax_")+QTextCodec::locale(),".");
  a.installTranslator(&myapp);

  Quax * quax = new Quax();
  
  a.setMainWidget(quax);
  quax->setCaption("Quax");
  quax->show();
  return a.exec();
}
