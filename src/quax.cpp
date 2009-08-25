//
//
// C++ Implementation: cpp
//
// Description: Quax application implementation
//
//
// Authors:  <claudiuc@kde.org>, (C) 2001, 2002, 2003
//           <enrico.ros@gmail.com>, (C) 2009
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

#include "quax.h"

#include <QToolTip>
#include <QColor>
#include <QCursor>
#include <QClipboard>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QTimerEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QPixmap>
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
  , m_inDrag(false)
  , m_colorTipEnabled(false)
  , m_zoom(ZOOM_SCALE_MIN)
  , m_lookAt(0)
{
    // customize looks
    //setAttribute(Qt::WA_NoSystemBackground);
    setFixedSize(150, 150);
    setCursor(Qt::PointingHandCursor);
    setIcon(QPixmap(icon_xpm));
    rotate(0);
    
    // build RMB popup menu
    menu = new QMenu(this);
    menuZoom = new QMenu(this);
    menuZoom->setCheckable(true);
    for (int i = ZOOM_SCALE_MIN; i <= ZOOM_SCALE_MAX; i++)
        zoomid[i]=menuZoom->insertItem(tr("1:%1").arg(i), this, SLOT(zoomTo(int)));
    menuZoom->addSeparator();
    menuZoom->insertItem(QPixmap(viewmagin_xpm),QObject::tr("Zoom In"), this, SLOT(zoomIn()),QKeySequence(QObject::tr("+","Zoom In")));
    menuZoom->insertItem(QPixmap(viewmagout_xpm),QObject::tr("Zoom Out"), this, SLOT(zoomOut()),QKeySequence(QObject::tr("-","Zoom Out")));

    menuLook = new QMenu(this);
    menuLook->setCheckable(true);
    lookid[1]=menuLook->insertItem(QObject::tr("North-West"), this, SLOT(rotateNorthWest()),QKeySequence(QObject::tr("U", "North-West")));
    lookid[0]=menuLook->insertItem(QObject::tr("South-West"), this, SLOT(rotateSouthWest()),QKeySequence(QObject::tr("J", "South-West")));
    lookid[2]=menuLook->insertItem(QObject::tr("North-East"), this, SLOT(rotateNorthEast()),QKeySequence(QObject::tr("I", "North-East")));
    lookid[3]=menuLook->insertItem(QObject::tr("South-East"), this, SLOT(rotateSouthEast()),QKeySequence(QObject::tr("K", "South-East")));
    menuLook->insertSeparator();
    menuLook->insertItem(QPixmap(left_xpm),QObject::tr("Rotate Left"), this, SLOT(rotateLeft()),QKeySequence(QObject::tr("L","Rotate Left")));
    menuLook->insertItem(QPixmap(right_xpm),QObject::tr("Rotate Right"), this, SLOT(rotateRight()),QKeySequence(QObject::tr("R","Rotate Right")));

    menuColor = new QMenu(this);
    colorid[1]=menuColor->insertItem(m_colorStringDecimal, this, SLOT(colorToClipboard(int)));
    colorid[2]=menuColor->insertItem(m_colorStringHexaLower, this, SLOT(colorToClipboard(int)),QKeySequence(QObject::tr("Ctrl+C","Copy Color")));
    colorid[3]=menuColor->insertItem(m_colorStringHexaUpper, this, SLOT(colorToClipboard(int)));

    menu->insertItem(QPixmap(viewmag_xpm),QObject::tr("Zoom"),menuZoom);
    menu->insertItem(QPixmap(look_xpm),QObject::tr("Look At"),menuLook);
    colorid[0]=menu->insertItem(m_colorPixmap,QObject::tr("Copy Color"),menuColor);
    menu->insertItem(QPixmap(help_xpm),QObject::tr("Help"), this, SLOT(help()),QKeySequence(QObject::tr("H","Help")));
    menu->insertSeparator();
    menu->insertItem(QObject::tr("About Quax"), this, SLOT(about()));
    menu->insertItem(QObject::tr("About Qt"), this, SLOT(aboutQt()));
    menu->insertSeparator();
    menu->insertItem(QPixmap(exit_xpm),QObject::tr("Quit"),qApp,SLOT(quit()),QKeySequence(QObject::tr("Q","Quit")));

    menuZoom->setItemChecked(zoomid[m_zoom],true);
    menuLook->setItemChecked(lookid[m_lookAt],true);
    connect(menu,SIGNAL(aboutToShow()), this, SLOT(updatemenuColor()));
    
    // set the color tooltip
    m_colorTip = new QTextBrowser;
    m_colorTip->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Tool | Qt::X11BypassWindowManagerHint);
    m_colorTip->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_colorTip->setFrameStyle(QFrame::Plain | QFrame::Box);
    m_colorTip->setTextFormat(Qt::RichText);
    m_colorTip->resize(120, 28);
    
    setMouseTracking(true);
    startTimer(UPDATE);
}


