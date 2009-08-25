//
//
// C++ Implementation: cpp
//
// Description: Quax application implementation
//
//
// Author:  <claudiuc@kde.org>, (C) 2001, 2002, 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "mag.xpm"
#include "mag-alpha.xpm"
#include "exit.xpm"
#include "viewmag.xpm"
#include "viewmagin.xpm"
#include "viewmagout.xpm"
#include "look.xpm"
#include "left.xpm"
#include "right.xpm"
#include "icon.xpm"
#include "help.xpm"

#include <qglobal.h>

#if QT_VERSION >= 300
#include <q3process.h>
#else
#include <qmatrix.h>
#include <qbitmap.h>
#include <q3accel.h>
#endif

#include <qtooltip.h>
#include <qcolor.h>
#include <qcursor.h>
#include <qclipboard.h>


#include "quax.h"
//Added by qt3to4:
#include <QTimerEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QPixmap>
#include <Q3Frame>
#include <Q3PopupMenu>
#include <QMouseEvent>
#include <QEvent>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QDesktopWidget>


Quax::Quax() : QWidget(0,0,Qt::WStyle_Customize|Qt::WStyle_NoBorder|Qt::WType_TopLevel) {

    QPixmap icon=QPixmap(icon_xpm);
    setIcon(icon);

#ifdef _OS_WIN32_
    hand_cursor = QCursor(Qt::ArrowCursor);
#else
    // "borrowed" from kcursor.cpp from kdelibs
    static const unsigned char HAND_BITS[] = {
                0x80, 0x01, 0x00, 0x40, 0x02, 0x00, 0x40, 0x02, 0x00, 0x40, 0x02,
                0x00, 0x40, 0x02, 0x00, 0x40, 0x02, 0x00, 0x40, 0x1e, 0x00, 0x40,
                0xf2, 0x00, 0x40, 0x92, 0x01, 0x70, 0x92, 0x02, 0x50, 0x92, 0x04,
                0x48, 0x80, 0x04, 0x48, 0x00, 0x04, 0x48, 0x00, 0x04, 0x08, 0x00,
                0x04, 0x08, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x20,
                0x00, 0x02, 0x40, 0x00, 0x02, 0x40, 0x00, 0x01, 0xc0, 0xff, 0x01
            };
    static const unsigned char HAND_MASK_BITS[] = {
                0x80, 0x01, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03,
                0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x1f, 0x00, 0xc0,
                0xff, 0x00, 0xc0, 0xff, 0x01, 0xf0, 0xff, 0x03, 0xf0, 0xff, 0x07,
                0xf8, 0xff, 0x07, 0xf8, 0xff, 0x07, 0xf8, 0xff, 0x07, 0xf8, 0xff,
                0x07, 0xf8, 0xff, 0x07, 0xf0, 0xff, 0x07, 0xf0, 0xff, 0x07, 0xe0,
                0xff, 0x03, 0xc0, 0xff, 0x03, 0xc0, 0xff, 0x01, 0xc0, 0xff, 0x01
            };
	QBitmap hand_bitmap(22, 22, HAND_BITS, true);
    QBitmap hand_mask(22, 22, HAND_MASK_BITS, true);
    hand_cursor = QCursor(hand_bitmap, hand_mask, 7, 0);
#endif

#if QT_VERSION >= 300
    cross_cursor = QCursor(Qt::CrossCursor);
    copy_cursor = QCursor(Qt::SizeAllCursor);
#else

    cross_cursor = QCursor(CrossCursor);
    copy_cursor = QCursor(SizeAllCursor);
#endif
    setCursor(hand_cursor);

    // fix the size
    setMinimumHeight(150);
    setMaximumHeight(150);
    setMinimumWidth(150);
    setMaximumWidth(150);
    /// setBackgroundMode(NoBackground);

    // set the shape
    pix=QPixmap(mag_xpm);
    pix_alpha=QPixmap(mag_alpha_xpm);
    /// setMask(*pix_alpha.mask());
#if QT_VERSION > 300
    setErasePixmap(pix);
#else
    setBackgroundPixmap(pix);
#endif


    pix_cursor.resize(14,14);
    pixelColorIcon = new QIcon();
    colorStringDecimal = QString("255, 255, 255");
    colorStringHexaLower = QString("#ffffff");
    colorStringHexaUpper = QString("#FFFFFF");
    
    colorTipShowed=false;
    cursorOverQuax=false;
    ctrlKeyOn=false;
    zoom=ZOOM_SCALE_MIN;
    inDrag=0;
    look=0;


    // build RMB popup menu
    menu = new Q3PopupMenu(this);

    menuzoom = new Q3PopupMenu(this);
    menuzoom->setCheckable(true);
    for (int i=ZOOM_SCALE_MIN; i<=ZOOM_SCALE_MAX; i++) {
        zoomid[i]=menuzoom->insertItem(QObject::tr("1:%1").arg(i),this,SLOT(zoomTo(int)));
    }
    menuzoom->insertSeparator();
#if QT_VERSION >= 300
    menuzoom->insertItem(QPixmap(viewmagin_xpm),QObject::tr("Zoom In"),this,SLOT(zoomIn()),QKeySequence(QObject::tr("+","Zoom In")));
    menuzoom->insertItem(QPixmap(viewmagout_xpm),QObject::tr("Zoom Out"),this,SLOT(zoomOut()),QKeySequence(QObject::tr("-","Zoom Out")));
#else
    menuzoom->insertItem(QPixmap(viewmagin_xpm),QObject::tr("Zoom In"),this,SLOT(zoomIn()),Q3Accel::stringToKey(QObject::tr("+","Zoom In")));
    menuzoom->insertItem(QPixmap(viewmagout_xpm),QObject::tr("Zoom Out"),this,SLOT(zoomOut()),Q3Accel::stringToKey(QObject::tr("-","Zoom Out")));
#endif

    menulook = new Q3PopupMenu(this);
    menulook->setCheckable(true);
#if QT_VERSION >= 300
    lookid[1]=menulook->insertItem(QObject::tr("North-West"),this,SLOT(rotateNorthWest()),QKeySequence(QObject::tr("U","North-West")));
    lookid[0]=menulook->insertItem(QObject::tr("South-West"),this,SLOT(rotateSouthWest()),QKeySequence(QObject::tr("J","South-West")));
    lookid[2]=menulook->insertItem(QObject::tr("North-East"),this,SLOT(rotateNorthEast()),QKeySequence(QObject::tr("I","North-East")));
    lookid[3]=menulook->insertItem(QObject::tr("South-East"),this,SLOT(rotateSouthEast()),QKeySequence(QObject::tr("K","South-East")));
#else
    lookid[1]=menulook->insertItem(QObject::tr("North-West"),this,SLOT(rotateNorthWest()),Q3Accel::stringToKey(QObject::tr("u","North-West")));
    lookid[0]=menulook->insertItem(QObject::tr("South-West"),this,SLOT(rotateSouthWest()),Q3Accel::stringToKey(QObject::tr("j","South-West")));
    lookid[2]=menulook->insertItem(QObject::tr("North-East"),this,SLOT(rotateNorthEast()),Q3Accel::stringToKey(QObject::tr("i","North-East")));
    lookid[3]=menulook->insertItem(QObject::tr("South-East"),this,SLOT(rotateSouthEast()),Q3Accel::stringToKey(QObject::tr("k","South-East")));
#endif
    menulook->insertSeparator();
#if QT_VERSION >= 300
    menulook->insertItem(QPixmap(left_xpm),QObject::tr("Rotate Left"),this,SLOT(rotateLeft()),QKeySequence(QObject::tr("L","Rotate Left")));
    menulook->insertItem(QPixmap(right_xpm),QObject::tr("Rotate Right"),this,SLOT(rotateRight()),QKeySequence(QObject::tr("R","Rotate Right")));
#else
    menulook->insertItem(QPixmap(left_xpm),QObject::tr("Rotate Left"),this,SLOT(rotateLeft()),Q3Accel::stringToKey(QObject::tr("l","Rotate Left")));
    menulook->insertItem(QPixmap(right_xpm),QObject::tr("Rotate Right"),this,SLOT(rotateRight()),Q3Accel::stringToKey(QObject::tr("r","Rotate Right")));
#endif
		
    menucolor = new Q3PopupMenu(this);
    colorid[1]=menucolor->insertItem(colorStringDecimal,this,SLOT(copyColor(int)));
#if QT_VERSION >= 300
    colorid[2]=menucolor->insertItem(colorStringHexaLower,this,SLOT(copyColor(int)),QKeySequence(QObject::tr("Ctrl+C","Copy Color")));
#else
    colorid[2]=menucolor->insertItem(colorStringHexaLower,this,SLOT(copyColor(int)),Q3Accel::stringToKey(QObject::tr("Ctrl+c","Copy Color")));
#endif
    colorid[3]=menucolor->insertItem(colorStringHexaUpper,this,SLOT(copyColor(int)));

    menu->insertItem(QPixmap(viewmag_xpm),QObject::tr("Zoom"),menuzoom);
    menu->insertItem(QPixmap(look_xpm),QObject::tr("Look At"),menulook);
    colorid[0]=menu->insertItem(pix_cursor,QObject::tr("Copy Color"),menucolor);
#if QT_VERSION >= 300
    menu->insertItem(QPixmap(help_xpm),QObject::tr("Help"),this,SLOT(help()),QKeySequence(QObject::tr("H","Help")));
#else
    menu->insertItem(QPixmap(help_xpm),QObject::tr("Help"),this,SLOT(help()),Q3Accel::stringToKey(QObject::tr("h","Help")));
#endif
    menu->insertSeparator();
    menu->insertItem(QObject::tr("About Quax"),this,SLOT(about()));
    menu->insertItem(QObject::tr("About Qt"),this,SLOT(aboutQt()));
    menu->insertSeparator();
#if QT_VERSION >= 300
    menu->insertItem(QPixmap(exit_xpm),QObject::tr("Quit"),qApp,SLOT(quit()),QKeySequence(QObject::tr("Q","Quit")));
#else
    menu->insertItem(QPixmap(exit_xpm),QObject::tr("Quit"),qApp,SLOT(quit()),Q3Accel::stringToKey(QObject::tr("q","Quit")));
#endif

    menuzoom->setItemChecked(zoomid[zoom],true);
    menulook->setItemChecked(lookid[look],true);
    connect(menu,SIGNAL(aboutToShow()),this,SLOT(updateMenuColor()));
    
    // set the color tooltip
    colorTip = new QLabel; /// (0,0,WStyle_StaysOnTop | WStyle_Customize | WStyle_NoBorder | WStyle_Tool | WX11BypassWM);
    colorTip->setMargin(1);
    colorTip->setIndent(0);
    /// colorTip->setAutoMask(false);
    colorTip->setLineWidth(1);
#if QT_VERSION >= 300
    colorTip->setAlignment(Qt::AlignLeft | Qt::AlignTop);
#else
    colorTip->setAlignment(Qt::AlignLeft | Qt::AlignTop);
#endif
    /// colorTip->setAutoResize(true);
    colorTip->setFrameStyle(Q3Frame::Box | Q3Frame::Plain);
    colorTip->setPalette(QToolTip::palette());
    colorTip->setTextFormat(Qt::RichText);

    
    setMouseTracking(true);
    startTimer(UPDATE);
}


