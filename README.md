Project Title: Sustainable Autonomous Device for River Waste Management

This project addresses the critical issue of plastic pollution in rivers by developing a sustainable autonomous device designed to collect and segregate river waste. The system integrates three major mechanisms: the Flapper, Air Blow, and Ballistic Separator. The Flapper mechanism is controlled using servo motors and IR sensors. The Air Blow mechanism comprises an ESC (Electronic Speed Controller), BLDC motor, propeller, and a servo connector. A conveyor belt, driven by four DC motors, is used to collect and transport waste. As waste reaches the top of the belt, a gate mechanism is activated, guided by two Proximity Capacitive (PC) sensors.

PC-1 detects plastic waste based on color and density.
PC-2 identifies non-plastic waste.

Upon detection, the corresponding flapper mechanism is triggered via IR sensors and the flapper rotates at an angle of 60 degree driven by the servo motor. The first flapper opens for non-plastic waste, while the second is designed to open for plastic waste. Lightweight plastic is then blown away using the Air Blow mechanism. Separated waste is collected in designated bins. Each bin is equipped with an ultrasonic sensor to monitor fill levels — categorized as low, medium, or high. Once either bin reaches full capacity, an SMS is sent to registered users. 

The SMS includes:
The bin’s GPS coordinates (latitude and longitude),
The bin’s fill level,
The type of waste collected.

Limitations:
The device is currently implemented at a small scale and is capable of handling only lightweight waste, with a maximum capacity of 1–2 pieces at a time.

Components Used:
IR Sensors (2 units)
Ultrasonic Sensors (2 units)
Proximity Capacitive Sensors (2 units)
Servo Motors (2 units)
Microcontroller: ESP32 NodeMCU
ESC (Electronic Speed Controller)
Propeller
BLDC Motor
DC Motors (4 units)
Batteries: 7V & 14V
Electrical Wires

Materials Used:
Rubber, Aluminum Sheets, Steel, Glass Fiber, 3D-Printed Flappers


                                                                  MODEL DESIGN
![Image](https://github.com/user-attachments/assets/b601e140-f7b8-4dcf-a4d0-dcb4f81867d1)
