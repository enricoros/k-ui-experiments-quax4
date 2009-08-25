%define name	quax
%define version 1.0
%define release 1
%define builddir $RPM_BUILD_DIR/%{name}-%{version}-%{release}
%define kdedir  /usr

Name:		%{name}
Version:	%{version}
Release:	%{release}
Vendor:		Claudiu Costin <claudiuc@kde.org>
Packager:	Claudiu Costin <claudiuc@kde.org>
URL:		http://www.ro.kde.org/quax/
Source:         %{name}-%{version}-%{release}.tar.gz
Group:		System/X11/Utilities
Copyright:      GPL
Provides:	quax
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}
Summary:	A neat desktop zooming tool

%description
Quax is a  desktop zooming tool intended especially for web developers, but
is very handy and friendly for newbies. There are few other tools for this
task: XMag, KMag and KZoom. All of them are full featured applications
and take some time to figure how to use them.
But Quax is *very* easy to use.


%prep
%setup -n %{name}-%{version}-%{release}


%build
./configure
PREFIX="%{_prefix}" make


%install
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf "$RPM_BUILD_ROOT"
PREFIX="%{_prefix}" INSTALL_ROOT="$RPM_BUILD_ROOT" make install 
mkdir -p $RPM_BUILD_ROOT/%{kdedir}/share/icons/hicolor/16x16/apps
mkdir -p $RPM_BUILD_ROOT/%{kdedir}/share/icons/hicolor/32x32/apps
mkdir -p $RPM_BUILD_ROOT/%{kdedir}/share/icons/hicolor/48x48/apps
mkdir -p $RPM_BUILD_ROOT/%{kdedir}/share/applnk/Graphics
cp -f src/quax-hi16.png $RPM_BUILD_ROOT/%{kdedir}/share/icons/hicolor/16x16/apps/quax.png
cp -f src/quax-hi32.png $RPM_BUILD_ROOT/%{kdedir}/share/icons/hicolor/32x32/apps/quax.png
cp -f src/quax-hi48.png $RPM_BUILD_ROOT/%{kdedir}/share/icons/hicolor/48x48/apps/quax.png
cp -f src/quax.desktop  $RPM_BUILD_ROOT/%{kdedir}/share/applnk/Graphics


%clean
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf "$RPM_BUILD_ROOT"


%files

%defattr(0755, root, root)
%{_prefix}/bin/*

%defattr(0644, root, root)
%{_mandir}/man1/*

%defattr(-, root, root)
%doc AUTHORS BUGS COPYING CREDITS ChangeLog Doxyfile INSTALL README TODO VERSION quax.lsm

%defattr(0644, root, root)
%{kdedir}/share/icons/hicolor/16x16/apps/*
%{kdedir}/share/icons/hicolor/32x32/apps/*
%{kdedir}/share/icons/hicolor/48x48/apps/*
%{kdedir}/share/applnk/Graphics/*


%changelog
* Sun Jun 08 2003 Claudiu Costin <claudiuc@kde.org>
- KDE integration, icons
- RPM customizations
- improved documentation
- color tool tip

* Sat May 25 2003 Claudiu Costin <claudiuc@kde.org>
- initial RPM