Quax::~Quax() {
  delete colorTip;
}


void Quax::enterEvent(QEvent *e) {
  e=e;

  cursorOverQuax=true;
  if (colorTipShowed && colorTip->isHidden()) {
    displayColorTip();
  }
}


void Quax::leaveEvent(QEvent *e) {
  e=e;

  cursorOverQuax=false;
  if (colorTipShowed) {
    colorTip->hide();
  }
}


void Quax::paintEvent(QPaintEvent *e) {
    e=e;
    
    if (pix_zoom.isNull())
        return;

    QPainter paint(this);
    paint.setClipping(true);
    paint.setClipRegion(QRegion(SHAPE_INNER_X,SHAPE_INNER_Y,SHAPE_INNER_WIDTH,SHAPE_INNER_HEIGHT,QRegion::Ellipse));


    int x,y;
    if (pix_zoom.width()>SHAPE_INNER_WIDTH) {
        x=(pix_zoom.width()-SHAPE_INNER_WIDTH)/2;
    } else {
        x=(SHAPE_INNER_WIDTH-pix_zoom.width())/2;
    }
    if (pix_zoom.height()>SHAPE_INNER_HEIGHT) {
        y=(pix_zoom.height()-SHAPE_INNER_HEIGHT)/2;
    } else {
        y=(SHAPE_INNER_HEIGHT-pix_zoom.height())/2;
    }
    x += SHAPE_INNER_X;
    y += SHAPE_INNER_Y;

    paint.drawPixmap(x,y,pix_zoom,0,0,SHAPE_INNER_WIDTH,SHAPE_INNER_HEIGHT);
}

