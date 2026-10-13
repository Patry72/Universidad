# Driver SO #

### What is this repository for? ###

* Driver example using Raspi DAC board 

### How do I get set up? ###

Use: 

* *make* to build the module

* *make install*, to install de module

* *make uninstall*, to unsinstall

Once installed, use:

* *./bin2char 111000 > /dev/leds*      to set the led ON/OFF
* *./dev2char /dev/leds*                     to get the led values

### Who do I talk to? ###
* Andrés Rodríguez (andres@uma.es)
