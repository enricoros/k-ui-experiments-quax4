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

#include "quax.h"

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

#include <QApplication>
#include <QClipboard>
#include <QColor>
#include <QCursor>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QProcess>
#include <QToolTip>
#include <QUrl>
#include <QWheelEvent>


Quax::Quax()
  : QWidget(0, Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint)
  , m_colorStringDecimal("255, 255, 255")
  , m_colorStringHexaLower("#ffffff")
  , m_colorStringHexaUpper("#FFFFFF")
  , m_inDrag(false)
  , m_colorTipEnabled(false)
  , m_zoomLevel(ZOOM_SCALE_MIN)
  , m_lookAt(0)
{
    // customize looks
    //setAttribute(Qt::WA_NoSystemBackground);
    //setAttribute(Qt::WA_TranslucentBackground, true);
    setFixedSize(150, 150);
    setCursor(Qt::PointingHandCursor);
    setWindowIcon(QPixmap(icon_xpm));
    rotate(0);
    
    // build RMB popup menu
    m_menuZoom = new QMenu(tr("Zoom"), this);
    m_menuZoom->setIcon(QPixmap(viewmag_xpm));
    m_zoomGroup = new QActionGroup(this);
    for (int i = ZOOM_SCALE_MIN; i <= ZOOM_SCALE_MAX; i++) {
        QAction * zoomAction = m_menuZoom->addAction(tr("%1:1").arg(i), this, SLOT(slotZoomTo()));
        zoomAction->setCheckable(true);
        zoomAction->setProperty("level", i);
        zoomAction->setActionGroup(m_zoomGroup);
    }
    m_menuZoom->addSeparator();
    m_menuZoom->addAction(QPixmap(viewmagin_xpm), tr("Zoom In"), this, SLOT(slotZoomIn()), QKeySequence(tr("+","Zoom In")));
    m_menuZoom->addAction(QPixmap(viewmagout_xpm), tr("Zoom Out"), this, SLOT(slotZoomOut()), QKeySequence(tr("-","Zoom Out")));

    m_menuLook = new QMenu(tr("Look At"), this);
    m_menuLook->setIcon(QPixmap(look_xpm));
    m_rotGroup = new QActionGroup(this);
    QAction * r0 = m_menuLook->addAction(tr("North-West"), this, SLOT(slotRotate()), QKeySequence(tr("U", "North-West")));
    r0->setProperty("rotation", 0);
    r0->setActionGroup(m_rotGroup);
    QAction * r1 = m_menuLook->addAction(tr("South-West"), this, SLOT(slotRotate()), QKeySequence(tr("J", "South-West")));
    r1->setProperty("rotation", 1);
    r1->setActionGroup(m_rotGroup);
    QAction * r2 = m_menuLook->addAction(tr("North-East"), this, SLOT(slotRotate()), QKeySequence(tr("I", "North-East")));
    r2->setProperty("rotation", 2);
    r2->setActionGroup(m_rotGroup);
    QAction * r3 = m_menuLook->addAction(tr("South-Eash"), this, SLOT(slotRotate()), QKeySequence(tr("K", "South-East")));
    r3->setProperty("rotation", 3);
    r3->setActionGroup(m_rotGroup);
    m_menuLook->addSeparator();
    m_menuLook->addAction(QPixmap(left_xpm), tr("Rotate Left"), this, SLOT(slotRotateLeft()), QKeySequence(tr("L", "Rotate Left")));
    m_menuLook->addAction(QPixmap(right_xpm),tr("Rotate Right"), this, SLOT(slotRotateRight()), QKeySequence(tr("R", "Rotate Right")));

    m_menuColor = new QMenu(tr("Copy Color"), this);
    m_colorGroup = new QActionGroup(this);
    QAction * c0 = m_menuColor->addAction(m_colorStringDecimal, this, SLOT(slotColorToClipboard()));
    c0->setActionGroup(m_colorGroup);
    c0->setProperty("id", 0);
    QAction * c1 = m_menuColor->addAction(m_colorStringHexaLower, this, SLOT(slotColorToClipboard()));
    c1->setActionGroup(m_colorGroup);
    c1->setProperty("id", 1);
    QAction * c2 = m_menuColor->addAction(m_colorStringHexaUpper, this, SLOT(slotColorToClipboard()));
    c2->setActionGroup(m_colorGroup);
    c2->setProperty("id", 2);

    m_menu = new QMenu(this);
    m_menu->addMenu(m_menuZoom);
    m_menu->addMenu(m_menuLook);
    m_menu->addMenu(m_menuColor);
    m_menu->addAction(QPixmap(help_xpm), tr("Help"), this, SLOT(slotHelp()), QKeySequence(tr("H", "Help")));
    m_menu->addSeparator();
    m_menu->addAction(tr("About Quax"), this, SLOT(slotAbout()));
    m_menu->addAction(tr("About Qt"), this, SLOT(slotAboutQt()));
    m_menu->addSeparator();
    m_menu->addAction(QPixmap(exit_xpm),tr("Quit"), qApp, SLOT(quit()), QKeySequence(tr("Q", "Quit")));
    connect(m_menu, SIGNAL(aboutToShow()), this, SLOT(slotUpdateColorMenu()));

    m_zoomGroup->actions().first()->setChecked(true);
    m_rotGroup->actions().at(1)->setChecked(true);
    
    // set the color tooltip
    m_colorTip = new QTextBrowser;
    m_colorTip->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Tool | Qt::X11BypassWindowManagerHint);
    m_colorTip->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_colorTip->setFrameStyle(QFrame::Plain | QFrame::Box);
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
            slotRotateLeft();
        else
            slotRotateRight();
    } else {
        if (e->delta() > 0)
            slotZoomIn();
        else
            slotZoomOut();
    }
    e->accept();
    if (m_colorTipEnabled && !m_colorTip->isHidden())
        genColorTip();
}

