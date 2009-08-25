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
#include <QMenu>
#include <QMouseEvent>
#include <QEvent>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QDesktopWidget>


Quax::Quax()
  : QWidget(0,0,Qt::WStyle_Customize|Qt::WStyle_NoBorder|Qt::WType_TopLevel)
  , m_colorStringDecimal("255, 255, 255")
  , m_colorStringHexaLower("#ffffff")
  , m_colorStringHexaUpper("#FFFFFF")
  , m_pixelColorIcon(new QIcon)
  , m_inDrag(false)
  , m_colorTipShowed(false)
  , m_cursorOverQuax(false)
  , m_ctrlKeyOn(false)
  , m_zoom(ZOOM_SCALE_MIN)
  , m_lookAt(0)
{
    // customize looks
    //setAttribute(Qt::WA_NoSystemBackground);
    setFixedSize(150, 150);
    setCursor(Qt::PointingHandCursor);
    setIcon(QPixmap(icon_xpm));

    // set the background image and shape
    pix = QPixmap(mag_xpm);
    QPalette pal;
    pal.setBrush(QPalette::Window, pix);
    setAutoFillBackground(true);
    setPalette(pal);
    pix_alpha = QPixmap(mag_alpha_xpm);
    setMask(pix_alpha.mask());

    pix_cursor.resize(14,14);
    
    // build RMB popup menu
    menu = new QMenu(this);
    menuZoom = new QMenu(this);
    menuZoom->setCheckable(true);
    for (int i = ZOOM_SCALE_MIN; i <= ZOOM_SCALE_MAX; i++)
        zoomid[i]=menuZoom->insertItem(tr("1:%1").arg(i), this, SLOT(zoomTo(int)));
    menuZoom->addSeparator();
#if QT_VERSION >= 300
    menuZoom->insertItem(QPixmap(viewmagin_xpm),QObject::tr("Zoom In"),this,SLOT(zoomIn()),QKeySequence(QObject::tr("+","Zoom In")));
    menuZoom->insertItem(QPixmap(viewmagout_xpm),QObject::tr("Zoom Out"),this,SLOT(zoomOut()),QKeySequence(QObject::tr("-","Zoom Out")));
#else
    menuZoom->insertItem(QPixmap(viewmagin_xpm),QObject::tr("Zoom In"),this,SLOT(zoomIn()),Q3Accel::stringToKey(QObject::tr("+","Zoom In")));
    menuZoom->insertItem(QPixmap(viewmagout_xpm),QObject::tr("Zoom Out"),this,SLOT(zoomOut()),Q3Accel::stringToKey(QObject::tr("-","Zoom Out")));
#endif

    menuLook = new QMenu(this);
    menuLook->setCheckable(true);
#if QT_VERSION >= 300
    lookid[1]=menuLook->insertItem(QObject::tr("North-West"),this,SLOT(rotateNorthWest()),QKeySequence(QObject::tr("U","North-West")));
    lookid[0]=menuLook->insertItem(QObject::tr("South-West"),this,SLOT(rotateSouthWest()),QKeySequence(QObject::tr("J","South-West")));
    lookid[2]=menuLook->insertItem(QObject::tr("North-East"),this,SLOT(rotateNorthEast()),QKeySequence(QObject::tr("I","North-East")));
    lookid[3]=menuLook->insertItem(QObject::tr("South-East"),this,SLOT(rotateSouthEast()),QKeySequence(QObject::tr("K","South-East")));
#else
    lookid[1]=menuLook->insertItem(QObject::tr("North-West"),this,SLOT(rotateNorthWest()),Q3Accel::stringToKey(QObject::tr("u","North-West")));
    lookid[0]=menuLook->insertItem(QObject::tr("South-West"),this,SLOT(rotateSouthWest()),Q3Accel::stringToKey(QObject::tr("j","South-West")));
    lookid[2]=menuLook->insertItem(QObject::tr("North-East"),this,SLOT(rotateNorthEast()),Q3Accel::stringToKey(QObject::tr("i","North-East")));
    lookid[3]=menuLook->insertItem(QObject::tr("South-East"),this,SLOT(rotateSouthEast()),Q3Accel::stringToKey(QObject::tr("k","South-East")));
#endif
    menuLook->insertSeparator();
#if QT_VERSION >= 300
    menuLook->insertItem(QPixmap(left_xpm),QObject::tr("Rotate Left"),this,SLOT(rotateLeft()),QKeySequence(QObject::tr("L","Rotate Left")));
    menuLook->insertItem(QPixmap(right_xpm),QObject::tr("Rotate Right"),this,SLOT(rotateRight()),QKeySequence(QObject::tr("R","Rotate Right")));
#else
    menuLook->insertItem(QPixmap(left_xpm),QObject::tr("Rotate Left"),this,SLOT(rotateLeft()),Q3Accel::stringToKey(QObject::tr("l","Rotate Left")));
    menuLook->insertItem(QPixmap(right_xpm),QObject::tr("Rotate Right"),this,SLOT(rotateRight()),Q3Accel::stringToKey(QObject::tr("r","Rotate Right")));
#endif

    menuColor = new QMenu(this);
    colorid[1]=menuColor->insertItem(m_colorStringDecimal,this,SLOT(copyColor(int)));
#if QT_VERSION >= 300
    colorid[2]=menuColor->insertItem(m_colorStringHexaLower,this,SLOT(copyColor(int)),QKeySequence(QObject::tr("Ctrl+C","Copy Color")));
#else
    colorid[2]=menuColor->insertItem(m_colorStringHexaLower,this,SLOT(copyColor(int)),Q3Accel::stringToKey(QObject::tr("Ctrl+c","Copy Color")));
#endif
    colorid[3]=menuColor->insertItem(m_colorStringHexaUpper,this,SLOT(copyColor(int)));

    menu->insertItem(QPixmap(viewmag_xpm),QObject::tr("Zoom"),menuZoom);
    menu->insertItem(QPixmap(look_xpm),QObject::tr("m_lookAt At"),menuLook);
    colorid[0]=menu->insertItem(pix_cursor,QObject::tr("Copy Color"),menuColor);
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

    menuZoom->setItemChecked(zoomid[m_zoom],true);
    menuLook->setItemChecked(lookid[m_lookAt],true);
    connect(menu,SIGNAL(aboutToShow()),this,SLOT(updatemenuColor()));
    
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
    colorTip->setTextFormat(Qt::RichText);

    
    setMouseTracking(true);
    startTimer(UPDATE);
}


