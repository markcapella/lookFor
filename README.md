# lookFor

!['lookFor'](https://github.com/markcapella/lookFor/blob/main/assets/screenshot.png)

## Description

###    Quickly look for files on your system, using (optionally), a string portion
###    of the file name, the location to be searched, and anything the file name ends with.

    lf  - returns simple file name results.
    lfl - returns detailed file results.


## Installation

###    CD into source repo.

    make

    sudo make install
    sudo make uninstall

    make clean


## Usage

###    lookFor string target endswith
###    lf string target endswith

    lf
    lf foo
    lf foo ~
    lf foo / .cpp


###    lookForLong string target endswith
###    lfl string target endswith

    lfl
    lfl foo
    lfl foo ~
    lfl foo / .cpp


### NOTE:

        If the target of lookFor or lookForSelect is the root dir "/",
    the user's /HOME folder is assumed un-interesting and will be skipped,
    perhaps requiring a second scan for an entire system review.

        We're now also skipping /MEDIA during root scans to avoid
    thrashing thumb storage file-systems. Use an explicit scan requeast.


## markcapella@twcny.rr.com Rocks !

    Yeah I do.
