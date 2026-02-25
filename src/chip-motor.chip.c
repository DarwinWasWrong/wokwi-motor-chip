
// Wokwi Custom Chip - For docs and examples see:
// https://docs.wokwi.com/chips-api/getting-started
//
// SPDX-License-Identifier: MIT
// Copyright 2023 Darwin WasWrong
// src/L298-module.chip.c

// thanks to
// Maverick - for saving my mind with PWM
//
// https://notisrac.github.io/FileToCArray/
// for the conversion for images
//

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#define BOARD_HEIGHT 100
#define BOARD_WIDTH 100


// basic RGBA color
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} rgba_t;

typedef struct {

  //Module Pins
  pin_t pin_M1;
  pin_t pin_M2;
 
  // motor graphics position
  int8_t motorphase;
  uint8_t motor_y;
  uint8_t motor_x;
  uint8_t motor_state;
  uint8_t motor_arrow_y;
  uint8_t motor_right_arrow_x;
  uint8_t motor_left_arrow_x;

// frame buffer details
  uint32_t fb_w;
  uint32_t fb_h;
  uint32_t row;
  buffer_t framebuffer;

// colors
  rgba_t   white;
  rgba_t   green;
  rgba_t   background;
  rgba_t   purple;
  rgba_t   black;
  rgba_t   red;
  rgba_t   blue;


 // arrow graphics position
  uint8_t vertical_start;
  uint8_t position_x;
  uint8_t position_y;
  
  uint8_t  speed_percent;
  uint8_t  previous_speed_percent;
  timer_t timer_motor;

uint8_t speed_bar_y;
uint8_t speed_bar_x;

} chip_state_t;


static void draw_line(chip_state_t *chip, uint32_t row, rgba_t color);
static void draw_pixel(chip_state_t *chip, uint32_t x,  uint32_t y,  rgba_t color) ;
static void draw_cog(chip_state_t *chip, uint32_t x,  uint32_t y ,int8_t phase);

static void draw_right_arrow(chip_state_t *chip, uint32_t x_start,  uint32_t y_start, uint8_t wipe);
static void draw_left_arrow(chip_state_t *chip, uint32_t x_start,  uint32_t y_start, uint8_t wipe);

static void draw_rectangle(chip_state_t *chip, uint32_t x_start, uint32_t y_start, uint32_t x_len, uint32_t y_len,  rgba_t color, uint8_t fill);
static void draw_speed(chip_state_t *chip, uint32_t x_start, uint32_t y_start, uint32_t x_len, uint32_t y_len,  rgba_t color, uint8_t fill) ;
//static void send_signal(chip_state_t *chip);


// timer for graphics
static void chip_timer_event_motor(void *user_data);



