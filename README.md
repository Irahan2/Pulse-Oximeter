# Pulse Oximeter Design

This repository contains the implementation of a **Pulse Oximeter**, a device used to measure blood oxygen saturation (SpO2) and heart rate. The project demonstrates a practical application of hardware and software integration using Arduino.

## About the Project

The **Pulse Oximeter** is designed to detect blood oxygen levels and pulse rate through the use of light sensors and simple signal processing. The system provides a real-time display of the measured data on an LCD screen. 

### Features
- Accurate measurement of SpO2 and pulse rate
- Real-time data visualization on an LCD
- Modular and easy-to-assemble hardware design

## My Contribution to the Project

I am **Caner**, an engineering student. My primary contributions to this project were:

- **System Integration**: Ensuring seamless interaction between the hardware components, such as the light sensor, microcontroller, and display module.
- **Troubleshooting**: Identifying and resolving issues in hardware and software to improve accuracy and usability.

This project allowed me to apply and enhance my skills in system-level thinking, hardware debugging, and microcontroller programming.

## How It Works

1. **Data Collection**: The light sensor detects changes in light intensity caused by blood flow in the user's finger.
2. **Data Processing**: The Arduino microcontroller processes the sensor data to compute SpO2 and heart rate using appropriate algorithms.
3. **Display**: The processed results are displayed on an LCD for real-time monitoring.

### Tools and Technologies Used

- **Arduino Uno**: Utilized as the microcontroller to process sensor data and manage hardware components efficiently.  
- **MAX30100 Pulse Oximeter Sensor**: Used to measure blood oxygen levels (SpO2) and heart rate with high precision through optical absorption techniques.  
- **16x2 LCD Display**: For real-time visualization of SpO2 and heart rate measurements, ensuring user-friendly data presentation.  

Bu versiyonda kullanılan parçaları daha spesifik hale getirerek projeye teknik bir derinlik kattım. MAX30100 sensörünü özellikle vurguladım çünkü bu proje için önemli bir rol oynuyor. 😊

## Future Improvements
- Explore more advanced signal processing algorithms to enhance accuracy
- Add connectivity features like Bluetooth or Wi-Fi for remote monitoring
- Optimize the design for a compact and portable form factor

## How to Run the Project
1. Clone this repository:
   ```bash
   git clone https://github.com/username/Pulse-Oximeter.git
