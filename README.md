# selfiemat

selfiemat is a software designed to operate a photo booth. The software is written in C++ and it is based on Qt5 an gPhoto2. It provides easy customization options and is especially designed to run on a Raspberry Pi with a (LCD) touch screen.
It is also capabel of printing out photos on a photo printer.

Stay tuned to read about the whole selfiemat project (including setup details, housing, configurations to sync photos to a webserver etc.).

## Prerequisite

selfiemat is based on Qt5 and gPhoto2. In order to install the necessary software run the following command on a Ubuntu/Raspbian machine:
```bash
sudo apt-get install qt5-default qt5-qmake libgphoto2-dev
```

## Installation

In order to compile and install sefliemat, run the following commands:
```bash
git clone https://github.com/ChristianHirsch/selfiemat.git
cd selfiemat
qmake
make
make install
```
This installs the software to `/opt/selfiemat`.

## Configuration
An example configuration file looks like this:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<frame width="1772" height="1205" orientation="landscape" background="./selfiemat.png">
	<image x="39" y="35" width="1076" height="727"/>
	<image x="39" y="794" width="551" height="379"/>
	<image x="605" y="794" width="566" height="379"/>
	<image x="1186" y="794" width="553" height="379"/>
</frame>
```
This creates a photo of size 1772x1205px which is landscape oriented (important for printing).
The photo consists of the (background) frame `./selfiemat.png` and four photos that will be taken from the camera.
For each photo you need to provide the coordinates `x` and `y`, and the `width` and `height` (relative to the overall frame size).
selfiemat automatically merges the frame with the photos based on this configuration.
In order to load the configuration you can use the `-t <PATH>` option.