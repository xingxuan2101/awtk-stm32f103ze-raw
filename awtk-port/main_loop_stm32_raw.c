/**
 * file:   main_loop_stm32_raw.c
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  main loop for stm32
 *
 * copyright (c) 2018 - 2018 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-02-17 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "button.h"
#include "delay.h"
#include "flash.h"
#include "gui.h"
#include "led.h"
#include "rtc.h"
#include "sys.h"
#include "tim.h"
#include "touch.h"
#include "usart.h"
#include "stdlib.h"
#include "lcd_driver.h"

#include "base/idle.h"
#include "base/timer.h"
#include "lcd/lcd_reg.h"
#include "base/main_loop.h"
#include "base/platform.h"
#include "base/event_queue.h"
#include "base/font_manager.h"
#include "base/window_manager.h"

static ret_t post_touch_events(main_loop_t* l, bool_t pressed, xy_t x, xy_t y);

ret_t platform_disaptch_input(main_loop_t* l) { return RET_OK; }

static lcd_t* platform_create_lcd(wh_t w, wh_t h) { return lcd_reg_create(w, h); }

void TIM3_IRQHandler(void) {
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    if (TOUCH_Scan() == 0) {
      post_touch_events(main_loop(), TRUE, TouchData.lcdx, TouchData.lcdy);
    } else {
      post_touch_events(main_loop(), FALSE, TouchData.lcdx, TouchData.lcdy);
    }
  }
}

#include "main_loop/main_loop_raw.inc"