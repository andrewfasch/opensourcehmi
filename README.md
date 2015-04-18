# opensourcehmi
Web based control for the arduino


Instructions for install

1) Create LAMP server( linux or windows )
 - https://www.digitalocean.com/community/tutorials/how-to-install-linux-apache-mysql-php-lamp-stack-on-ubuntu

2) Copy the hmi folder into the html folder on the LAMP server.

3) edit database login info 
 - /var/www/html/hmi/config/login_info.php
 
5) open wepage and nagvagate to 
 - localhost/hmi
 - [server ip]/hmi
 - specifiy analog points and digital points
 
4) Upload sketch to arduino
 - enter IP and number of analog/digital points into sketch
 - opensourcehmi.ino
 

 
 
