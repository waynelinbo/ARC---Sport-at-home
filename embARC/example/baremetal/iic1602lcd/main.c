/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without
modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice,
this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors
may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */
#include <stdio.h>

#include "embARC.h"
#include "embARC_debug.h"
#include "iic1602lcd.h"

IIC1602LCD_OBJ_PTR lcd_obj;

const char degreeC[3] = {0xDF, 'C', '\0'};

/**
 * \brief	Test hardware board without any peripheral
 */
int main(void) {
  IIC1602LCD_CONFIG_T lcd_config = {
      .iic_addr = 0x27, // May be 0x3F according to your PCF8574
      .En = 2,
      .Rw = 1,
      .Rs = 0,
      .d4 = 4,
      .d5 = 5,
      .d6 = 6,
      .d7 = 7,
      .backlighPin = 3,
      .pol = BACKLIGHT_POL_POSITIVE};
  lcd_obj = iic1602lcd_getobj(lcd_config);
  if (lcd_obj == NULL) {
    goto error_exit;
  }
  lcd_obj->set_backlight(BACKLIGHT_STATE_ON);

  char str[32] = {0};
  const int32_t temp = 32;

  lcd_obj->set_cursor(0, 0);
  snprintf(str, sizeof(str), "Temp: %d%s", temp, degreeC);
  lcd_obj->print(str, strlen(str));

  lcd_obj->set_cursor(0, 1);
  snprintf(str, sizeof(str), "This is a demo");
  lcd_obj->print(str, strlen(str));
  snprintf(str, sizeof(str), "!");
  lcd_obj->print(str, strlen(str));

  while (1) {
  }

  return E_SYS;
error_exit:
  EMBARC_PRINTF("ERROR\r\n");
  return E_SYS;
}
