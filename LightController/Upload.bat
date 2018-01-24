avrdude -p 2313 -P com4 -c avrisp -b 19200 -v -U flash:w:Build\LightController.hex
pause