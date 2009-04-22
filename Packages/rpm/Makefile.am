EXTRA_DIST = RealTimeBattle.spec.in

SPECDIR =	$(shell rpm --showrc | grep "^specdir" | sed "s/specdir[  ]*: *//")
SOURCEDIR =	$(shell rpm --showrc | grep "^sourcedir" | sed "s/sourcedir[  ]*: *//")
RPMDIR =	$(shell rpm --showrc | grep "^rpmdir" | sed "s/rpmdir[  ]*: *//")
BUILDARCH =	$(shell rpm --showrc | grep "^build arch" | sed "s/build arch[  ]*: *//")
SRCRPMDIR =	$(shell rpm --showrc | grep "^srcrpmdir" | sed "s/srcrpmdir[  ]*: *//")


.PHONY: rpm

rpm:
	echo "It seems as if this target rule is broken due to new versions of rpm. Please correct!"
	if test "x$(VERSION)" != "x"; then \
	  cp ../../../$(PACKAGE)-$(VERSION).tar.gz $(SOURCEDIR) && \
	  cp RealTimeBattle.spec $(SPECDIR)/RealTimeBattle-$(RTB_RPM_VERSION).spec && \
	  rpm -ba --clean --rmsource $(SPECDIR)/RealTimeBattle-$(RTB_RPM_VERSION).spec && \
	  cp $(RPMDIR)/$(BUILDARCH)/RealTimeBattle-$(RTB_RPM_VERSION).$(BUILDARCH).rpm . && \
	  cp $(SRCRPMDIR)/RealTimeBattle-$(RTB_RPM_VERSION).src.rpm . ; \
	else \
	  cd ../../ && $(MAKE) rpm ; \
	fi

