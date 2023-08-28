
# *****************************************************
# Variables to control Makefile operation

CC = g++
CFLAGS = -Wall -O3 -s

# ****************************************************
# Targets needed to bring the executable up to date

all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%.out: %.cpp Makefile
	$(CC) $(CFLAGS) $< -o $(@:.out=)
	@echo "Build "$(@:.out=)" Complete!"
	@echo

# ****************************************************
# Targets needed to bring the executable up to date

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

clean: $(patsubst %.cpp, %.clean, $(wildcard *.cpp))
	@echo "Done!"
	@echo

%.clean:
	rm -f $(@:.clean=)
