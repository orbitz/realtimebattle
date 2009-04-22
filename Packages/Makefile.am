SUBDIRS = rpm debian freebsd

PACKAGES = rpm debian freebsd

packages: $(PACKAGES)

.PHONY: $(PACKAGES)

$(PACKAGES):
	cd $@ && $(MAKE) $@

