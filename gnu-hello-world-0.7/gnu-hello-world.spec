Name:           gnu-hello-world
Version:        1.0
Release:        2%{?dist}
Summary:        An Example GTK+ Application

License:        GPLv3
URL:            http://majsterklepka.github.io/
Source0:        %{name}-%{version}.tar.gz


BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  gettext
BuildRequires:  desktop-file-utils
BuildRequires:  libappstream-glib
       

%description
An Example GTK+ Application


%prep
%autosetup

%build
%configure
%make_build


%install
rm -rf $RPM_BUILD_ROOT
%make_install
desktop-file-install --dir %{buildroot}%{_datadir}/applications %{_vpath_srcdir}/data/org.majsterklepka.hello.desktop
install -dm 755 %{buildroot}%{_metainfodir}
install -pm 644 %{_vpath_srcdir}/data/org.majsterklepka.hello.appdata.xml %{buildroot}%{_metainfodir}

%find_lang %{name}

%check
desktop-file-validate %{buildroot}/%{_datadir}/applications/*.desktop
appstream-util validate-relax --nonet %{buildroot}%{_metainfodir}/*.appdata.xml

%clean
rm -rf $RPM_BUILD_ROOT

%files 
%defattr(-,root,root,-)
%{_bindir}/hello 
%{_datadir}/applications/*.desktop
%{_metainfodir}/*.appdata.xml
%{_datadir}/icons/hicolor/scalable/apps/*.svg

%lang(en) %{_datadir}/locale/en/*/*.mo
%lang(pl) %{_datadir}/locale/pl/*/*.mo


%changelog
* Wed May 23 2018 Majster Klepka <48721262935pl@gmail.com>
- 1.0-1
