# Room mapping with OpenGL
## Abstract
This project presents a method of analyzing PointCloud data collected from a lidar sensor attached to an Xmachine X100 mobile robot for mapping purposes. Programm allows the user to record points from the sensor in an absolute coordinate system not influenced by the robot's position, visualization of said points, isolating stationary objects with adjustable sensitivity, as well as exporting and importing point data with a simple XML format. The program also allows the user to move through the 3d scene for better visualization.  
## Instructions
The program acquires the data through a network socket with TCP protocol, which opens automatically on port 44201. When program is opened, you can see the recorded points which represent stationary objects. In the top left corner there are options for importing and exporting point data from and to an XML file. More about the sensitivity tweaking and XML formats can be read in the full documentation of the project.
![Image Alt text](/images/Capture1.PNG "Main Screen"))
## Contributors
Admir Ahmespahić, Nejla Buljina, Alen Hrbat
