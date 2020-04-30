# AutoLEDController
Autonomic LED controlling device which controls power of LED's,. according to luminous intensity reading from APDS9301 sensor, by previously choosen data by user [data.csv represents my light preferences(left column is luminous intensity, right column is LED power)]. I used scipy.optimize and curve_fit to calculate poly function of my led preferences according to luminous intensity. Coded in Python language. Runs on raspberry pi or any other device, which can be used as TCP server Circuit of LED controller i was modeling on: https://botland.com.pl/img/art/inne/CMS131_schemat.jpg 


Project is in beta(or even alpha) state. TODO LIST: control over django local server. 
Give more functions like: data learn mode and on/off. 


