
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

#define BOARD_HEIGHT 50
#define BOARD_WIDTH 100


// left arrow
#define LEFT_HEIGHT 14
#define LEFT_WIDTH 10

// array size is 560
static const int32_t left_arrow[]  = {
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xfe000052, 0xff00000f, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff00007c, 0xfe0000ea, 0xff00003f, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff00000c, 0xff0000af, 0xfe0000f3, 0xff0000ff, 0xff00003d, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff00002d, 0xfe0000cb, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff00003a, 
  0x00000000, 0x00000000, 0x00000000, 0xff00005a, 0xfe0000e3, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xfe0000ec, 0xff000037, 
  0x00000000, 0x00000000, 0xfe000096, 0xfe0000f4, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xfe0000d8, 0xff000034, 
  0x00000000, 0xfe0000ce, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xfe0000d9, 0xff000032, 
  0x00000000, 0xfe00008f, 0xfe0000f4, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xfe0000d9, 0xff00002f, 
  0x00000000, 0x00000000, 0xfe00006a, 0xfe0000e1, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xfe0000da, 0xff00002d, 
  0x00000000, 0x00000000, 0x00000000, 0xff000046, 0xfe0000d4, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xfe0000db, 0xff00002a, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000021, 0xfe0000ce, 0xff0000ff, 0xff0000ff, 0xfe0000db, 0xff000028, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000007, 0xff0000b2, 0xff0000ff, 0xfe0000dc, 0xff000025, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xfe000085, 0xfe0000dc, 0xff000022, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xfe000057, 0xff000006
};

//right arrow
#define RIGHT_HEIGHT 14
#define RIGHT_WIDTH 10

// array size is 560
static const int32_t right_arrow[]  = {
  0xff000008, 0xff00005b, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0xff000025, 0xfe0000dc, 0xfe000086, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0xff000027, 0xfe0000dc, 0xff0000ff, 0xff0000b2, 0xff000008, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0xff00002a, 0xfe0000db, 0xff0000ff, 0xff0000ff, 0xfe0000cd, 0xff000021, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0xff00002c, 0xfe0000da, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xfe0000d3, 0xff000047, 0x00000000, 0x00000000, 0x00000000, 
  0xff00002f, 0xfe0000d9, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000e0, 0xfe00006a, 0x00000000, 0x00000000, 
  0xff000032, 0xfe0000d9, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xfe0000f3, 0xff000090, 0x00000000, 
  0xff000034, 0xfe0000d8, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000cc, 0x00000000, 
  0xff000037, 0xfe0000d8, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xfe0000f5, 0xff000093, 0x00000000, 0x00000000, 
  0xff00003a, 0xfe0000f6, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xfe0000e4, 0xff000056, 0x00000000, 0x00000000, 0x00000000, 
  0xff00003c, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xfe0000cd, 0xff000029, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0xff00003f, 0xff0000ff, 0xfe0000f4, 0xfe0000a5, 0xff000008, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0xff000041, 0xfe0000eb, 0xff000078, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0xff00000f, 0xff00004e, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// block out for covering arrows
#define BLACK_HEIGHT 14
#define BLACK_WIDTH 10

// array size is 560
static const int32_t black[]  = {
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};


const char motor_state_text[][17]=
{
  "Backward       ",
  "Forward        ", 
  "Stop           "
};


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
  uint32_t voltage_attr;
  float M1_in;
  float M2_in;
  float prev_M1_in;
  float prev_M2_in;
  float inVoltage;
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
  
  float  bias;
  timer_t timer_motor;
   uint8_t speed_bar_y;
  uint8_t speed_bar_x;

} chip_state_t;

float m1_prev=9999;
float m2_prev=9999;

static void draw_state (chip_state_t *chip);
static void draw_line(chip_state_t *chip, uint32_t row, rgba_t color);
static void draw_pixel(chip_state_t *chip, uint32_t x,  uint32_t y,  rgba_t color) ;
static void draw_right_arrow(chip_state_t *chip, uint32_t x_start,  uint32_t y_start, uint8_t wipe);
static void draw_left_arrow(chip_state_t *chip, uint32_t x_start,  uint32_t y_start, uint8_t wipe);
static void draw_rectangle(chip_state_t *chip, uint32_t x_start, uint32_t y_start, uint32_t x_len, uint32_t y_len,  rgba_t color, uint8_t fill);
static void draw_speed(chip_state_t *chip, uint32_t x_start, uint32_t y_start, uint32_t x_len, uint32_t y_len,  rgba_t color, uint8_t fill) ;

