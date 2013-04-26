# Interactive replica of Lumiere Camera using RaspberryPi

![mechanics](https://raw.github.com/patriciogonzalezvivo/lumiereCam/master/images/02.jpeg)

## Raspberry Setup

### [The basics]((https://gist.github.com/jkosoy/5379904))
	sudo wget http://goo.gl/1BOfJ -O /usr/bin/rpi-update && sudo chmod +x /usr/bin/rpi-update
	sudo rpi-update
	sudo reboot
	sudo apt-get clean
	sudo apt-get update
	sudo apt-get upgrade
	sudo apt-get install git-core binutils git libnss-mdns

### [Installing openFramewors](https://github.com/openFrameworks-RaspberryPi/openFrameworks/wiki/Raspberry-Pi-Getting-Started)

	cd ~
	git clone https://github.com/openFrameworks-RaspberryPi/openFrameworks/
	cd openFrameworks/scripts/linux/debian_armv6l
	sudo ./install_codecs.sh
	sudo ./install_dependencies.sh
	cd ~/openFrameworks/libs/openFrameworksCompiled/project
	make
	
	cd ~/openFrameworks/scripts/linux/template/linuxarmv6l
	cp Makefile ~/
	cd ~/openFrameworks/apps/devApps/raspberrypi_hello_world
	cp ~/Makefile ./
	make clean
	make

### [Install wiringPi](https://github.com/openFrameworks-RaspberryPi/openFrameworks/wiki/Raspberry-Pi-Using-the-GPIO-pins-with-Wiring-Pi-and-openFrameworks)
Important: The GPIO pins on the Raspberry Pi is 3.3V, even though there is also a 5V supply. Be sure not to connect a higher voltage than 3.3V to the GPIO pins or you could severely damage your Pi.

WiringPi is a C-library by Drogon making it easy to read and write to the GPIO pins in an Arduino style. See https://projects.drogon.net/raspberry-pi/wiringpi/ for details.

To get and install the library do the following (from projects.drogon.net):

To obtain WiringPi using GIT:

	git clone git://git.drogon.net/wiringPi

If you have already used the clone operation for the first time, then cd wiringPi git pull origin

Will fetch an updated version then you can re-run the build script below. To build/install there is a new simplified script:

	cd wiringPi
	./build

The new build script will compile and install it all for you – it does use the sudo command at one point, so you may wish to inspect the script before running it.

Then in your projects config.make-file include the following line (in bold) in the PROJECT LINKER FLAGS-section:

	################################################################################
	# PROJECT LINKER FLAGS
	#   These flags will be sent to the linker when compiling the executable.
	#
	#   Note: Leave a leading space when adding list items with the += operator
	################################################################################

	# Currently, shared libraries that are needed are copied to the 
	# $(PROJECT_ROOT)/bin/libs directory.  The following LDFLAGS tell the linker to
	# add a runtime path to search for those shared libraries, since they aren't 
	# incorporated directly into the final executable application binary.
	# TODO: should this be a default setting?
	PROJECT_LDFLAGS=-Wl,-rpath=./libs
	PROJECT_LDFLAGS += -lwiringPi

### Remove SPI kernel modules 
1. ```sudo nano /etc/modprobe.d/raspi-blacklist.conf```
2. comment with # (or remove) those lines
3. reboot the pi