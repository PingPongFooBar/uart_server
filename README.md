# uart_server
# by CZJ
a server for serial to net.

This project is a serial port server project,and  it is use to get massage from remote uart devices.
This project is an embedded linux project. The platform is smart4418, and I use wk2114 to expand serial port.
Beside uart communication, the project also has the function to control remote IO and have some special funtions than I want.
Path /platform include the drivers. Junction_driver.c is the driver and junction_dev0.c ~ junction_dev4.c is 5 devices.
Path /uart_server is for uart communication through net, and /junction_server is for remote junction control through ethernet.
