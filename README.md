# DetectaVillanos
An Arduino-based distance detector combined with the Pololu 3Pi robot

* The DetectaVillanos project was made during the ImagineCode hackaton. It is a Pololu 3Pi line avoider combined with an Arduino, for stopping the robot when it detects a "Villain" in front of him.

* The behavior of the robot is quite simple, it uses the IR sensors to avoid the line and when it detects a flag sent by the Arduino in its GPIO 'PC5', it stops. This flag is sent if the Arduino detects an object less than 15 cm in front of him. 

* The montage should include an ultrasonic module, an Arduino Nano or Uno,  a Pololu 3Pi and a few cables.


<img src="https://pbs.twimg.com/media/DoUyzb8XoAEuiBC.jpg" width="300" height="400" />