void chip_init(void) {
  printf("*** Motorchip initialising...\n");
  
  chip_state_t *chip = malloc(sizeof(chip_state_t));


  chip->pin_M1 = pin_init("M1",ANALOG);
  chip->pin_M2 = pin_init("M2",ANALOG);

  // Display values
  chip->speed_percent=0;
 

  // display colors
  chip-> white      = (rgba_t) { .r = 0xff, .g = 0xff, .b = 0xff, .a = 0xff };
  chip-> green      = (rgba_t) { .r = 0x08, .g = 0x7f, .b = 0x45, .a = 0xff };
  chip-> background = (rgba_t) { .r = 0xf7, .g = 0xf7, .b = 0xf7, .a = 0xff };
  chip-> purple    = (rgba_t) { .r = 0xff, .g=0x00,   .b=0xff,   .a=0xff   };
  chip-> black    =  (rgba_t) { .r = 0x00, .g=0x00,   .b=0x00,   .a=0x00   };
  chip-> red    =  (rgba_t) { .r = 0xff, .g=0x00,   .b=0x00,   .a=0xff   };
  chip-> blue    =  (rgba_t) { .r = 0x00, .g=0x00,   .b=0xff,   .a=0xff   };
  chip->row = 0;
  // get the screen size
  chip->framebuffer = framebuffer_init(&chip->fb_w, &chip->fb_h);
  printf("Framebuffer: fb_w=%d, fb_h=%d\n", chip->fb_w, chip->fb_h);

   chip->motorphase=0;
  // positioning
  chip->motor_y=20;
  chip->motor_x=113;
  // arrow and speed base position
  chip->motor_arrow_y = 70;
  // Motor arrow positions  relative to motor position
  chip->motor_right_arrow_x =chip->motor_x + 40;
  chip->motor_left_arrow_x = chip->motor_x  ;
  // bars position relative to motor
  chip-> speed_bar_y=  chip->motor_arrow_y + 20;
  chip-> speed_bar_x=chip->motor_x ;

   printf("Framebuffer: fb_w=%d, fb_h=%d\n", chip->fb_w, chip->fb_h);
//  box around displays
   draw_rectangle(chip, 101,10,75,100,chip->green ,0);
   draw_rectangle(chip, 175,10,75,100,chip->green ,0);
  printf("Draw the speed holders ...\n");
  draw_speed(chip, chip-> speed_bar_x,chip-> speed_bar_y,50,15, chip-> purple  ,0);
  //Draw the cogs
  printf("Draw the cogs ...\n");
  draw_cog(chip, chip->motor_y,chip->motor_x,0);

const timer_config_t timer_config_motor = {
    .callback = chip_timer_event_motor,
    .user_data = chip,
  };
  chip->timer_motor = timer_init(&timer_config_motor);
  timer_start(chip->timer_motor,20000, true);


}


// graphics for motor A
void chip_timer_event_motor(void *user_data) {
  chip_state_t *chip = (chip_state_t*)user_data;
  draw_cog(chip, chip->motor_y,chip->motor_x,chip->motorphase);
  if ( chip-> motor_state == 0 )   chip->motorphase=chip->motorphase - 1;
  if ( chip-> motor_state == 1) chip->motorphase=chip->motorphase + 1;
  if (chip->motorphase < 0) chip->motorphase =8;
  if (chip->motorphase >8) chip->motorphase = 0;

}


void draw_cog(chip_state_t *chip, uint32_t x_start,  uint32_t y_start,int8_t phase) {
// size of our graphic
uint8_t square_size = 50;
uint32_t pixel_spot_data = 0;
for (int x=x_start;x < square_size + x_start; x++)
   {
     for (int y=y_start;y<square_size + y_start ;y++)
    {
        uint32_t pixel_data = all_cogs[phase][pixel_spot_data] ;
        rgba_t  color =
            { 
            .r= (pixel_data  & 0xFF000000) >> 24,
            .g = (pixel_data & 0x00FF0000) >> 16,
            .b = (pixel_data & 0x0000FF00) >> 8,
            .a = (pixel_data & 0x000000FF) 
            };
        uint32_t offset = chip->fb_w * 4 ;  
        buffer_write(chip->framebuffer, (offset * x) + y * 4, (uint8_t*)&color , 4); 
        pixel_spot_data++;
    }
   }
}  




void draw_rectangle(chip_state_t *chip, uint32_t x_start, uint32_t y_start, uint32_t x_len, uint32_t y_len,  rgba_t color, uint8_t fill) {
  // draw the top and base lines
  uint32_t offset = chip->fb_w * 4 * y_start;
  uint32_t offset2 = chip->fb_w * 4 * (y_len + y_start) ;
  for (int x = x_start * 4 ; x < x_start * 4 + x_len * 4; x += 4) {
    buffer_write(chip->framebuffer, offset + x, (uint8_t*)&color, sizeof(color));
    buffer_write(chip->framebuffer, offset2 + x, (uint8_t*)&color, sizeof(color));
  }
// draw the left and right lines
  for (int y = y_start  ; y < y_start + y_len ; y++) {
    buffer_write(chip->framebuffer, y * chip->fb_w * 4 + x_start * 4 , (uint8_t*)&color, sizeof(color));
    buffer_write(chip->framebuffer, y * chip->fb_w * 4 +x_start * 4 + (x_len * 4)-4 , (uint8_t*)&color, sizeof(color));
  }
}


