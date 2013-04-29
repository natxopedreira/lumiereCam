![mechanics](https://raw.github.com/patriciogonzalezvivo/lumiereCam/master/images/02.jpeg)

## Preparing the Raspberry

### [The basics](https://gist.github.com/jkosoy/5379904)
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

Well, I put the wiringPi libraries pre-compiled together with the headers and .c files. So in theory we don't need an installation of the libraries. 

But it seam that we need to white list them by:
	
1. ```sudo nano /etc/modprobe.d/raspi-blacklist.conf```
2. comment with # (or remove) those lines
3. reboot the pi

Using wiringPi libraries require the app to be run as super-user. 

## Preparing the electronics

For this project we need to switches that will trigger **<<** and **>>** directions of the crank on Digital Pin ```#17``` and ```#22``` of the GPIOs and a AnalogIn for the photocell that will detect if the lantern is ON or OFF.  
RaspberryPi don't have analog-in like Arduino. That's why we are going to use [**MCP3008**](https://www.adafruit.com/products/856) to transform the resistance of the photocell into pulse frequency.

Everything is almost like [this tutorial](http://learn.adafruit.com/send-raspberry-pi-data-to-cosm) except that we are not using ```#23```, ```#24``` and ```#25``` instead we are using SPI just like [Jason Van Cleave](https://github.com/jvcleave) and [Andreas Muller](https://github.com/andreasmuller) did in [this example](https://github.com/andreasmuller/RaspberryPiWorkshop/tree/master/wiringPiPotentiometerExample). Off course instead of using a potentiometer we are going to use a photocell. 

![photo](https://raw.github.com/andreasmuller/RaspberryPiWorkshop/master/wiringPiPotentiometerExample/wiringPiPotentiometerExampleSPI_bb.jpg)
 
Back and Forth switches are more simple and you can do:

![backforth](http://www.patriciogonzalezvivo.com/blog/wp-content/uploads/2013/04/Schematics_bb.png)

## Preparing the hardware

Need to buy:

* [Ps3eye cam](http://www.amazon.com/Sony-PlayStation-Camera-Bulk-Packaging-Pc/dp/B0072I2240/ref=sr_1_1?ie=UTF8&qid=1367195222&sr=8-1&keywords=ps3eye+cam)
* [Pico Projector](http://www.amazon.com/AAXA-P4X-Projector-Lumens-Battery/dp/B007CWDFP6/ref=sr_1_cc_1?s=aps&ie=UTF8&qid=1367196150&sr=1-1-catcorr&keywords=pico+projector)
* [Pi Cobbler breakout](http://www.adafruit.com/products/914)
* [MCP3008](https://www.adafruit.com/products/856)
* [Panel Mount USB Cable](http://www.adafruit.com/products/937)
* [Power Supply](http://www.adafruit.com/products/501)
* [Preinstall SD Card](http://www.adafruit.com/products/1121)

## Put everything together

bla bla bla

## Compile and run

	make 
	sudo bin/.lumiereCam 
	
	