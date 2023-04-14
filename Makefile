
# *****************************************************
# Variables to control Makefile operation

CC = g++
CFLAGS = -Wall -O3 -s

# ****************************************************
# Targets needed to bring the executable up to date

lookFor: lookFor.o
	$(CC) $(CFLAGS) -o lookFor lookFor.cpp

lookFor.o: lookFor.cpp
	$(CC) $(CFLAGS) -c lookFor.cpp

# ****************************************************
# Targets needed to bring the executable up to date

install: lookFor
ifneq ($(shell id -u), 0)
	@echo "You must be root to perform this action. Please re-run with:"
	@echo "   sudo make install"
	@echo
	@exit 1;
endif

	@echo
	@echo "sudo make install: starts ..."
	cp lookFor /usr/local/bin
	chmod +x /usr/local/bin/lookFor
	ln -s -f /usr/local/bin/lookFor /usr/local/bin/lf

	@echo
	@type lf
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
	rm -f /usr/local/bin/lf
	rm -f /usr/local/bin/lookFor

	@echo "Done!"
	@echo

clean:
	rm -f lookFor.o
	rm -f lookFor

	@echo "Done!"
	@echo
