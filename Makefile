
# *****************************************************
# Variables to control Compile / Link.

CC = g++
CFLAGS = -Wall -O3 -s

# ****************************************************
# Compile any single FOO.cpp file and link it as FOO.

all: $(patsubst %.cpp, %.target, $(wildcard *.cpp))

%.target: %.cpp Makefile
	$(CC) $(CFLAGS) $< -o $(@:.target=)
	@echo "Build "$(@:.target=)" Complete!"
	@echo

# ****************************************************
# sudo make install

install: all
ifneq ($(shell id -u), 0)
	@echo "You must be root to perform this action. Please re-run with:"
	@echo "   sudo make install"
	@echo
	@exit 1;
endif

	@echo
	@echo "sudo make install: starts ..."
	@echo

	cp lookFor /usr/local/bin
	chmod +x /usr/local/bin/lookFor
	ln -s -f /usr/local/bin/lookFor /usr/local/bin/lf
	@type lf
	@echo

	cp lookForLong /usr/local/bin
	chmod +x /usr/local/bin/lookForLong
	ln -s -f /usr/local/bin/lookForLong /usr/local/bin/lfl
	@type lfl
	@echo

	@echo "Done!"
	@echo

# ****************************************************
# sudo make uninstall

uninstall:
ifneq ($(shell id -u), 0)
	@echo "You must be root to perform this action. Please re-run with:"
	@echo "   sudo make uninstall"
	@echo
	@exit 1;
endif

	@echo
	@echo "sudo make uninstall: starts ..."
	@echo

	rm -f /usr/local/bin/lf
	rm -f /usr/local/bin/lookFor
	@echo

	rm -f /usr/local/bin/lfl
	rm -f /usr/local/bin/lookForLong
	@echo

	@echo "Done!"
	@echo

# ****************************************************
# make clean

clean: $(patsubst %.cpp, %.clean, $(wildcard *.cpp))
	@echo "Done!"
	@echo

%.clean:
	rm -f $(@:.clean=)