void draw_speed(chip_state_t *chip, uint32_t x_start, uint32_t y_start, uint32_t x_len, uint32_t y_len,  rgba_t color, uint8_t percent_up) {
  // draw the top and base lines
  uint32_t offset = chip->fb_w * 4 * y_start;
  uint32_t offset2 = chip->fb_w * 4 * (y_len + y_start) ;

  for (int x = x_start * 4 ; x < x_start * 4 + x_len * 4; x += 4) {
    buffer_write(chip->framebuffer, offset + x, (uint8_t*)&color, sizeof(color));
    buffer_write(chip->framebuffer, offset2 + x, (uint8_t*)&color, sizeof(color));
  }
// draw the left and right lines
  for (int y = y_start  ; y < y_start + y_len ; y++) {
    buffer_write(chip->framebuffer, y * chip->fb_w * 4 + x_start * 4 , (uint8_t*)&color, sizeof(color));
    buffer_write(chip->framebuffer, y * chip->fb_w * 4 +x_start * 4 + (x_len * 4)-4 , (uint8_t*)&color, sizeof(color));
  }
 rgba_t color_black = chip->black;
 for (int y = y_start +1 ; y < y_start + y_len  ; y++) {

    for ( int z= (x_start +1) * 4 ; z < (x_start) * 4 + ((x_len-1)* 4) ; z+= 4)
    {
    buffer_write(chip->framebuffer, y * chip->fb_w * 4 + z , (uint8_t*)&color_black , sizeof(color_black ));
    
    }
 }
    rgba_t color2 = chip->white;
    percent_up = percent_up /2;
    //printf(" x_len %d       percent_up %d\n",x_len, percent_up);
    for (int y = y_start +1 ; y < y_start + y_len  ; y++) {

    for ( int z= (x_start +1) * 4 ; z < (x_start) * 4 + ((percent_up)* 4) ; z+= 4)
    {
    buffer_write(chip->framebuffer, y * chip->fb_w * 4 + z , (uint8_t*)&color2, sizeof(color2));
    
    }
  }
}

void draw_right_arrow(chip_state_t *chip, uint32_t x_start,  uint32_t y_start, uint8_t wipe) {
uint32_t pixel_spot_data = 0;
for (int x=x_start;x < RIGHT_HEIGHT + x_start; x++)
   {
     for (int y=y_start;y<  RIGHT_WIDTH+ y_start ;y++)
    {
       uint32_t pixel_data = right_arrow[pixel_spot_data] ;
        rgba_t  color =
            { 
            .r=  (pixel_data  & 0xFF000000) >> 24,
            .g = (pixel_data & 0x00FF0000) >> 16,
            .b = (pixel_data & 0x0000FF00) >> 8,
            .a = (pixel_data & 0x000000FF) 
            };
        
     if(wipe == 1)
{
  color =  chip-> black;
}   
        uint32_t offset = chip->fb_w * 4 ;  
        buffer_write(chip->framebuffer, (offset * x) + y * 4, (uint8_t*)&color , 4); 
        pixel_spot_data++;
    }
   }
}  

void draw_left_arrow(chip_state_t *chip, uint32_t x_start,  uint32_t y_start, uint8_t wipe) {
uint32_t pixel_spot_data = 0;
for (int x=x_start;x < RIGHT_HEIGHT + x_start; x++)
   {
     for (int y=y_start;y<  RIGHT_WIDTH+ y_start ;y++)
    {
        uint32_t pixel_data = left_arrow[pixel_spot_data] ;
        rgba_t  color =
            { 
            .r= (pixel_data  & 0xFF000000) >> 24,
            .g = (pixel_data & 0x00FF0000) >> 16,
            .b = (pixel_data & 0x0000FF00) >> 8,
            .a = (pixel_data & 0x000000FF) 
            };
        
        
if(wipe == 1)
{
  color =  chip-> black;
}
        uint32_t offset = chip->fb_w * 4 ;  
        buffer_write(chip->framebuffer, (offset * x) + y * 4, (uint8_t*)&color , 4); 
        pixel_spot_data++;
    }
   }
}  











