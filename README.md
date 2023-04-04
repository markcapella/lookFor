# lookFor

    alias - lf


## Description

###    Quickly look for files on your system, using (optionally), a string portion
###    of the file name, the location to be searched, and anything the file name ends with.



## Installation

###    CD into source repo.

    make
    make clean

    make install
    make uninstall


## Usage

###    lookFor {string=""} {target="."} {endswith=""}
###    lf {string=""} {target="."} {endswith=""}

    NOTE: If target is the root dir "/", the user's HOME folder will be skipped.

    lf
    lf "" /
    lf foo
    lf foo ~
    lf foo /
    lf "" ~ /.git
    lf foo ~ .cpp
    lf foo / .cpp

