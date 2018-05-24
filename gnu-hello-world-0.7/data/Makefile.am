prefix = @prefix@
datadir = @datadir@
desktopdir = @datadir@/applications
appdatadir = @datadir@/appdata

desktop_file = gnu_hello_world.desktop
appdata_in_file = gnu_hello_world.appdata.xml.in

EXTRA_DIST = $(desktop_file) $(appdata_in_file)

desktop_DATA = $(desktop_file)
appdata_XML = $(appdata_in_file:.xml.in=.xml)

CLEANFILES = $(appdata_XML) $(desktop_DATA)