Quax::~Quax()
{
    delete m_colorTip;
}

void Quax::enterEvent(QEvent *)
{
    if (m_colorTipEnabled && m_colorTip->isHidden())
        genColorTip();
}

void Quax::leaveEvent(QEvent *)
{
    if (m_colorTipEnabled)
        m_colorTip->hide();
}

void Quax::paintEvent(QPaintEvent *)
{
    if (m_zoomPixmap.isNull())
        return;

    QPainter p(this);
    p.setClipping(true);
    p.setClipRegion(QRegion(SHAPE_INNER_X,SHAPE_INNER_Y,SHAPE_INNER_WIDTH,SHAPE_INNER_HEIGHT,QRegion::Ellipse));

    int x,y;
    if (m_zoomPixmap.width() > SHAPE_INNER_WIDTH)
        x=(m_zoomPixmap.width() - SHAPE_INNER_WIDTH)/2;
    else
        x=(SHAPE_INNER_WIDTH - m_zoomPixmap.width())/2;
    if (m_zoomPixmap.height() > SHAPE_INNER_HEIGHT)
        y=(m_zoomPixmap.height() - SHAPE_INNER_HEIGHT)/2;
    else
        y=(SHAPE_INNER_HEIGHT - m_zoomPixmap.height())/2;
    x += SHAPE_INNER_X;
    y += SHAPE_INNER_Y;
    p.drawPixmap(x, y, m_zoomPixmap, 0, 0, SHAPE_INNER_WIDTH, SHAPE_INNER_HEIGHT);
}

void Quax::timerEvent(QTimerEvent *)
{
    grab();
}

void Quax::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->delta() > 0)
            rotateLeft();
        else
            rotateRight();
    } else {
        if (e->delta() > 0)
            zoomIn();
        else
            zoomOut();
    }
    e->accept();
    if (m_colorTipEnabled && !m_colorTip->isHidden())
        genColorTip();
}

