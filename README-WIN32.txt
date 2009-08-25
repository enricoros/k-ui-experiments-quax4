
    Quax - A neat desktop zooming tool
  ======================================
  
  
  1) What is Quax?

    Quax is a  desktop zooming tool intended especially for web developers, but
  is very handy and friendly for newbies. There are few other tools for this
  task: XMag, KMag and KZoom. All of them are full featured applications 
  and take some time to figure how to use them. Quax is *very* easy to use.

    The only thing you need to know is to right click on Quax surface to get
  the menu.

    Web developers and graphics designer need fast access so they use shortcuts.
  
    For the moment there's no help file for Quax, but only a UNIX manual page.
  This can only be read if you install Cygnus environment (GNU/UNIX under
  MS Windows)


  2) How I'll install Quax?

    To compile Quax you need a working Qt 4.4+ setup on Microsoft Windows. You
    can create the Quax executable with both mingw-based setups and MSVC (tested
    with 2008) based ones.

    Just open the Qt Development Prompt, go to the Quax directory and type the usual
     qmake
     make
    commands to get quax.exe built!

    To install Quax copy quax.exe somewhere when %PATH% variable can reach it.
  For example %WINDIR%, where it can be C:\WINDOWS, C:\WINNT.  


  3) How to obtain Quax?
  
    Go to:  http://gitorious.org/quax/quax
    The old homepage was on:  http://www.ro.kde.org/quax
  

  4) Where I can report bugs and whishes?
  
    To the authors. See the AUTHORS file.
  

  5) Notes for WIN32 release
  
    a) There's no native Windows help file
    b) The compilation hasn't been tested after the port to Qt 4
