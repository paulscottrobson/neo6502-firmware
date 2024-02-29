/*\
|*| Convenience macros for Neo6502 applications programming
|*| SPDX-License-Identifier: CC0-1.0
\*/


/* Neo6502 Kernel API control addresses */

uint16_t* ControlPort         = (uint16_t*)0xFF00 ;
uint16_t* API_COMMAND_ADDR    = (uint16_t*)0xFF00 ; // function group address
uint16_t* API_FUNCTION_ADDR   = (uint16_t*)0xFF01 ; // function address
uint16_t* API_PARAMETERS_ADDR = (uint16_t*)0xFF04 ; // function parameters base address (+0..7)


/* Neo6502 Kernel API control codes (see api.pdf) */

/*\
|*| Console functions (Group 2)
\*/
uint8_t API_GROUP_CONSOLE = (uint8_t )0x02 ; // API function group
uint8_t API_FN_WRITE_CHAR = (uint8_t )0x06 ; // API function
uint8_t API_FN_CURSOR_POS = (uint8_t )0x07 ; // API function

/*\
|*| Sound functions (Group 8)
\*/
uint8_t API_GROUP_SOUND   = (uint8_t )0x08 ; // API function group
uint8_t API_FN_PLAY_SOUND = (uint8_t )0x05 ; // API function
uint8_t API_SOUND_CH_00   = (uint8_t )0x00 ; // API parameter (channel index)
uint8_t API_SFX_COIN      = (uint8_t )0x06 ; // API parameter (sound effect)


/* Neo6502 Kernel API functions */

/*\
|*| Play sound effect - (API Group 8, Function 5)
\*/
void SoundEffect(uint8_t sound_ch , uint8_t sound_effect)
{
  *API_FUNCTION_ADDR     = API_FN_PLAY_SOUND ;
  API_PARAMETERS_ADDR[0] = sound_ch          ;
  API_PARAMETERS_ADDR[1] = sound_effect      ;
  *API_COMMAND_ADDR      = API_GROUP_SOUND   ;
}

/*\
|*| Set cursor position - (API Group 2, Function 7)
\*/
void SetCursorPosition(uint8_t x_pos , uint8_t y_pos)
{
  *API_FUNCTION_ADDR     = API_FN_CURSOR_POS ;
  API_PARAMETERS_ADDR[0] = x_pos             ;
  API_PARAMETERS_ADDR[1] = y_pos             ;
  *API_COMMAND_ADDR      = API_GROUP_CONSOLE ;
}

/*\
|*| Write character to console - (API Group 2, Function 6)
\*/
int write(int /* fildes */ , const unsigned char* buf , unsigned count)
{
  while (count--)
  {
    while(*API_COMMAND_ADDR) {}

    *API_FUNCTION_ADDR   = API_FN_WRITE_CHAR ;
    *API_PARAMETERS_ADDR = *buf++            ;
    *API_COMMAND_ADDR    = API_GROUP_CONSOLE ;
  }

  return 0 ;
}