Quax::~Quax() {
    delete colorTip;
}


void Quax::enterEvent(QEvent *e) {
    e=e;

    m_cursorOverQuax=true;
    if (m_colorTipShowed && colorTip->isHidden()) {
        displayColorTip();
    }
}


void Quax::leaveEvent(QEvent *e) {
    e=e;

    m_cursorOverQuax=false;
    if (m_colorTipShowed) {
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
    if (m_ctrlKeyOn) {
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
    if (m_colorTipShowed && ! colorTip->isHidden()) {
        displayColorTip();
    }
}

void Quax::grab() {
    QPixmap pix_grab;
    int x, y, w, h;
    int grabX, grabY, grabW, grabH, ofsX, ofsY;

    w = SHAPE_INNER_WIDTH/m_zoom;
    h = SHAPE_INNER_HEIGHT/m_zoom;
    switch (m_lookAt) {
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
        qWarning("m_lookAt=%d: This must not happen",m_lookAt);
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
    m.scale((double)m_zoom,(double)m_zoom);
    pix_zoom=pix_grab.xForm(m);
    repaint(false);
}

void Quax::grabForPixel() {
    // get the color under cursor
    QPixmap pix_grab = QPixmap::grabWindow(QApplication::desktop()->winId(),QCursor::pos().x()-1,QCursor::pos().y()-1,1,1);
    QImage img_grab = pix_grab.convertToImage();
    QRgb rgb_color = img_grab.pixel(0,0);
    m_colorStringDecimal = QString::number(qRed(rgb_color)) + ", " +
                         QString::number(qGreen(rgb_color)) + ", " +
                         QString::number(qBlue(rgb_color));
    m_colorStringHexaLower.sprintf("#%02x%02x%02x",qRed(rgb_color),qGreen(rgb_color),qBlue(rgb_color));
    m_colorStringHexaUpper = m_colorStringHexaLower.upper();

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
  colorTip->setText("<qml>&nbsp;<img height=\"14\" width=\"14\" src=\"pixel\">&nbsp;<tt>" + m_colorStringHexaUpper + "</tt></qml>");
  
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
        if (m_colorTipShowed) {
            copyColor(0);
            setCursor(Qt::SizeAllCursor);
        } else {
            grabMouse(Qt::PointingHandCursor);
            m_inDrag=true;
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
        if (m_colorTipShowed) {
            setCursor(Qt::CrossCursor);
        } else {
            releaseMouse();
            m_inDrag=false;
        }
        break;
    default:
        break;
    }
}

void Quax::mouseMoveEvent(QMouseEvent *e) {
    e=e;
    
    if (m_colorTipShowed && m_cursorOverQuax) {
        displayColorTip();
    }
    if (!m_inDrag || this != mouseGrabber()) return;
    move(QCursor::pos()-dragOffset);
}


void Quax::keyReleaseEvent(QKeyEvent *e) {
    if (m_ctrlKeyOn) {
        if (!(e->state() & Qt::ShiftButton) && (e->key() == Qt::Key_Control)) {
            m_ctrlKeyOn=false;
        }
    }
    
    if (m_colorTipShowed) {
        if (e->key() == Qt::Key_Control ||e->key() == Qt::Key_Shift) {
            m_colorTipShowed=false;
            setCursor(Qt::PointingHandCursor);
            colorTip->hide();
        }
    }
}

void Quax::keyPressEvent(QKeyEvent *e) {
    QPoint dist(0,0);

    if (!(e->state() & Qt::ShiftButton) && (e->key() == Qt::Key_Control)) {
        m_ctrlKeyOn=true;
        e->ignore();
        return;
    }

    if (!m_colorTipShowed) {
        if (((e->state() & Qt::ShiftButton) && (e->key() == Qt::Key_Control)) ||
            ((e->state() & Qt::ControlButton) && (e->key() == Qt::Key_Shift))) {
            m_colorTipShowed=true;
            m_ctrlKeyOn=false;
            setCursor(Qt::CrossCursor);
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
    QMessageBox::about(this,QObject::tr("About Quax %1").arg(QUAX_VERSION "-" QUAX_RELEASE),QObject::tr(
            "<p><b>Quax</b> is a little magnifing tool for X. Quax homepage is "
            "at <tt>&lt;http://www.ro.kde.org/quax/&gt;</tt>."
            "</p><p>This is Quax version %1."
            "</p><p>This program is released under GNU General Public License."
            "<dl>"
            "<dt><nobr>Claudiu Costin <tt>&lt;claudiuc@kde.org&gt;</tt></nobr></dt>"
            "<dd>coding, documentation, packaging</dd>"
            "<dt><nobr>Vlad Oancea <tt>&lt;xtrim@fx.ro&gt;</tt></nobr></dt>"
            "<dd>Quax skin design</dd>"
            "</dl></p>").arg(QUAX_VERSION "-" QUAX_RELEASE));
}

void Quax::aboutQt() {
    QMessageBox::aboutQt(this,QObject::tr("About Qt"));
}

void Quax::zoomIn() {
    if (m_zoom < ZOOM_SCALE_MAX) {
        menuZoom->setItemChecked(zoomid[m_zoom],false);
        m_zoom++;
        menuZoom->setItemChecked(zoomid[m_zoom],true);
    }
}

void Quax::zoomOut() {
    if (m_zoom > ZOOM_SCALE_MIN) {
        menuZoom->setItemChecked(zoomid[m_zoom],false);
        m_zoom--;
        menuZoom->setItemChecked(zoomid[m_zoom],true);
    }
}

void Quax::zoomTo(int pos) {
    for (int i = ZOOM_SCALE_MIN; i <= ZOOM_SCALE_MAX; i++)
        if (zoomid[i]==pos) {
            menuZoom->setItemChecked(zoomid[i],true);
            m_zoom=i;
        } else {
            menuZoom->setItemChecked(zoomid[i],false);
        }
}

void Quax::rotateNorthWest() {
    menuLook->setItemChecked(lookid[m_lookAt],false);
    m_lookAt=1;
    menuLook->setItemChecked(lookid[m_lookAt],true);
    rotate(m_lookAt);
}

void Quax::rotateSouthWest() {
    menuLook->setItemChecked(lookid[m_lookAt],false);
    m_lookAt=0;
    menuLook->setItemChecked(lookid[m_lookAt],true);
    rotate(m_lookAt);
}

void Quax::rotateNorthEast() {
    menuLook->setItemChecked(lookid[m_lookAt],false);
    m_lookAt=2;
    menuLook->setItemChecked(lookid[m_lookAt],true);
    rotate(m_lookAt);
}

void Quax::rotateSouthEast() {
    menuLook->setItemChecked(lookid[m_lookAt],false);
    m_lookAt=3;
    menuLook->setItemChecked(lookid[m_lookAt],true);
    rotate(m_lookAt);
}

void Quax::rotateLeft() {
    menuLook->setItemChecked(lookid[m_lookAt],false);
    if (m_lookAt==0) {
        m_lookAt=3;
    } else {
        m_lookAt--;
    }
    menuLook->setItemChecked(lookid[m_lookAt],true);
    rotate(m_lookAt);
}

void Quax::rotateRight() {
    menuLook->setItemChecked(lookid[m_lookAt],false);
    if (m_lookAt==3) {
        m_lookAt=0;
    } else {
        m_lookAt++;
    }
    menuLook->setItemChecked(lookid[m_lookAt],true);
    rotate(m_lookAt);
}

void Quax::rotate(int pos) {
    QMatrix m;
    m.rotate(pos*90.0);
    setBackgroundPixmap(pix.xForm(m));
    /// setMask(*(pix_alpha.xForm(m)).mask());
}


void Quax::copyColor(int id) {

    if (id == colorid[1] ) {
        qApp->clipboard()->setText(m_colorStringDecimal);
    } else if (id == colorid[2]) {
        // this get called when user press Ctrl+C
        //
        // I must call grabForPixel because it's posible that
        // this slot get executed even user doesn't right click
        // on Quax, so updatemenuColor is not yet called and
        // variables may contain old or invalid color
        grabForPixel();
        qApp->clipboard()->setText(m_colorStringHexaLower);
    } else if (id == colorid[3] || id == 0) {
        // id=0 if user click when color tip is displayed
        qApp->clipboard()->setText(m_colorStringHexaUpper);
    } else {
        qWarning("id=%d: This must not happen",id);
    }
}

void Quax::updateMenuColor() {
    grabForPixel();
#if QT_VERSION > 300
    ///m_pixelColorIcon->clearGenerated();
#endif
    m_pixelColorIcon->reset(pix_cursor,QIcon::Small);

    menu->changeItem(colorid[0],QIcon(pix_cursor),QObject::tr("Copy Color"));
    menuColor->changeItem(colorid[1],m_colorStringDecimal);
    menuColor->changeItem(colorid[2],m_colorStringHexaLower);
    menuColor->changeItem(colorid[3],m_colorStringHexaUpper);
}
