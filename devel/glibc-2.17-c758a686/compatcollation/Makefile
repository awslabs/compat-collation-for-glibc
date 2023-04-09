###############################
# Build compatcollation lib and so
###############################
.PHONY: clean install

COMPATPREFIX ?= $(error COMPATPREFIX is undefined)
BUILDROOT ?= $(error BUILDROOT is undefined)
BUILDDIR ?= $(error BUILDDIR is undefined)
INSTALLROOT ?= $(error INSTALLROOT is undefined)
GLIBCVERSION ?= $(error GLIBCVERSION is undefined)
GLIBCRELEASE ?= $(error GLIBCRELEASE is undefined)
COMPATCOLLATIONVERSION ?= $(error COMPATCOLLATIONVERSION is undefined)


all: $(BUILDDIR)/libcompatcollation.$(GLIBCVERSION)-$(GLIBCRELEASE).so

clean:
	rm -f $(BUILDDIR)/compatcollation/compatcollation.os $(BUILDDIR)/libccompat_pic.a $(BUILDDIR)/libccompat_pic.os $(BUILDDIR)/libcompatcollation.$(GLIBCVERSION)-$(GLIBCRELEASE).so

install:
	mkdir -p $(INSTALLROOT)$(COMPATPREFIX)/lib64/gconv-compatcollation$(COMPATCOLLATIONVERSION) $(INSTALLROOT)$(COMPATPREFIX)/lib/locale-compatcollation$(COMPATCOLLATIONVERSION) $(INSTALLROOT)$(COMPATPREFIX)/share/locale-compatcollation$(COMPATCOLLATIONVERSION) && \
	cp -r $(INSTALLROOT)/usr/lib64/gconv/* $(INSTALLROOT)$(COMPATPREFIX)/lib64/gconv-compatcollation$(COMPATCOLLATIONVERSION)/ && \
#	cp -r $(INSTALLROOT)/usr/lib/locale/C.utf8 $(INSTALLROOT)$(COMPATPREFIX)/lib/locale-compatcollation$(COMPATCOLLATIONVERSION)/ && \
	cp -r $(INSTALLROOT)/usr/lib/locale/locale* $(INSTALLROOT)$(COMPATPREFIX)/lib/locale-compatcollation$(COMPATCOLLATIONVERSION)/ && \
	cp -r $(INSTALLROOT)/usr/share/locale/* $(INSTALLROOT)$(COMPATPREFIX)/share/locale-compatcollation$(COMPATCOLLATIONVERSION)/ && \
	cp $(BUILDDIR)/libcompatcollation.$(GLIBCVERSION)-$(GLIBCRELEASE).so $(INSTALLROOT)$(COMPATPREFIX)/lib64/

$(BUILDDIR)/libcompatcollation.$(GLIBCVERSION)-$(GLIBCRELEASE).so:
	mkdir -p $(BUILDDIR)/compatcollation && \
	cd $(BUILDROOT)/compatcollation && \
	chmod 755 ./build-compatcollation.sh && \
	./build-compatcollation.sh | tee glibc-rpmbuild-compat.out

