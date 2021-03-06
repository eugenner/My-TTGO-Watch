/****************************************************************************
 *   Aug 3 12:17:11 2020
 *   Copyright  2020  Dirk Brosswick
 *   Email: dirk.brosswick@googlemail.com
 ****************************************************************************/
 
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "config.h"
#include <TTGO.h>

#include "my_app.h"
#include "my_app_main.h"
#include "my_app_setup.h"

#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"
#include "gui/app.h"
#include "gui/widget.h"

#include "hardware/json_psram_allocator.h"
#include <hardware/wifictl.h>

my_app_config_t my_app_config;


uint32_t my_app_main_tile_num;
uint32_t my_app_setup_tile_num;

// app icon
icon_t *my_app = NULL;

// widget icon
icon_t *my_widget = NULL;

// declare you images or fonts you need
LV_IMG_DECLARE(my_app_64px);
LV_IMG_DECLARE(r_app);
LV_IMG_DECLARE(info_1_16px);

// declare callback functions for the app and widget icon to enter the app
static void enter_my_app_event_cb( lv_obj_t * obj, lv_event_t event );
static void enter_my_widget_event_cb( lv_obj_t * obj, lv_event_t event );
static bool wifi_connect = false;
bool check_wifictl_event_cb( EventBits_t event, void *arg );

void my_app_load_config( void );
/*
 * setup routine for my app
 */
void my_app_setup( void ) {

    my_app_load_config();
    // register 2 vertical tiles and get the first tile number and save it for later use
    my_app_main_tile_num = mainbar_add_app_tile( 1, 2, "my app" );
    my_app_setup_tile_num = my_app_main_tile_num + 1;

    // register app icon on the app tile
    // set your own icon and register her callback to activate by an click
    // remember, an app icon must have an size of 64x64 pixel with an alpha channel
    // use https://lvgl.io/tools/imageconverter to convert your images and set "true color with alpha" to get fancy images
    // the resulting c-file can put in /app/examples/images/ and declare it like LV_IMG_DECLARE( your_icon );
    my_app = app_register( "R123", &r_app, enter_my_app_event_cb );
    app_hide_indicator(my_app);
    // register callback in your setup function
    wifictl_register_cb( WIFICTL_CONNECT | WIFICTL_OFF, check_wifictl_event_cb, "check wifi" );


#ifdef MY_WIDGET
    // register widget icon on the main tile
    // set your own icon and register her callback to activate by an click
    // remember, an widget icon must have an max size of 64x64 pixel
    // use https://lvgl.io/tools/imageconverter to convert your images and set "true color with alpha" to get fancy images
    // the resulting c-file can put in /app/examples/images/ and declare it like LV_IMG_DECLARE( your_icon );
    my_widget = widget_register( "", &r_app, enter_my_widget_event_cb );
    widget_hide_indicator(my_widget);
#endif // MY_WIDGET

    // init main and setup tile, see my_app_main.cpp and my_app_setup.cpp
    my_app_main_setup( my_app_main_tile_num );
    my_app_setup_setup( my_app_setup_tile_num );
}

/*
 *
 */
uint32_t my_app_get_app_main_tile_num( void ) {
    return( my_app_main_tile_num );
}

/*
 *
 */
uint32_t my_app_get_app_setup_tile_num( void ) {
    return( my_app_setup_tile_num );
}

/*
 *
 */
static void enter_my_app_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       statusbar_hide( true );
                                        mainbar_jump_to_tilenumber( my_app_main_tile_num, LV_ANIM_OFF );
                                        break;
    }    
}

/*
 *
 */
static void enter_my_widget_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       statusbar_hide( true );
                                        mainbar_jump_to_tilenumber( my_app_main_tile_num, LV_ANIM_OFF );
                                        break;
    }    
}

my_app_config_t *my_app_get_config( void ) {
    return( &my_app_config );
}


/*
 *
 */
void my_app_save_config( void ) {
  

    fs::File file = SPIFFS.open( MY_APP_JSON_CONFIG_FILE, FILE_WRITE );

    if (!file) {
        log_e("Can't open file: %s!", MY_APP_JSON_CONFIG_FILE );
    }
    else {
        SpiRamJsonDocument doc( 1000 );

        doc["url"] = my_app_config.url;
        doc["url2"] = my_app_config.url2;
        doc["url3"] = my_app_config.url3;

        if ( serializeJsonPretty( doc, file ) == 0) {
            log_e("Failed to write config file");
        }
        doc.clear();
    }
    file.close();
}

/*
 *
 */
void my_app_load_config( void ) {
    if ( SPIFFS.exists( MY_APP_JSON_CONFIG_FILE ) ) {        
        fs::File file = SPIFFS.open( MY_APP_JSON_CONFIG_FILE, FILE_READ );
        if (!file) {
            log_e("Can't open file: %s!", MY_APP_JSON_CONFIG_FILE );
        }
        else {
            int filesize = file.size();
            SpiRamJsonDocument doc( filesize * 2 );

            DeserializationError error = deserializeJson( doc, file );
            if ( error ) {
                log_e("update check deserializeJson() failed: %s", error.c_str() );
            }
            else {

                log_i("doc: url: %s, url2: %s, url3: %s ",  doc["url"], doc["url2"], doc["url3"]);

                if(doc.containsKey("url"))
                    strlcpy( my_app_config.url, doc["url"], sizeof( my_app_config.url) );
                if(doc.containsKey("url2"))
                    strlcpy( my_app_config.url2, doc["url2"], sizeof( my_app_config.url2) );
                if(doc.containsKey("url3"))
                    strlcpy( my_app_config.url3, doc["url3"], sizeof( my_app_config.url3) );

            }        
            doc.clear();
        }
        file.close();
    }

}

bool check_wifictl_event_cb( EventBits_t event, void *arg ) {   
    switch( event ) {
        case WIFICTL_CONNECT:
            wifi_connect = true;  
            log_i("my app check_wifictl_event_cb: WIFICTL_CONNECT");
            break;
        case WIFICTL_OFF:
            wifi_connect = false;  
            log_i("my app check_wifictl_event_cb: WIFICTL_OFF");
            break;
        case WIFICTL_DISCONNECT:
            wifi_connect = false;  
            log_i("my app check_wifictl_event_cb: WIFICTL_DISCONNECT");
            break;
    }
    return( true );
}

boolean get_wifi_connect(void) {
    return wifi_connect;
}
