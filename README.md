# <ins>lookFor</ins>

    alias - lf


## <ins>Description</ins>

###    Quickly look for files on your system, using (optionally), a string portion
###    of the file name, the location to be searched, and anything the file name ends with.



## <ins>Installation</ins>

###    CD into source repo.

    make
    make install

    make uninstall
    make clean

## <ins>Usage</ins>

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

