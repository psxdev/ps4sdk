ifeq (x$(PS4SDKSRC), x)
  export PS4SDKSRC=$(shell pwd)
endif

.DEFAULT_GOAL := all

%:
	$(MAKE) -C base $@ $?
	$(MAKE) -C common $@ $?
	$(MAKE) -C extension $@ $?
	$(MAKE) -C core $@ $?


# PS4SDK just should use it for the installation process to copy the files

install:
	if test ! -d $(PS4SDK) ; then \
		mkdir -p $(PS4SDK) ; \
	fi
	
	cp -R ./include $(PS4SDK)/include
	cp -R ./make $(PS4SDK)/make
	cp -R ./lib $(PS4SDK)/lib
	cp crt0.s $(PS4SDK)
	cp linker.x $(PS4SDK)
