# Project O.A.S.I.S

**Omnidirectional Aerial and Surface Intercept System**: a multi-modal interceptor for land- and air-based targets. It uses ultrasonic and LiDAR sensors to detect and locate a target, turns a pan-tilt mount to face it, and engages it with a spring-loaded projectile mechanism.

Developed for the course TA212: Manufacturing Processes at IIT Kanpur, where it was awarded best project among 50+ projects in the course.

<img width="1600" height="1200" alt="Final Image" src="https://github.com/user-attachments/assets/f387222d-bba0-4804-b560-19f9888f8ef5" />

## Pan-tilt mechanism

The mount is adapted from isaac879's camera [Pan-Tilt Mount](https://github.com/isaac879/Pan-Tilt-Mount). We modified it so the Arduino electronics and the launcher both fit on it.

## Kinetic mechanism

We tried several launch mechanisms, including an electromagnetic rail gun and an elastic (rubber-band) launcher. Within our time and budget, a spring-loaded mechanism worked best.

## Software

An Arduino UNO runs the system: it is small enough to sit on the mount and has enough memory and speed for the job. The sketch drives the two stepper motors and polls three ultrasonic sensors. Ping timing and sensor placement are arranged so the sensors don't interfere with each other, and memory use stays within the UNO's limits.

To upload it:

1. In the Arduino IDE, install the **NewPing** library from the Library Manager.
2. Open `O.A.S.I.S_Arduino_Code.ino`, select **Arduino Uno** as the board, and upload.
3. Open the Serial Monitor at 9600 baud to see the system's status messages.

Pin assignments are defined at the top of the sketch.

## Repository contents

| Path | Contents |
|---|---|
| `O.A.S.I.S_Arduino_Code.ino` | Final Arduino UNO sketch |
| `O.A.S.I.S_Overall_Report.pdf` | Full project report |
| `images/` | Photos of the finished build, and test videos for each of the three sensors |
