# lookFor
    
!['lookFor'](https://github.com/markcapella/lookFor/blob/main/screenshot.png)
    
## Description
        Quickly look for files on your system, using (optionally), a
    string portion of the file name, the location to be searched,
    and anything the file name ends with.
    
    lf   - Returns simple file name results.
    lfl  - Returns detailed file results.
    
    
## Installation
    
### Install Pre-reqs.

For Debian systems:

    sudo apt install git build-essential libglib2.0-dev libgtk-3-dev gettext automake libx11-dev libxft-dev libxpm-dev libxt-dev libxext-dev x11proto-dev libxinerama-dev libxtst-dev libxkbcommon-dev libgsl-dev appmenu-gtk3-module

For Fedora systems:

    sudo dnf install git gcc gcc-c++ make glib2-devel gtk3-devel gdk-pixbuf2-modules-extra gettext automake libX11-devel libXft-devel libXpm-devel libXt-devel libXext-devel xorg-x11-proto-devel libXinerama-devel libXtst-devel libxkbcommon-devel gsl-devel unity-gtk3-module

### CD into source repo.
    make
    
    sudo make install
    sudo make uninstall
    
    make clean
    
    
## Usage
    lookFor string target endswith
    lf string target endswith
    
    lf
    lf foo
    lf foo ~
    lf foo / .cpp
    
    
    lookForLong string target endswith
    lfl string target endswith
    
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
    
