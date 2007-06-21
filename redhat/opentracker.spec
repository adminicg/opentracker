Summary:	An open architecture for reconfigurable tracking based on XML
Name:		opentracker
Version:	2.0.0
Release:	%(date '+%Y%m%d')
License:	LGPL
Group:		Development/Tools
Source:		%{name}-%{version}.tar.bz2
URL:		http://studierstube.icg.tugraz.at/opentracker
Vendor:		Institute for Computer Graphics and Vision, Graz University of Technology, Austria
Packager:	Institute for Computer Graphics and Vision, Graz University of Technology, Austria
Prefix:		/usr
BuildRoot: 	%{_tmppath}/buildroot-%{name}-%{version}
Requires:	ACE >= 5.5.6 Xerces-c boost ncurses ARToolKitPlus
BuildRequires:	ICGBuilder ACE-devel >= 5.5.6 Xerces-c-devel boost-devel ncurses-devel ARToolKitPlus-devel

%description
OpenTracker is developed to be generic solution to the different tasks
involved in tracking input devices and processing tracking data for virtual
environments. It provides an open software architecture based on a highly
modular design and a configuration syntax based on XML, thus taking full
advantage of this new technology. OpenTracker is a first attempt towards a
"write once, track anywhere" approach to virtual reality application
development.

%prep
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT
%setup
sed -i 's/curses/ncurses/' SConstruct

%build
%ifarch x86_64
export CFLAGS=-DUSE_64_BIT
export CXXFLAGS=-DUSE_64_BIT
%endif
scons

%install
scons --cache-disable DESTDIR=$RPM_BUILD_ROOT PREFIX=%{_prefix} LIBDIR=%{_libdir} install

%clean
scons -c
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_libdir}/*.so*
%{_bindir}/*

%package devel
Summary:	Open Tracker header and include files
Group:		Development/Libraries
Requires:	%{name} = %{version} ICGBuilder ACE-devel Xerces-c-devel boost-devel ARToolKitPlus-devel

%description devel
This package contains header files and include files that are needed for development using the OpenTracker library.

%files devel
%defattr(-,root,root)
%{_libdir}/pkgconfig/*
%{_prefix}/include/OpenTracker