void Quax::timerEvent(QTimerEvent *e) {
    e=e;
    grab();
}

void Quax::wheelEvent(QWheelEvent *e) {
  if (ctrlKeyOn) {
    if (e->delta()>0) {
      rotateLeft();
    } else {
      rotateRight();
    }
  } else {
    if (e->delta()>0) {
      zoomIn();
    } else {
      zoomOut();
    }
  }
  e->accept();
  if (colorTipShowed && ! colorTip->isHidden()) {
    displayColorTip();
  }
}

void Quax::grab() {
    QPixmap pix_grab;
    int x, y, w, h;
    int grabX, grabY, grabW, grabH, ofsX, ofsY;

    w = SHAPE_INNER_WIDTH/zoom;
    h = SHAPE_INNER_HEIGHT/zoom;
    switch (look) {
    case 0: // South-West
        x=this->x()-w;
        y=this->y()+height();
        break;
    case 1: // North-West
        x=this->x()-w;
        y=this->y()-h;
        break;
    case 2: // North-East
        x=this->x()+width();
        y=this->y()-h;
        break;
    case 3: // South-East
        x=this->x()+width();
        y=this->y()+height();
        break;
    default:
        x=this->x();
        y=this->y();
        qWarning("look=%d: This must not happen",look);
    }

    // calculate grab surface to avoid garbage
    if (x<0) {
        grabX = 0;
        grabW = x+w;
        ofsX  = -x;
    } else if (x+w > qApp->desktop()->width()-1) {
        grabX = x;
        grabW = qApp->desktop()->width()-1-x;
        ofsX  = 0;
    } else {
        grabX = x;
        grabW = w;
        ofsX  = 0;
    }
    if (y<0) {
        grabY = 0;
        grabH = y+h;
        ofsY  = -y;
    } else if (y+h > qApp->desktop()->height()-1) {
        grabY = y;
        grabH = qApp->desktop()->height()-1-y;
        ofsY  = 0;
    } else {
        grabY = y;
        grabH = h;
        ofsY  = 0;
    }


    pix_grab.resize(w,h);
    QPainter p(&pix_grab);
    p.fillRect(0,0,w,h,QBrush(Qt::black));
    if (grabW>0 && grabH>0) {
        QPixmap pix_temp = QPixmap::grabWindow(QApplication::desktop()->winId(),grabX,grabY,grabW,grabH);
        p.drawPixmap(ofsX,ofsY,pix_temp);
    }
    p.end();

    QMatrix m;
    m.scale((double)zoom,(double)zoom);
    pix_zoom=pix_grab.xForm(m);
    repaint(false);
}

