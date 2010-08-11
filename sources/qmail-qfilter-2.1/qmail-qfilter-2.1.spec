Name: qmail-qfilter
Summary: qmail-queue filter front end
Version: 2.1
Release: 1
Copyright: GPL
Group: Utilities/System
Source: http://untroubled.org/qmail-qfilter/qmail-qfilter-2.1.tar.gz
BuildRoot: %{_tmppath}/qmail-qfilter-root
URL: http://untroubled.org/qmail-qfilter/
Packager: Bruce Guenter <bruceg@em.ca>
BuildRequires: bglibs >= 1.017

%description
This program allows the body of a message to be filtered through a
series of filters before being passed to the real qmail-queue program,
and injected into the qmail queue.

%prep
%setup

%build
echo %{_bindir} >conf-bin
echo %{_mandir} >conf-man
echo gcc %{optflags} >conf-cc
echo gcc -s >conf-ld
make

%install
rm -fr %{buildroot}
make install install_prefix=%{buildroot}

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc COPYING NEWS README samples
%{_bindir}/qmail-qfilter
%{_mandir}/man1/*
