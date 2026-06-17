# Project-O.A.S.I.S

Note - This project was devloped as part of the course TA212:Manufacturing Processes offered at Indian Institute of Technology Kanpur and was awarded as the best project among 50+ other projects developed in the same course.

Project O.A.S.I.S (Omnidirectional Aerial and Surface Intercept System) is a multi-modal interceptor system for land and air based threats. It uses Ultrasonic as well as LiDAR sensors for detecting and pinpointing the targets and then a Pan-Tilt system to align with the target and engage it with its projectile mechanism.

<img width="1600" height="1200" alt="Final Image" src="https://github.com/user-attachments/assets/f387222d-bba0-4804-b560-19f9888f8ef5" />

This project was devloped as part of the course TA212:Manufacturing Processes offered at Indian Institute of Technology Kanpur and was awarded as the best project among 50+ other projects developed in the same course.

## Physical Pan-Tilt Mechanism
We were inspired for the pan-tilt mechnaism from the camera pan-tilt mount created by isaac879(https://github.com/isaac879/Pan-Tilt-Mount). We modified it a bit to help it usable for our usecase inorder to fit the arduino sub-system along with the kinetic sub-system. 

## Kinetic Mechanism
We tried multiple kinetic systems including but not limited to Electro-Magnetic Rail Gun, Elastic Potential Gun(Rubber) but finally had to settle with spring loaded shooting mechanism due to resource and time constriant 

## Software Subsystem
Arduino UNO was used as the main board due to its small size and good enough storage and processing capabilites for our usecase. The full code for the uno is uploaded in one of the Arduino UNO Code file, cyclical sensor ping, sensor placement to avoid ping overlap and memory size constraint have all been optimized for in the code for seamless integration and easy workflow on edge devices such as the uno itself.


