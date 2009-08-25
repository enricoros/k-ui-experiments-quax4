//
//
// C++ Interface: h
//
// Description: Quax declarations
//
//
// Author:  <claudiuc@kde.org>, (C) 2001, 2002, 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef QUAX_H
#define QUAX_H

#include "../config.h"

#include <qglobal.h>

#include <qcursor.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qimage.h>
#include <qpainter.h>
#include <qmap.h>

#include <qicon.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qmatrix.h>
#include <q3popupmenu.h>
#include <qapplication.h>
//Added by qt3to4:
#include <QWheelEvent>
#include <QEvent>
#include <QMouseEvent>
#include <QTimerEvent>
#include <QKeyEvent>
#include <QPaintEvent>

#define PIX_CURSOR_SCALE 10
#define ZOOM_SCALE_MIN 2
#define ZOOM_SCALE_MAX 5

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
   * Show color tooltip when mouse enter Quax area, and set 
   * the @p cursorOverQuax flag.
   */
  void enterEvent(QEvent *e);
  /**
   * Hide color tooltip if shown, and set the @p cursorOverQuax flag.
   */
  void leaveEvent(QEvent *e);
  /**
   * If right button was pressed, show popup menu. If color tooltip is already
   * showed then "take a shot" of current pixel under cursor.
   */
  void mousePressEvent(QMouseEvent *e);
  /**
    * Executed when user release one mouse button.
    */
  void mouseReleaseEvent(QMouseEvent *e);
  /**
    * Executed when user release move the mouse. If the color tip is enabled,
    * then displayed color is updated as well the tooltip position.
    * If is @p inDrag is true, then Quax position is updated (dragged)
    * to that of mouse position.
    */
  void mouseMoveEvent(QMouseEvent *e);
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
  QString colorStringDecimal,   ///< the current color under mouse as decimals comma seperated
          colorStringHexaLower, ///< the current color under mouse as web RGB with lower hexadecimals
          colorStringHexaUpper; ///< the current color under mouse as web RGB with upper hexadecimals
  QIcon *pixelColorIcon; ///< The icon set which is used in QML content from @ref colorTip
  QLabel *colorTip; ///< The tool tip for displaying current color under mouse
  Q3PopupMenu *menu, ///< The main menu of Quax
             *menuzoom, ///< Menu for zoom levels and zoom in and zoom out items
             *menulook,  ///< The "Look at" menu
             *menucolor; ///< Menu for coying textual representation into clipboard
  QPixmap pix,        ///< Quax skin image
          pix_alpha,  ///< Quax alpha mask for skin image
          pix_zoom,   ///< The grabbed image zoomend and clipped
          pix_cursor; ///< The icon used in color menu item and color tooltip
	QCursor hand_cursor,  ///< normal mouse cursor when mouse pointer is over Quax
          cross_cursor, ///< cursor when color tooltip is displayed (cross)
          copy_cursor;  ///< cursor when user "take a shot" of current color
  bool inDrag,         ///< if true, the user is dragging the Quax
       colorTipShowed, ///< if true, the color tooltip is displayed
       cursorOverQuax, ///< if true, mouse cursor is over Quax surface
       ctrlKeyOn;      ///< if true, the Ctrl key is pressed down
  QPoint dragOffset;   ///< the offset between Quax position and new mouse position
	QMap<int,int> zoomid,  ///< mapping between zoom level and @ref menuzoom menu item ids
                lookid,  ///< mapping between look direction and @ref menulook menu item ids
                colorid; ///< mapping between text color type and @ref menucolor menu item ids
  int zoom, ///< current zoom level
      look; ///< current quadrant direction where Quax "look at"

  /**
   * It grab desktop area, scale it, clip it with circle region, 
   * and then paint on Quax area.
   */
  void grab();
  /**
   * Show color tooltip and move the QLabel when mouse position change. Color
   * is updated runnning @ref grabForPixel(). If tooltip will be displayed
   * clipped by screen margins, then it will be repositioned before moving.
   */
  void displayColorTip();
  /**
   * Grab exactly one pixel under mouse cursor to show it in color tooltip.
   */
  void grabForPixel();

protected slots:
  /**
   * Display manual page or open help center according with underling desktop environment.
   */
	void help();
  /**
   * Open Quax "about" messagebox with author list and some other 
   * usefull information (copyright, version).
   */
	void about();
  /**
   * Open Trolltech standard "about" messagebox.
   */
	void aboutQt();
  /**
   * Scale up grabed desktop area if not at maximum zoom value.
   */
	void zoomIn();
  /**
   * Scale down grabed desktop area if not at minimum zoom value.
   */
	void zoomOut();
  /**
   * Zoom grabed desktop area to the @p pos level.
   * @param pos represent position into zoom menu, as well the zoom level
   */
	void zoomTo(int pos);
  /**
   * Move the Quax point of desktop area grab to the North-West direction.
   * It make use of @ref Quax::rotate
   */
	void rotateNorthWest();
  /**
   * Move the Quax point of desktop area grab to the South-West direction.
   */
	void rotateSouthWest();
  /**
   * Move the Quax point of desktop area grab to the North-East direction.
   */
	void rotateNorthEast();
  /**
   * Move the Quax point of desktop area grab to the South-East direction.
   */
	void rotateSouthEast();
  /**
   * Rotate Quax point of desktop area grab to the left (counter-clockwise).
   */
	void rotateLeft();
  /**
   * Rotate Quax point of desktop area grab to the right (clockwise).
   */
	void rotateRight();
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
   * @param id the menu identificator to get what color menu item was selected
   */
  void copyColor(int id);
  /**
   * Build the menu left pixmap from the grabed color under mouse. This is slot is
   * called from QPopupMenu::aboutToShow signal.
   */
  void updateMenuColor();
};


#endif
