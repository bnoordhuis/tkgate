## Process this file with automake to produce Makefile.in
#
#    Copyright (C) 1987-2007 by Jeffery P. Hansen
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

AUTOMAKE_OPTIONS = foreign

#
# List of directories that contain data files necessary for tkgate to run.
#
TKG_DATADIRS=scripts bindings doc gdf images locale povray primitives vlib vpd test

#
# Subdirectories in which more makefiles can be found 
#
SUBDIRS = src

#
# List of data files in this directory that should be included with a tkgate distribution.
#
dist_pkgdata_DATA=README README.verga COPYING INSTALL TODO site-preferences license.txt pkg-comment pkg-descr

#
# List of man pages for tkgate
#
dist_man_MANS = tkgate.1 verga.1 gmac.1

#
# List of header files that should not be automatically installed
#
noinst_HEADERS=options.h

#
# Update the release data for this compilation
#
release:
	echo 'char *release_date = "'`(LANG=; date "+%b %d %Y")`'";' > $(srcdir)/src/tkgate/release.c


dist-hook:
	@echo Packaging data files...
	@for d in $(TKG_DATADIRS); do \
	   cp -pR $(srcdir)/$$d $(distdir); \
	done

tests:
	cd test/verga;./runtests.sh

#
# Make sure the libexec directory has symbolic links to the 'tkgate' and 'verga' executables.
#
install-exec-hook:
	@echo Creating symbolic links in libexec...
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/libexec
	@for p in tkgate verga; do \
	   f=`echo "$$p" | sed 's,^.*/,,;$(transform);s/$$/$(EXEEXT)/'`; \
	   rm -f $(DESTDIR)$(pkgdatadir)/libexec/$$p; \
	   $(LN_S) `(cd $(DESTDIR)$(bindir);pwd)`/$$f $(DESTDIR)$(pkgdatadir)/libexec/$$p; \
	done
#
# Copy all the data files we need
#
install-data-hook:
	@echo Installing data files...
	@for d in $(TKG_DATADIRS); do \
	   cp -pR $(srcdir)/$$d $(DESTDIR)$(pkgdatadir); \
	done

uninstall-local:
	@echo Uninstalling data files...
	@for d in $(TKG_DATADIRS); do \
	   chmod -R u+w $(DESTDIR)$(pkgdatadir)/$$d; \
	   rm -rf $(DESTDIR)$(pkgdatadir)/$$d; \
	done
	@echo Uninstalling libexec links...
	chmod -R u+w $(DESTDIR)$(pkgdatadir)/libexec
	rm -f $(DESTDIR)$(pkgdatadir)/libexec/tkgate
	rm -f $(DESTDIR)$(pkgdatadir)/libexec/verga
	rmdir $(DESTDIR)$(pkgdatadir)/libexec