void Quax::grab()
{
    int x, y, w, h;
    int grabX, grabY, grabW, grabH, ofsX, ofsY;

    w = SHAPE_INNER_WIDTH / m_zoom;
    h = SHAPE_INNER_HEIGHT / m_zoom;
    switch (m_lookAt) {
        case 0: // South-West
        default:
            x = this->x()-w;
            y = this->y()+height();
            break;
        case 1: // North-West
            x = this->x()-w;
            y = this->y()-h;
            break;
        case 2: // North-East
            x = this->x()+width();
            y = this->y()-h;
            break;
        case 3: // South-East
            x = this->x()+width();
            y = this->y()+height();
            break;
    }

    // calculate grab surface to avoid garbage
    if (x < 0) {
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
    if (y < 0) {
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

    QPixmap grabPix(w, h);
    grabPix.fill(Qt::blue);
    if (grabW > 0 && grabH > 0) {
        QPixmap pix_temp = QPixmap::grabWindow(QApplication::desktop()->winId(), grabX, grabY, grabW, grabH);
        QPainter p(&grabPix);
        p.drawPixmap(ofsX,ofsY,pix_temp);
    }

    m_zoomPixmap = grabPix.transformed(QTransform::fromScale((double)m_zoom,(double)m_zoom));
    update();
}

void Quax::grabForPixel()
{
    // get the color under cursor
    QPixmap grabPix = QPixmap::grabWindow(QApplication::desktop()->winId(), QCursor::pos().x() - 1, QCursor::pos().y() - 1, 1, 1);
    QImage img_grab = grabPix.convertToImage();
    QRgb rgb_color = img_grab.pixel(0, 0);
    m_colorStringDecimal = QString::number(qRed(rgb_color)) + ", " +
                           QString::number(qGreen(rgb_color)) + ", " +
                           QString::number(qBlue(rgb_color));
    m_colorStringHexaLower.sprintf("#%02x%02x%02x",qRed(rgb_color),qGreen(rgb_color),qBlue(rgb_color));
    m_colorStringHexaUpper = m_colorStringHexaLower.upper();

    // make a little pixmap with grabbed color
    m_colorPixmap = grabPix.scaled(14, 14);
}

void Quax::genColorTip()
{
    grabForPixel();
    m_colorTip->document()->addResource(QTextDocument::ImageResource, QUrl("qrc:/pixel"), m_colorPixmap);
    m_colorTip->document()->setHtml("<html>&nbsp;<img height=\"14\" width=\"14\" src=\"qrc:/pixel\">&nbsp;<tt>" + m_colorStringHexaUpper + "</tt></html>");

    int x_pos, y_pos;
    int x_ofs = 7, y_ofs = 5;
    int x_margin = 2, y_margin = 2;
    //check if tooltip get outside screen area
    QRect  r = m_colorTip->rect();
    QPoint c = QCursor::pos();
    if (c.x()+r.width()+x_ofs+x_margin > qApp->desktop()->width())
        x_pos = c.x()-r.width()-x_ofs;
    else
        x_pos = c.x()+x_ofs;
    if (c.y()-r.height()-y_ofs-y_margin < 0)
        y_pos = c.y()+y_ofs;
    else
        y_pos = c.y()-r.height()-y_ofs;
    m_colorTip->move(x_pos,y_pos);
    m_colorTip->show();
}

void Quax::mousePressEvent(QMouseEvent *e)
{
    switch (e->button()) {
        case Qt::LeftButton:
            if (m_colorTipEnabled) {
                colorToClipboard(0);
                setCursor(Qt::SizeAllCursor);
            } else {
                grabMouse(Qt::PointingHandCursor);
                m_inDrag = true;
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

void Quax::mouseMoveEvent(QMouseEvent *)
{
    if (m_colorTipEnabled)
        genColorTip();
    if (m_inDrag && this == mouseGrabber())
        move(QCursor::pos() - dragOffset);
}

void Quax::mouseReleaseEvent(QMouseEvent *e)
{
    switch (e->button()) {
        case Qt::LeftButton:
            if (m_colorTipEnabled)
                setCursor(Qt::CrossCursor);
            else {
                releaseMouse();
                m_inDrag = false;
            }
            break;
        default:
            break;
    }
}

void Quax::keyPressEvent(QKeyEvent *e)
{
    if (!m_colorTipEnabled && e->modifiers() & Qt::ShiftModifier && e->modifiers() & Qt::ControlModifier) {
        e->ignore();
        m_colorTipEnabled = true;
        setCursor(Qt::CrossCursor);
        genColorTip();
        return;
    }

    QPoint dist(0, 0);
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
        case Qt::Key_Plus:
            zoomIn();
            break;
        case Qt::Key_Underscore:
        case Qt::Key_Minus:
            zoomOut();
            break;
        default:
            e->ignore();
            return;
    }
    if (e->modifiers() & Qt::ShiftModifier)
        dist *= 10;
    move(pos() + dist);
}

void Quax::keyReleaseEvent(QKeyEvent *e)
{
    if (m_colorTipEnabled) {
        if (e->key() == Qt::Key_Control || e->key() == Qt::Key_Shift) {
            m_colorTipEnabled = false;
            m_colorTip->hide();
            setCursor(Qt::PointingHandCursor);
        }
    }
}

void Quax::help()
{
    if (QDesktopServices::openUrl(QUrl("help:quax")))
        return;
    QMessageBox::warning(this, tr("Help Error"),
                          tr("<qml>There was an loading the help application."
                             "Please check your KDE installation. You may try to open an <b>xterm</b> "
                             "window and run <tt>man quax</tt> from there.</qml>"),
                          QMessageBox::Ok);
}

void Quax::about()
{
    QMessageBox::about(this, QObject::tr("About Quax %1").arg(QUAX_VERSION "-" QUAX_RELEASE), QObject::tr(
            "<p><b>Quax</b> is a little magnifing tool for X. Quax homepage is "
            "at <tt>&lt;http://www.ro.kde.org/quax/&gt;</tt>."
            "</p><p>This is Quax version %1."
            "</p><p>This program is released under GNU General Public License."
            "<dl>"
            "<dt><nobr>Claudiu Costin <tt>&lt;claudiuc@kde.org&gt;</tt></nobr></dt>"
            "<dd>coding, documentation, packaging</dd>"
            "<dt><nobr>Enrico Ros <tt>&lt;enrico.ros@gmail.com&gt;</tt></nobr></dt>"
            "<dd>porting to Qt4 and cleanups</dd>"
            "<dt><nobr>Vlad Oancea <tt>&lt;xtrim@fx.ro&gt;</tt></nobr></dt>"
            "<dd>Quax skin design</dd>"
            "</dl></p>").arg(QUAX_VERSION "-" QUAX_RELEASE));
}

void Quax::aboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void Quax::zoomIn()
{
    if (m_zoom >= ZOOM_SCALE_MAX)
        return;
    menuZoom->setItemChecked(zoomid[m_zoom], false);
    m_zoom++;
    menuZoom->setItemChecked(zoomid[m_zoom], true);
}

void Quax::zoomOut()
{
    if (m_zoom <= ZOOM_SCALE_MIN)
        return;
    menuZoom->setItemChecked(zoomid[m_zoom], false);
    m_zoom--;
    menuZoom->setItemChecked(zoomid[m_zoom], true);
}

void Quax::zoomTo(int pos)
{
    for (int i = ZOOM_SCALE_MIN; i <= ZOOM_SCALE_MAX; i++)
        if (zoomid[i]==pos) {
            menuZoom->setItemChecked(zoomid[i], true);
            m_zoom=i;
        } else {
            menuZoom->setItemChecked(zoomid[i], false);
        }
}

void Quax::rotateNorthWest()
{
    menuLook->setItemChecked(lookid[m_lookAt], false);
    m_lookAt = 1;
    menuLook->setItemChecked(lookid[m_lookAt], true);
    rotate(m_lookAt);
}

void Quax::rotateSouthWest()
{
    menuLook->setItemChecked(lookid[m_lookAt], false);
    m_lookAt = 0;
    menuLook->setItemChecked(lookid[m_lookAt], true);
    rotate(m_lookAt);
}

void Quax::rotateNorthEast()
{
    menuLook->setItemChecked(lookid[m_lookAt], false);
    m_lookAt = 2;
    menuLook->setItemChecked(lookid[m_lookAt], true);
    rotate(m_lookAt);
}

void Quax::rotateSouthEast()
{
    menuLook->setItemChecked(lookid[m_lookAt], false);
    m_lookAt = 3;
    menuLook->setItemChecked(lookid[m_lookAt], true);
    rotate(m_lookAt);
}

void Quax::rotateLeft()
{
    menuLook->setItemChecked(lookid[m_lookAt], false);
    if (m_lookAt == 0)
        m_lookAt = 3;
    else
        m_lookAt--;
    menuLook->setItemChecked(lookid[m_lookAt], true);
    rotate(m_lookAt);
}

void Quax::rotateRight()
{
    menuLook->setItemChecked(lookid[m_lookAt], false);
    if (m_lookAt == 3)
        m_lookAt = 0;
    else
        m_lookAt++;
    menuLook->setItemChecked(lookid[m_lookAt], true);
    rotate(m_lookAt);
}

void Quax::rotate(int pos)
{
    QTransform rot;
    rot.rotate(pos * 90.0);
    QPalette pal;
    pal.setBrush(QPalette::Window, QPixmap(mag_xpm).transformed(rot));
    setPalette(pal);
    setMask(QPixmap(mag_alpha_xpm).transformed(rot).mask());
}

void Quax::colorToClipboard(int id)
{
    if (id == colorid[1]) {
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
    } else
        qWarning("id=%d: This must not happen",id);
}

void Quax::updateMenuColor()
{
    grabForPixel();
    menu->changeItem(colorid[0], QIcon(m_colorPixmap), QObject::tr("Copy Color"));
    menuColor->changeItem(colorid[1], m_colorStringDecimal);
    menuColor->changeItem(colorid[2], m_colorStringHexaLower);
    menuColor->changeItem(colorid[3], m_colorStringHexaUpper);
}