void Quax::grabForPixel() {
  // get the color under cursor
  QPixmap pix_grab = QPixmap::grabWindow(QApplication::desktop()->winId(),QCursor::pos().x()-1,QCursor::pos().y()-1,1,1);
  QImage img_grab = pix_grab.convertToImage();
  QRgb rgb_color = img_grab.pixel(0,0);
  colorStringDecimal = QString::number(qRed(rgb_color)) + ", " + 
                       QString::number(qGreen(rgb_color)) + ", " + 
                       QString::number(qBlue(rgb_color));
  colorStringHexaLower.sprintf("#%02x%02x%02x",qRed(rgb_color),qGreen(rgb_color),qBlue(rgb_color));
  colorStringHexaUpper = colorStringHexaLower.upper();

  // make a little pixmap with grabbed color
  QMatrix m;
  m.scale((double)14,(double)14);
  pix_cursor=pix_grab.xForm(m);
}

void Quax::displayColorTip() {
return;
/*
  grabForPixel();
  Q3MimeSourceFactory::defaultFactory()->setPixmap("pixel",pix_cursor);    
  colorTip->setText("<qml>&nbsp;<img height=\"14\" width=\"14\" src=\"pixel\">&nbsp;<tt>" + colorStringHexaUpper + "</tt></qml>");
  
  int x_pos, y_pos;
  int x_ofs=7, y_ofs=5;
  int x_margin=2, y_margin=2;
  //check if tooltip get outside screen area 
  QRect  r=colorTip->rect();
  QPoint c=QCursor::pos();
  if (c.x()+r.width()+x_ofs+x_margin > qApp->desktop()->width()) {
    x_pos=c.x()-r.width()-x_ofs;
  } else {
    x_pos=c.x()+x_ofs;
  }
  if (c.y()-r.height()-y_ofs-y_margin < 0) {
    y_pos=c.y()+y_ofs;
  } else {
    y_pos=c.y()-r.height()-y_ofs;
  }
  colorTip->move(x_pos,y_pos);
  if (colorTip->isHidden()) colorTip->show();
  */
}


