###################################

ifdef PS4SDKSRC
PS4SDK = $(PS4SDKSRC)
endif

ifndef PS4SDK
ifdef ps4sdk
PS4SDK := $(ps4sdk)
endif
ifdef PS4SDK
PS4SDK := $(PS4SDK)
endif
ifndef PS4SDK
$(error Neither PS4SDKSRC, PS4SDK, Ps4Sdk nor ps4sdk set)
endif
endif

###################################

AssemblerFlags += -I$(PS4SDK)/include
CompilerFlags += -D__PS4__ -I$(PS4SDK)/include -I $(PS4SDK)/include/sce
CompilerFlagsCpp += -D__PS4__ -I$(PS4SDK)/include -I $(PS4SDK)/include/sce -I$(PS4SDK)/include/c++ -I$(PS4SDK)/include/c++/tr1
LinkerFlags += -L$(PS4SDK)/lib

###################################

CrtFile ?= $(PS4SDK)/crt0.s
#link = $(Linker) $(Ps4Sdk)/crt0.s $? $(LinkerFlags) $(Libraries) -o $@

###################################