void Quax::grab()
{
    int x, y, w, h;
    int grabX, grabY, grabW, grabH, ofsX, ofsY;

    w = SHAPE_INNER_WIDTH / m_zoomLevel;
    h = SHAPE_INNER_HEIGHT / m_zoomLevel;
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

    m_zoomPixmap = grabPix.transformed(QTransform::fromScale((double)m_zoomLevel,(double)m_zoomLevel));
    update();
}

void Quax::grabForPixel()
{
    // get the color under cursor
    QPixmap grabPix = QPixmap::grabWindow(QApplication::desktop()->winId(), QCursor::pos().x() - 1, QCursor::pos().y() - 1, 1, 1);
    QRgb rgb_color = grabPix.toImage().pixel(0, 0);
    m_colorStringDecimal = QString::number(qRed(rgb_color)) + ", " +
                           QString::number(qGreen(rgb_color)) + ", " +
                           QString::number(qBlue(rgb_color));
    m_colorStringHexaLower.sprintf("#%02x%02x%02x",qRed(rgb_color),qGreen(rgb_color),qBlue(rgb_color));
    m_colorStringHexaUpper = m_colorStringHexaLower.toUpper();

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
                slotColorToClipboard();
                setCursor(Qt::SizeAllCursor);
            } else {
                grabMouse(Qt::PointingHandCursor);
                m_inDrag = true;
                m_dragOffset = QCursor::pos() - QPoint(geometry().left(),geometry().top());
            }
            break;
        case Qt::RightButton:
            m_menu->popup(QCursor::pos());
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
        move(QCursor::pos() - m_dragOffset);
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
            slotZoomIn();
            break;
        case Qt::Key_R:
            slotRotateRight();
            break;
        case Qt::Key_Underscore:
        case Qt::Key_Minus:
            slotZoomOut();
            break;
        default:
            QWidget::keyPressEvent(e);
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
    QWidget::keyReleaseEvent(e);
}

void Quax::slotHelp()
{
    if (QDesktopServices::openUrl(QUrl("help:quax")))
        return;
    QMessageBox::warning(this, tr("Help Error"),
                          tr("<qml>There was an loading the help application."
                             "Please check your KDE installation. You may try to open an <b>xterm</b> "
                             "window and run <tt>man quax</tt> from there.</qml>"),
                          QMessageBox::Ok);
}

void Quax::slotAbout()
{
    QMessageBox::about(this, tr("About Quax %1").arg(qApp->applicationVersion()), tr(
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
            "</dl></p>").arg(qApp->applicationVersion()));
}

void Quax::slotAboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void Quax::slotZoomIn()
{
    if (m_zoomLevel < ZOOM_SCALE_MAX) {
        m_zoomLevel++;
        foreach (QAction * action, m_zoomGroup->actions())
            if (action->property("level").toInt() == m_zoomLevel)
                action->setChecked(true);
    }
}

void Quax::slotZoomOut()
{
    if (m_zoomLevel > ZOOM_SCALE_MIN) {
        m_zoomLevel--;
        foreach (QAction * action, m_zoomGroup->actions())
            if (action->property("level").toInt() == m_zoomLevel)
                action->setChecked(true);
    }
}

void Quax::slotZoomTo()
{
    QAction * action = static_cast<QAction *>(sender());
    m_zoomLevel = action->property("level").toInt();
}

void Quax::slotRotate()
{
    QAction * action = static_cast<QAction *>(sender());
    m_lookAt = action->property("rotation").toInt();
    rotate(m_lookAt);
}

void Quax::slotRotateLeft()
{
    m_lookAt = !m_lookAt ? 3 : m_lookAt - 1;
    m_rotGroup->actions().at(m_lookAt)->setChecked(true);
    rotate(m_lookAt);
}

void Quax::slotRotateRight()
{
    m_lookAt = m_lookAt == 3 ? 0 : m_lookAt + 1;
    m_rotGroup->actions().at(m_lookAt)->setChecked(true);
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

void Quax::slotColorToClipboard()
{
    QAction * action = dynamic_cast<QAction *>(sender());
    int id = action ? action->property("id").toInt() : 2;
    switch (id) {
        case 0:
            qApp->clipboard()->setText(m_colorStringDecimal);
            break;
        case 1:
            // this get called when user press Ctrl+C
            //
            // I must call grabForPixel because it's posible that
            // this slot get executed even user doesn't right click
            // on Quax, so slotUpdateColorMenu is not yet called and
            // variables may contain old or invalid color
            grabForPixel();
            qApp->clipboard()->setText(m_colorStringHexaLower);
            break;
        case 2:
            qApp->clipboard()->setText(m_colorStringHexaUpper);
            break;
    }
}

void Quax::slotUpdateColorMenu()
{
    grabForPixel();
    m_menuColor->setIcon(QIcon(m_colorPixmap));
    m_colorGroup->actions().at(0)->setText(m_colorStringDecimal);
    m_colorGroup->actions().at(1)->setText(m_colorStringHexaLower);
    m_colorGroup->actions().at(2)->setText(m_colorStringHexaUpper);
}
