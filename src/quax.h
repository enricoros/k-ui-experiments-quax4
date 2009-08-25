//
//
// C++ Interface: h
//
// Description: Quax declarations
//
//
// Authors:  <claudiuc@kde.org>, (C) 2001, 2002, 2003
//           <enrico.ros@gmail.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __quax_h__
#define __quax_h__

#include <QBitmap>
#include <QEvent>
#include <QIcon>
#include <QImage>
#include <QMenu>
#include <QPixmap>
#include <QTextBrowser>
#include <QWidget>

#define ZOOM_SCALE_MIN 2
#define ZOOM_SCALE_MAX 8

#define SHAPE_WIDTH 150
#define SHAPE_HEIGHT 150
#define SHAPE_INNER_X 7
#define SHAPE_INNER_Y 6
#define SHAPE_INNER_WIDTH 136
#define SHAPE_INNER_HEIGHT 136

#define UPDATE 40

/**
 * Magnifing widget for desktop
 * 
 * @short Magnifing widget for desktop
 * @author Claudiu Costin <claudiuc@kde.org>
 * @version $Id: quax.h,v 1.9 2003/11/15 20:15:15 claudiuc Exp $
 */
class Quax : public QWidget
{
    Q_OBJECT    
    public:
        /**
         * Contruct Quax GUI and initialize variables to sane values.
         * It build popup menu, set the Quax widget pixmap mask, build custom
         * cursors and start grab timer.
         */
        Quax();
        /**
         * Destroy Quax instance
         */
        ~Quax();
    
    protected:
        /**
         * Show color tooltip when mouse enter Quax area.
         */
        void enterEvent(QEvent *e);
        /**
         * Hide color tooltip if shown.
         */
        void leaveEvent(QEvent *e);
        /**
         * If right button was pressed, show popup menu. If color tooltip is already
         * showed then "take a shot" of current pixel under cursor.
         */
        void mousePressEvent(QMouseEvent *e);
        /**
         * Executed when user release move the mouse. If the color tip is enabled,
         * then displayed color is updated as well the tooltip position.
         * If is @p m_inDrag is true, then Quax position is updated (dragged)
         * to that of mouse position.
         */
        void mouseMoveEvent(QMouseEvent *e);
        /**
         * Executed when user release one mouse button.
         */
        void mouseReleaseEvent(QMouseEvent *e);
        /**
         * Get the modifier keys status and display color tooltip if Shift and Ctrl
         * are both pressed down. If cursor keys are pressed, then adjust Quax
         * position by one pixel or by ten pixels if Shift is down.
         */
        void keyPressEvent(QKeyEvent *e);
        /**
         * Hide color tooltip if one of Shift and Ctrl key modifiers is released.
         */
        void keyReleaseEvent(QKeyEvent *e);
        /**
         * Take the grabbed @p pix_zoom pixmap and scale it. Then it's painted
         * on Quax following an circle region for clipping.
         */
        void paintEvent(QPaintEvent *e);
        /**
         * Grab a screen region at every 40 miliseconds executing @ref grab().
         */
        void timerEvent(QTimerEvent *e);
        /**
         * If mouse wheel is dragged, then zoom scale in or out. When Ctrl key
         * modifier is pressed, Quax is rotated left or right
         */
        void wheelEvent(QWheelEvent *e);

    private:
        QString m_colorStringDecimal,   ///< the current color under mouse as decimals comma seperated
                m_colorStringHexaLower, ///< the current color under mouse as web RGB with lower hexadecimals
                m_colorStringHexaUpper; ///< the current color under mouse as web RGB with upper hexadecimals
        QMenu *m_menu,      ///< The main menu of Quax
              *m_menuZoom,  ///< Menu for zoom levels and zoom in and zoom out items
              *m_menuLook,  ///< The "Look at" menu
              *m_menuColor; ///< Menu for coying textual representation into clipboard
        QActionGroup *m_zoomGroup,
                     *m_rotGroup,
                     *m_colorGroup;
        QTextBrowser *m_colorTip; ///< The tool tip for displaying current color under mouse
        QPixmap m_zoomPixmap,  ///< The grabbed image zoomend and clipped
                m_colorPixmap; ///< The icon used in color menu item and color tooltip
        bool m_inDrag,         ///< if true, the user is dragging the Quax
             m_colorTipEnabled;///< if true, the color tooltip is displayed
        int m_zoomLevel, ///< current zoom level
            m_lookAt;    ///< current quadrant direction where Quax "look at"
        QPoint m_dragOffset;   ///< the offset between Quax position and new mouse position

        /**
         * It grabs the desktop area, scale it, clip it with circle region,
         * and then paint on Quax area.
         */
        void grab();
        /**
         * Show color tooltip and move the QLabel when mouse position change. Color
         * is updated runnning @ref grabForPixel(). If tooltip will be displayed
         * clipped by screen margins, then it will be repositioned before moving.
         */
        void genColorTip();
        /**
         * Grab exactly one pixel under mouse cursor to show it in color tooltip.
         */
        void grabForPixel();

    protected Q_SLOTS:
        /**
         * Display manual page or open help center according with underling desktop environment.
         */
        void slotHelp();
        /**
         * Open Quax "about" messagebox with author list and some other
         * usefull information (copyright, version).
         */
        void slotAbout();
        /**
         * Open Trolltech standard "about" messagebox.
         */
        void slotAboutQt();
        /**
         * Scale up grabed desktop area if not at maximum zoom value.
         */
        void slotZoomIn();
        /**
         * Scale down grabed desktop area if not at minimum zoom value.
         */
        void slotZoomOut();
        /**
         * Zoom grabed desktop area to the @p pos level.
         * @param pos represent position into zoom menu, as well the zoom level
         */
        void slotZoomTo();
        /**
         * Move the Quax point of desktop area grab to the North-West direction.
         * It make use of @ref Quax::rotate
         */
        void slotRotate();
        /**
         * Rotate Quax point of desktop area grab to the left (counter-clockwise).
         */
        void slotRotateLeft();
        /**
         * Rotate Quax point of desktop area grab to the right (clockwise).
         */
        void slotRotateRight();
        /**
         * Rotate Quax point of desktop area to the @p pos quadrant.
         * @param pos quadrant number (0..3)
         */
        void rotate(int pos);
        /**
         * Copy the last color under mouse which was saved on @p colorStringDecimal,
         * @p colorStringHexaLower and @p colorStringHexaUpper into system
         * clipboard. For the menu item which handle "Ctrl+C" accelerator, it call
         * @ref grabForPixel() prior to cliboard operation.
         */
        void slotColorToClipboard();
        /**
         * Build the menu left pixmap from the grabed color under mouse. This is slot is
         * called from QPopupMenu::aboutToShow signal.
         */
        void slotUpdateColorMenu();
};

#endif
