//
//
// C++ Implementation: cpp
//
// Description: main application
//
//
// Authors:  <claudiuc@kde.org>, (C) 2001, 2002, 2003
//           <enrico.ros@gmail.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "quax.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("Quax");
    app.setApplicationVersion("1.2");

    // install translators (qt + this application)
    QString locale = QLocale::system().name();
    QTranslator qTranslator;
    qTranslator.load(QString("qt_") + locale, ".");
    app.installTranslator(&qTranslator);
    QTranslator appTranslator;
    appTranslator.load(QString("quax_") + locale, ".");
    app.installTranslator(&appTranslator);

    Quax quax;
    quax.setWindowTitle(app.applicationName());
    quax.show();
    return app.exec();
}
