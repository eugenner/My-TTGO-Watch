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
#include "my_app_setup.h"

#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"
#include "gui/keyboard.h"


lv_obj_t *my_app_setup_tile = NULL;
lv_style_t my_app_setup_style;

lv_obj_t *url_textarea = NULL;
lv_obj_t *url_textarea2 = NULL;
lv_obj_t *url_textarea3 = NULL;

LV_IMG_DECLARE(exit_32px);

static void exit_my_app_setup_event_cb( lv_obj_t * obj, lv_event_t event );
static void my_app_textarea_event_cb( lv_obj_t * obj, lv_event_t event );

void my_app_setup_setup( uint32_t tile_num ) {

    my_app_setup_tile = mainbar_get_tile_obj( tile_num );
    lv_style_copy( &my_app_setup_style, mainbar_get_style() );

    my_app_config_t * my_app_config = my_app_get_config();

    lv_style_set_bg_color( &my_app_setup_style, LV_OBJ_PART_MAIN, LV_COLOR_GRAY);
    lv_style_set_bg_opa( &my_app_setup_style, LV_OBJ_PART_MAIN, LV_OPA_100);
    lv_style_set_border_width( &my_app_setup_style, LV_OBJ_PART_MAIN, 0);
    lv_obj_add_style( my_app_setup_tile, LV_OBJ_PART_MAIN, &my_app_setup_style );

    lv_obj_t *exit_cont = lv_obj_create( my_app_setup_tile, NULL );
    lv_obj_set_size( exit_cont, lv_disp_get_hor_res( NULL ) , 40);
    lv_obj_add_style( exit_cont, LV_OBJ_PART_MAIN, &my_app_setup_style  );
    lv_obj_align( exit_cont, my_app_setup_tile, LV_ALIGN_IN_TOP_MID, 0, 10 );

    lv_obj_t *exit_btn = lv_imgbtn_create( exit_cont, NULL);
    lv_imgbtn_set_src( exit_btn, LV_BTN_STATE_RELEASED, &exit_32px);
    lv_imgbtn_set_src( exit_btn, LV_BTN_STATE_PRESSED, &exit_32px);
    lv_imgbtn_set_src( exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &exit_32px);
    lv_imgbtn_set_src( exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &exit_32px);
    lv_obj_add_style( exit_btn, LV_IMGBTN_PART_MAIN, &my_app_setup_style );
    lv_obj_align( exit_btn, exit_cont, LV_ALIGN_IN_TOP_LEFT, 10, 0 );
    lv_obj_set_event_cb( exit_btn, exit_my_app_setup_event_cb );
    
    lv_obj_t *exit_label = lv_label_create( exit_cont, NULL);
    lv_obj_add_style( exit_label, LV_OBJ_PART_MAIN, &my_app_setup_style  );
    lv_label_set_text( exit_label, "my app setup");
    lv_obj_align( exit_label, exit_btn, LV_ALIGN_OUT_RIGHT_MID, 5, 0 );

    // url 1 set field 
    lv_obj_t *my_app_url_label = lv_label_create( my_app_setup_tile, NULL);
    lv_obj_add_style( my_app_url_label, LV_OBJ_PART_MAIN, &my_app_setup_style  );
    lv_label_set_text( my_app_url_label, "url1:");
    lv_obj_align( my_app_url_label, my_app_setup_tile, LV_ALIGN_IN_TOP_LEFT, 0, 50 );

    url_textarea = lv_textarea_create( my_app_setup_tile, NULL);
    lv_textarea_set_pwd_mode( url_textarea, false);
    lv_textarea_set_one_line( url_textarea, true);
    lv_textarea_set_cursor_hidden( url_textarea, true);
    lv_obj_set_width( url_textarea, LV_HOR_RES);
    lv_textarea_set_text( url_textarea, my_app_config->url );
    lv_obj_align( url_textarea, my_app_url_label, LV_ALIGN_IN_TOP_LEFT, 0, 20 );
    lv_obj_set_event_cb( url_textarea, my_app_textarea_event_cb );

    // url 2 set field 
    lv_obj_t *my_app_url_label2 = lv_label_create( my_app_setup_tile, NULL);
    lv_obj_add_style( my_app_url_label2, LV_OBJ_PART_MAIN, &my_app_setup_style  );
    lv_label_set_text( my_app_url_label2, "url2:");
    lv_obj_align( my_app_url_label2, url_textarea, LV_ALIGN_IN_TOP_LEFT, 0, 40 );

    url_textarea2 = lv_textarea_create( my_app_setup_tile, NULL);
    lv_textarea_set_pwd_mode( url_textarea2, false);
    lv_textarea_set_one_line( url_textarea2, true);
    lv_textarea_set_cursor_hidden( url_textarea2, true);
    lv_obj_set_width( url_textarea2, LV_HOR_RES);
    lv_textarea_set_text( url_textarea2, my_app_config->url2 );
    lv_obj_align( url_textarea2, my_app_url_label2, LV_ALIGN_IN_TOP_LEFT, 0, 20 );
    lv_obj_set_event_cb( url_textarea2, my_app_textarea_event_cb );

    // url 3 set field 
    lv_obj_t *my_app_url_label3 = lv_label_create( my_app_setup_tile, NULL);
    lv_obj_add_style( my_app_url_label3, LV_OBJ_PART_MAIN, &my_app_setup_style  );
    lv_label_set_text( my_app_url_label3, "url3:");
    lv_obj_align( my_app_url_label3, url_textarea2, LV_ALIGN_IN_TOP_LEFT, 0, 40 );

    url_textarea3 = lv_textarea_create( my_app_setup_tile, NULL);
    lv_textarea_set_pwd_mode( url_textarea3, false);
    lv_textarea_set_one_line( url_textarea3, true);
    lv_textarea_set_cursor_hidden( url_textarea3, true);
    lv_obj_set_width( url_textarea3, LV_HOR_RES);
    lv_textarea_set_text( url_textarea3, my_app_config->url3 );
    lv_obj_align( url_textarea3, my_app_url_label3, LV_ALIGN_IN_TOP_LEFT, 0, 20 );
    lv_obj_set_event_cb( url_textarea3, my_app_textarea_event_cb );       

}

static void my_app_textarea_event_cb( lv_obj_t * obj, lv_event_t event ) {
    if( event == LV_EVENT_CLICKED ) {
        keyboard_set_textarea( obj );
    }
}

static void exit_my_app_setup_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       
                                        keyboard_hide();
                                        my_app_config_t *my_app_config = my_app_get_config();
                                        strlcpy( my_app_config->url, lv_textarea_get_text( url_textarea ), sizeof( my_app_config->url ) );
                                        strlcpy( my_app_config->url2, lv_textarea_get_text( url_textarea2 ), sizeof( my_app_config->url2 ) );
                                        strlcpy( my_app_config->url3, lv_textarea_get_text( url_textarea3 ), sizeof( my_app_config->url3 ) );
                                        my_app_save_config();
                                        statusbar_hide( true );
                                        mainbar_jump_to_tilenumber( my_app_get_app_main_tile_num(), LV_ANIM_ON );
                                        break;
    }
}