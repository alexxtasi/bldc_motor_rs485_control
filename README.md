## Controlling a Brushless Motor via RS485
This repo contains the Arduino code and Fritzing sketch for controlling a Brushless motor via RS485 communication.

### Use
Master's button:
- _rotation_ - changes the direction of the rotation
- _motion_ - spins the motor's shaft for 2 seconds

### TODO
- [ ] use a [toggle switch](https://en.wikipedia.org/wiki/Switch#Toggle_switch) for the _rotation_ button
- [ ] use global constants for e.g the delay
- [ ] use a function for showing activity on the led
- [ ] use half-duplex communication to avoid running the motor when "spamming" the motion button
- [ ] [circuit] perform real tests (with a long cable!)
- [ ] [circuit] consider the use of a ground wire in the RS485 link, in order to reduce noise[^RS485-Gammon] (**not** connecting together Master's and Remote's ground)
- [ ] perform tests with a __real motor__

<!-- info for the articles I used -->
## Based on
1. [RS485 Serial Data Transmission](https://github.com/RalphBacon/RS485-Serial-Data-Transmission) by [Ralph Bacon](https://ralphbacon.blog/)
1. [RS485 serial communications](https://arduino-info.wikispaces.com/RS485-Modules)
1. [SoftwareSerial RS485 Example](https://arduino-info.wikispaces.com/SoftwareSerialRS485Examples)

[^RS485-Gammon]: [RS485 communications - Nick Gammon](https://www.gammon.com.au/forum/?id=11428)

---
> License: ![GNU GPLv3 Logo](https://www.gnu.org/graphics/gplv3-88x31.png) [GNU GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html)
