
# *****************************************************
# Variables to control Compile / Link.

# Color styling.
COLOR_RED = \033[0;31m
COLOR_GREEN = \033[1;32m
COLOR_YELLOW = \033[1;33m
COLOR_BLUE = \033[1;34m
COLOR_NORMAL = \033[0m

CC = g++

CFLAGS = -Wall -O3 -s

# ****************************************************
# Compile any single FOO.cpp file and link it as FOO.

all:
	@if [ "$(shell id -u)" = 0 ]; then \
		echo; \
		echo "$(COLOR_RED)Error!$(COLOR_NORMAL) You must not"\
			"be root to perform this action."; \
		echo; \
		echo  "Please re-run with:"; \
		echo "   $(COLOR_GREEN)make$(COLOR_NORMAL)"; \
		echo; \
		exit 1; \
	fi

	@echo
	@echo "$(COLOR_BLUE)Build Starts.$(COLOR_NORMAL)"
	@echo

	$(CC) $(CFLAGS) -c lookFor.cpp
	$(CC) lookFor.o $(APP_LFLAGS) -o lookFor

	$(CC) $(CFLAGS) -c lookForLong.cpp
	$(CC) lookForLong.o $(APP_LFLAGS) -o lookForLong

	ln -sf testFolder/linkTarget testFolder/softLink

	@echo "true" > "BUILD_COMPLETE"

	@echo
	@echo "$(COLOR_BLUE)Build Done.$(COLOR_NORMAL)"

# ****************************************************
# sudo make install

install:
	@if [ ! -f BUILD_COMPLETE ]; then \
		echo; \
		echo "$(COLOR_RED)Error!$(COLOR_NORMAL) Nothing"\
			"currently built to install."; \
		echo; \
		echo "Please make this project first, with:"; \
		echo "   $(COLOR_GREEN)make$(COLOR_NORMAL)"; \
		echo; \
		exit 1; \
	fi

	@if ! [ "$(shell id -u)" = 0 ]; then \
		echo; \
		echo "$(COLOR_RED)Error!$(COLOR_NORMAL) You must"\
			"be root to perform this action."; \
		echo; \
		echo  "Please re-run with:"; \
		echo "   $(COLOR_GREEN)sudo make install$(COLOR_NORMAL)"; \
		echo; \
		exit 1; \
	fi

	@echo
	@echo "$(COLOR_BLUE)Install Starts.$(COLOR_NORMAL)"
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

	@echo
	@echo "$(COLOR_BLUE)Install Done.$(COLOR_NORMAL)"

# ****************************************************
# sudo make uninstall

uninstall:
	@if ! [ "$(shell id -u)" = 0 ]; then \
		echo; \
		echo "$(COLOR_RED)Error!$(COLOR_NORMAL) You must"\
			"be root to perform this action."; \
		echo; \
		echo  "Please re-run with:"; \
		echo "   $(COLOR_GREEN)sudo make uninstall$(COLOR_NORMAL)"; \
		echo; \
		exit 1; \
	fi

	@echo
	@echo "$(COLOR_BLUE)Uninstall Starts.$(COLOR_NORMAL)"
	@echo

	rm -f /usr/local/bin/lf
	rm -f /usr/local/bin/lookFor
	@echo

	rm -f /usr/local/bin/lfl
	rm -f /usr/local/bin/lookForLong
	@echo

	@echo
	@echo "$(COLOR_BLUE)Uninstall Done.$(COLOR_NORMAL)"

# ****************************************************
# make clean

clean:
	@if [ "$(shell id -u)" = 0 ]; then \
		echo; \
		echo "$(COLOR_RED)Error!$(COLOR_NORMAL) You must not"\
			"be root to perform this action."; \
		echo; \
		echo  "Please re-run with:"; \
		echo "   $(COLOR_GREEN)make clean$(COLOR_NORMAL)"; \
		echo; \
		exit 1; \
	fi

	@echo
	@echo "$(COLOR_BLUE)Clean Starts.$(COLOR_NORMAL)"
	@echo

	rm -f lookFor
	rm -f lookFor.o
	rm -f lookForLong
	rm -f lookForLong.o
	rm -f testFolder/softLink

	@rm -f "BUILD_COMPLETE"

	@echo
	@echo "$(COLOR_BLUE)Clean Done.$(COLOR_NORMAL)"
