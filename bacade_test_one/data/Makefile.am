prefix = @prefix@
datadir = @datadir@

desktopdir = @datadir@/applications
metainfodir = @datadir@/metainfo
icondir = $(datadir)/icons/hicolor/scalable/apps

desktop_file = majsterklepka-bacade.desktop
metainfo_file = org.majsterklepka.bacade.appdata.xml
metainfo_template = $(top_srcdir)/data/bacade-template.appdata.xml 
desktop_template = $(top_srcdir)/data/bacade-template.desktop
icon = $(top_srcdir)/data/majsterklepka-bacade.svg

EXTRA_DIST = $(metainfo_template) $(desktop_template) $(icon)

icon_DATA = $(icon)

.desktop-build: $(desktop_template)
	$(MSGFMT) --desktop -d $(top_srcdir)/po --template=$(desktop_template) -o $(desktop_file);
	$(MKDIR_P) "$(DESTDIR)$(desktopdir)" || exit 1;
	$(INSTALL_DATA) $(abs_builddir)/$(desktop_file) $(DESTDIR)$(desktopdir)/$(desktop_file);
	
.appdata.xml-build: $(metainfo_template)
	$(MSGFMT)  --xml --template=$(metainfo_template) -d $(top_srcdir)/po -o $(metainfo_file);
	$(MKDIR_P) "$(DESTDIR)$(metainfodir)" || exit 1;
	$(INSTALL_DATA) $(abs_builddir)/$(metainfo_file) $(DESTDIR)$(metainfodir)/$(metainfo_file);

.remove-desktop-file: 
	@$(NORMAL_UNINSTALL) 
	test -n "$(desktop_file)" || exit 0; 
	cd "$(DESTDIR)$(desktopdir)" && rm -f $(desktop_file);

.remove-metainfo-file: 
	@$(NORMAL_UNINSTALL)
	test -n "$(metainfo_file)" || exit 0; 
	cd "$(DESTDIR)$(metainfodir)" && rm -f $(metainfo_file);


install-data-local: .desktop-build .appdata.xml-build

uninstall-local: .remove-desktop-file .remove-metainfo-file


CLEANFILES = $(metainfo_file) $(desktop_file)