void Quax::mousePressEvent(QMouseEvent *e) {
    switch (e->button()) {
    case Qt::LeftButton:
        if (colorTipShowed) {
          copyColor(0);
          setCursor(copy_cursor);
        } else {
          grabMouse(hand_cursor);
          inDrag=true;
          dragOffset = QCursor::pos() - QPoint(geometry().left(),geometry().top());
        }
        break;
    case Qt::RightButton:
        menu->popup(QCursor::pos());
        break;
    default:
        break;
    }
}

void Quax::mouseReleaseEvent(QMouseEvent *e) {
    switch (e->button()) {
    case Qt::LeftButton:
        if (colorTipShowed) {
          setCursor(cross_cursor);
        } else {
          releaseMouse();
          inDrag=false;
        }
        break;
    default:
        break;
    }
}

void Quax::mouseMoveEvent(QMouseEvent *e) {
    e=e;
    
    if (colorTipShowed && cursorOverQuax) {
      displayColorTip();
    }
    if (!inDrag || this != mouseGrabber()) return;
    move(QCursor::pos()-dragOffset);
}


void Quax::keyReleaseEvent(QKeyEvent *e) {
    if (ctrlKeyOn) {
      if (!(e->state() & Qt::ShiftButton) && (e->key() == Qt::Key_Control)) {
        ctrlKeyOn=false;
      }
    }
    
    if (colorTipShowed) {
      if (e->key() == Qt::Key_Control ||e->key() == Qt::Key_Shift) {
        colorTipShowed=false;
        setCursor(hand_cursor);
        colorTip->hide();
      }
    }
}

void Quax::keyPressEvent(QKeyEvent *e) {
    QPoint dist(0,0);

    if (!(e->state() & Qt::ShiftButton) && (e->key() == Qt::Key_Control)) {
      ctrlKeyOn=true;
      e->ignore();
      return;
    }

    if (!colorTipShowed) {
      if ((e->state() & Qt::ShiftButton) && (e->key() == Qt::Key_Control) ||
          (e->state() & Qt::ControlButton) && (e->key() == Qt::Key_Shift)) {
        colorTipShowed=true;
        ctrlKeyOn=false;
        setCursor(cross_cursor);
        displayColorTip();
        e->ignore();
        return;
      }
    }



    switch (e->key()) {
      case Qt::Key_Left:
        dist.setX(-1);
      break;
      case Qt::Key_Right:
        dist.setX(1);
      break;
      case Qt::Key_Up:
        dist.setY(-1);
      break;
      case Qt::Key_Down:
        dist.setY(1);
      break;
		  case Qt::Key_Equal:
				zoomIn();
      break;
		  case Qt::Key_Underscore:
				zoomOut();
      break;
      default:
        e->ignore();
        return;
    }
    if (e->state() & Qt::ShiftButton) {
        dist *= 10;
    }
    move(pos()+dist);
}

void Quax::help() {
#if QT_VERSION >= 300
#ifdef _OS_WIN32_
#else
  QString helpApp = "kfmclient";

  Q3Process *proc = new Q3Process(this);
  proc->addArgument(helpApp);
  proc->addArgument("openURL");
  proc->addArgument("help:quax");
  proc->closeStdin();
  if (!proc->start()) {
    QMessageBox::critical(this,QObject::tr("Help Error"),
            QObject::tr("<qml>There was an error executing <b>%1</b> application. "
               "Please check your KDE installation. You may try to open an <b>xterm</b> "
               "window and run <tt>man quax</tt> from there.</qml>").arg(helpApp),
            QMessageBox::Ok, Qt::NoButton, Qt::NoButton);
  }
#endif
#endif
}