// timer for graphics
static void chip_timer_event_motor(void *user_data);

void chip_init(void) {
  printf("*** Motorchip initialising...\n");
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  chip->motor_state=3; // start stopped
  chip->pin_M1 = pin_init("M1",ANALOG);
  chip->pin_M2 = pin_init("M2",ANALOG);
 
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
  chip->motor_y=1;
  chip->motor_x=5;
  // arrow and speed base position
  chip->motor_arrow_y = 5;
  // Motor arrow positions  relative to motor position
  chip->motor_right_arrow_x =chip->motor_x + 40;
  chip->motor_left_arrow_x = chip->motor_x  ;
  // bars position relative to motor
  chip-> speed_bar_y=  chip->motor_arrow_y + 20;
  chip-> speed_bar_x=chip->motor_x ;

  printf("Framebuffer: fb_w=%d, fb_h=%d\n", chip->fb_w, chip->fb_h);

  printf("Draw the speed holders ...\n");
  draw_speed(chip, chip-> speed_bar_x,chip-> speed_bar_y,50,15, chip-> purple  ,0);
  //Draw the cogs
   draw_right_arrow(chip,chip->motor_arrow_y ,chip->motor_right_arrow_x,0);
   draw_left_arrow(chip,chip->motor_arrow_y ,chip->motor_left_arrow_x,0);

const timer_config_t timer_config_motor = {
    .callback = chip_timer_event_motor,
    .user_data = chip,
  };
  chip->timer_motor = timer_init(&timer_config_motor);
  timer_start(chip->timer_motor,100000, true);


}


// graphics for motor A
void chip_timer_event_motor(void *user_data) {
  chip_state_t *chip = (chip_state_t*)user_data;
  chip->M1_in=pin_adc_read(chip->pin_M1);
  chip->M2_in=pin_adc_read(chip->pin_M2);

  if ( chip->prev_M1_in!= chip->M1_in ||  chip->prev_M1_in != chip->M1_in )
  {
   printf("NEW     -- M1=%f, M2=%f\n", chip->M1_in, chip->M2_in);
   draw_state(chip);
   chip->prev_M1_in = chip->M1_in;
   chip->prev_M2_in = chip->M2_in;
  }
}



void draw_state(chip_state_t *chip) {

  //turn off the two timers
  timer_stop(chip->timer_motor);
  if (chip->M1_in > 0 )
  {
   chip-> motor_state = 0; 
    // remove right - place left
    draw_left_arrow(chip,chip->motor_arrow_y ,chip->motor_left_arrow_x,0);
    draw_right_arrow(chip,chip->motor_arrow_y ,chip->motor_right_arrow_x,1);
  }

  if (chip->M2_in > 0 )
  {
   chip-> motor_state = 1; 
         // remove left - place right
    draw_right_arrow(chip,chip->motor_arrow_y ,chip->motor_right_arrow_x,1);
    draw_left_arrow(chip,chip->motor_arrow_y ,chip->motor_left_arrow_x,0);
  }

if (chip->M1_in == 0 && chip->M2_in == 0 )
{
   // stopped
   chip-> motor_state = 2; 
     // remove left and right
  draw_right_arrow(chip,chip->motor_arrow_y ,chip->motor_right_arrow_x,1);
  draw_left_arrow(chip,chip->motor_arrow_y ,chip->motor_left_arrow_x,1); 
}



  
  if (chip-> motor_state == 0 || chip-> motor_state == 1)
  {
 chip-> bias= 5.00 - (chip->M1_in  + chip->M2_in);
  printf("chip->M1_in= %f  chip->M2_in = %f \n",chip->M1_in,chip->M2_in);
  if (chip-> bias > 0 )
  {
    printf("Bias = %f  motor_state = %d \n",chip-> bias,chip->motor_state);
   timer_start(chip->timer_motor, (chip-> bias ) * 3000 , true);
  }
  }
     draw_speed(chip, chip-> speed_bar_x,chip-> speed_bar_y,50,15, chip-> purple  ,chip-> bias * 20);
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











