#include <project.h>
#include <stdio.h>
#include <u8x8.h>

uint8_t psoc_gpio_and_delay_callback(u8x8_t *u8x8, uint8_t msg, 
                                     uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg,
                         uint8_t arg_int, void *arg_ptr);


int main(void) {

  u8x8_t EM2Display;

  CyGlobalIntEnable;
    
  I2C_DISPLAY_Start();

  u8x8_Setup(&EM2Display, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c,
    u8x8_byte_hw_i2c, psoc_gpio_and_delay_callback);

  u8x8_InitDisplay(&EM2Display);
  u8x8_SetPowerSave(&EM2Display, 0);
  u8x8_ClearDisplay(&EM2Display);
  u8x8_SetFont(&EM2Display, u8x8_font_amstrad_cpc_extended_f);

  u8x8_DrawString(&EM2Display, 1, 1, "HvA HBO-ICT");
  u8x8_DrawString(&EM2Display, 1, 3, "TI EM2");
  u8x8_DrawString(&EM2Display, 1, 5, "Ready to run." );

    
    


}

uint8_t psoc_gpio_and_delay_callback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {

  (void) u8x8;
  (void) arg_ptr;

  switch(msg) {

    case U8X8_MSG_GPIO_AND_DELAY_INIT:
    break;

    case U8X8_MSG_DELAY_MILLI:
    CyDelay(arg_int);
    break;

    case U8X8_MSG_DELAY_10MICRO:
    CyDelayUs(10);
    break;

    case U8X8_MSG_DELAY_100NANO:
    CyDelayCycles(100);
    break;

  }

  return 1;

}

/*
 PSoC I2C Hardware Interface for U8X8
*/
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {

  uint8_t *data;

  switch(msg) {

    case U8X8_MSG_BYTE_SEND:
    data = (uint8_t *)arg_ptr;

    while(arg_int > 0) {
      (void)I2C_DISPLAY_MasterWriteByte(*data);
      data++;
      arg_int--;
    }

    break;

    case U8X8_MSG_BYTE_INIT:
    break;

    case U8X8_MSG_BYTE_SET_DC:
    break;

    case U8X8_MSG_BYTE_START_TRANSFER:
    (void)I2C_DISPLAY_MasterSendStart(u8x8_GetI2CAddress(u8x8) >> 1,
                                      I2C_DISPLAY_WRITE_XFER_MODE);
    break;

    case U8X8_MSG_BYTE_END_TRANSFER:
    (void)I2C_DISPLAY_MasterSendStop();
    break;

    default:
    return 0;

  }

  return 1;

}

/* [] END OF FILE */

