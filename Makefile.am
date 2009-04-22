SUBDIRS =  m4 intl include po src Packages Arenas Robots $(subdirs) Documentation

rtbextradir = $(RTB_DIR)
rtbextra_DATA = RealTimeBattle.xpm tournament.txt rtblogo1024.jpg  rtblogo1600.jpg  rtb_logo.jpg
EXTRA_DIST = config/config.rpath config/mkinstalldirs  config/config.rpath   $(rtbextra_DATA)

debian freebsd rpm:
	cd Packages/$@ && $(MAKE) $@

packages:
	cd Packages && $(MAKE) packages

ACLOCAL_AMFLAGS = -I m4 -I config
AUTOMAKE_OPTIONS=dist-bzip2 dist-zip subdir-objects
