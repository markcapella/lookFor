
# *****************************************************
# Variables to control Makefile operation

CC = g++
CFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

lookFor: lookFor.o
	$(CC) $(CFLAGS) -o lookFor lookFor.cpp

lookFor.o: lookFor.cpp
	$(CC) $(CFLAGS) -c lookFor.cpp

# ****************************************************
# Targets needed to bring the executable up to date

install:
	cp lookFor ~/.local/bin
	chmod +x ~/.local/bin/lookFor
	ln -s -f ~/.local/bin/lookFor ~/.local/bin/lf

	@echo "Done!"

uninstall:
	rm -f ~/.local/bin/lf
	rm -f ~/.local/bin/lookFor

	@echo "Done!"

clean:
	rm -f lookFor.o
	rm -f lookFor

	@echo "Done!"