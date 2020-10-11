Simple REST requests with showing result on the screen. 
Clone of the example app with some additionals. 
Added 3 lines for static URL's addresses (in the setup screen).
After that it is possbile to requests urls by pressing corresponding buttons (R1, R2, R3) in the main screen of app.
App sends GET request and shows answer (JSON "result" field) in the text area. 
I use it to requests my NODE-RED server that makes more complicated requests to the other staff like REST API services or IoT devices.

How to add this app to other repo:
1. change .pio/libdeps/ttgo-t-watch/TTGO TWatch Library/src/lv_conf.h to:
    /*Declare the type of the user data of object (can be e.g. `void *`, `int`, `struct`)*/
    typedef int lv_obj_user_data_t;

2. add my_app_setup() call into src/my-ttgo-watch.ino somewhere after gui_setup();

3. add folder src/app/my_app with all its files into to the project

4. 