void Quax::about() {
    QMessageBox::about(this,QObject::tr("About Quax %1").arg(VERSION "-" RELEASE),QObject::tr(
                           "<p><b>Quax</b> is a little magnifing tool for X. Quax homepage is "
                           "at <tt>&lt;http://www.ro.kde.org/quax/&gt;</tt>."
                           "</p><p>This is Quax version %1."
                           "</p><p>This program is released under GNU General Public License."
                           "<dl>"
                           "<dt><nobr>Claudiu Costin <tt>&lt;claudiuc@kde.org&gt;</tt></nobr></dt>"
                           "<dd>coding, documentation, packaging</dd>"
                           "<dt><nobr>Vlad Oancea <tt>&lt;xtrim@fx.ro&gt;</tt></nobr></dt>"
                           "<dd>Quax skin design</dd>"
                           "</dl></p>").arg(VERSION "-" RELEASE));
}

void Quax::aboutQt() {
    QMessageBox::aboutQt(this,QObject::tr("About Qt"));
}

void Quax::zoomIn() {
    if (zoom < ZOOM_SCALE_MAX) {
        menuzoom->setItemChecked(zoomid[zoom],false);
        zoom++;
        menuzoom->setItemChecked(zoomid[zoom],true);
    }
}

void Quax::zoomOut() {
    if (zoom > ZOOM_SCALE_MIN) {
        menuzoom->setItemChecked(zoomid[zoom],false);
        zoom--;
        menuzoom->setItemChecked(zoomid[zoom],true);
    }
}

void Quax::zoomTo(int pos) {
    for (int i=ZOOM_SCALE_MIN;i<=ZOOM_SCALE_MAX;i++)
        if (zoomid[i]==pos) {
            menuzoom->setItemChecked(zoomid[i],true);
            zoom=i;
        } else {
            menuzoom->setItemChecked(zoomid[i],false);
        }
}

void Quax::rotateNorthWest() {
    menulook->setItemChecked(lookid[look],false);
    look=1;
    menulook->setItemChecked(lookid[look],true);
    rotate(look);
}

void Quax::rotateSouthWest() {
    menulook->setItemChecked(lookid[look],false);
    look=0;
    menulook->setItemChecked(lookid[look],true);
    rotate(look);
}

void Quax::rotateNorthEast() {
    menulook->setItemChecked(lookid[look],false);
    look=2;
    menulook->setItemChecked(lookid[look],true);
    rotate(look);
}

void Quax::rotateSouthEast() {
    menulook->setItemChecked(lookid[look],false);
    look=3;
    menulook->setItemChecked(lookid[look],true);
    rotate(look);
}

void Quax::rotateLeft() {
    menulook->setItemChecked(lookid[look],false);
    if (look==0) {
        look=3;
    } else {
        look--;
    }
    menulook->setItemChecked(lookid[look],true);
    rotate(look);
}

void Quax::rotateRight() {
    menulook->setItemChecked(lookid[look],false);
    if (look==3) {
        look=0;
    } else {
        look++;
    }
    menulook->setItemChecked(lookid[look],true);
    rotate(look);
}

void Quax::rotate(int pos) {
    QMatrix m;
    m.rotate(pos*90.0);
    setBackgroundPixmap(pix.xForm(m));
    /// setMask(*(pix_alpha.xForm(m)).mask());
}


void Quax::copyColor(int id) {
  
  if (id == colorid[1] ) {
      qApp->clipboard()->setText(colorStringDecimal);
  } else if (id == colorid[2]) {
      // this get called when user press Ctrl+C
      //
      // I must call grabForPixel because it's posible that
      // this slot get executed even user doesn't right click
      // on Quax, so updateMenuColor is not yet called and 
      // variables may contain old or invalid color
      grabForPixel();
      qApp->clipboard()->setText(colorStringHexaLower);
  } else if (id == colorid[3] || id == 0) {
      // id=0 if user click when color tip is displayed
      qApp->clipboard()->setText(colorStringHexaUpper);
  } else {
      qWarning("id=%d: This must not happen",id);
  }
}

void Quax::updateMenuColor() {
  grabForPixel();
#if QT_VERSION > 300
  ///pixelColorIcon->clearGenerated();
#endif
  pixelColorIcon->reset(pix_cursor,QIcon::Small);

  menu->changeItem(colorid[0],QIcon(pix_cursor),QObject::tr("Copy Color"));
  menucolor->changeItem(colorid[1],colorStringDecimal);
  menucolor->changeItem(colorid[2],colorStringHexaLower);
  menucolor->changeItem(colorid[3],colorStringHexaUpper);
}
