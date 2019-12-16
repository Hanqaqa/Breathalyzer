# Breathalyzer
In this project I created a small script for a fast Alcohol Meter or Breathalyzer using an Arduino, a MQ3 polyalcohol sensor, I2C communications and a small OLED screen.

![alt text](https://github.com/Hanqaqa/Breathalyzer/blob/master/Images/Breathalyzer.jpg)

https://www.youtube.com/watch?v=MPGyes-g0cU&feature=youtu.be

The code tries to be as simple as possible, using I2C communications and the Adafruit_GFX Adafruit__SSD1306 libraies in order to simplify the display on the OLED screen.

I normalized the Input/Output Alchol line (Black squares in the datasheet's image) using 15mg/L segments, as normalizing the whole Alchol line would require much better equipment.

![alt text](https://github.com/Hanqaqa/Breathalyzer/blob/master/Images/Datasheet.PNG)

The pseudocode or block diagram can be seen in this image:

![alt text](https://github.com/Hanqaqa/Breathalyzer/blob/master/Images/Block_Diagram.PNG)

