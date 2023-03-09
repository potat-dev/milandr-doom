// Утилиты нижнего уровня для работы с LCD-экраном МЭЛТ MT-12864J

#include "lcd.h"

/* Контекст подсистемы отрисовки */
LCD_Crystal CurrentCrystal = LCD_CRYSTAL1; /* Текущий выбранный кристал */
LCD_Method CurrentMethod; /* Текущий метод отрисовки */

/* Порты LCD-кристаллов */
const LCD_Ports CrystalPorts[NUM_LCD_CRYSTALS] = {
    /* Кристалл #1 */
    {0x18100000,  /* LCD_DATA1 */
     0x10100000}, /* LCD_CMD1  */
    /* Кристалл #2 */
    {0x18200000, /* LCD_DATA2 */
     0x10200000} /* LCD_CMD2  */
};

/* Утилиты работы с LCD */

void ResetLCD(void) {
  u32 i;

  PORTC->RXTX = 0x00000200;
  PORTC->OE = 0x00000200;
  for (i = 0; i < 255; i++) PORTC->RXTX = 0x00000000;
  PORTC->RXTX = 0x00000200;
}

void InitPortLCD(void) {
  PORTA->FUNC = 0x00005555; /* Main Function для DATA[7:0] */
  PORTA->ANALOG = 0xFFFF;   /* Digital */
  PORTA->PWR = 0x00005555;  /* Fast */

  PORTE->FUNC = 0x00400500; /* Main Function для ADDR[20,21,27] */
  PORTE->ANALOG = 0xFFFF;   /* Digital */
  PORTE->PWR = 0x00400500;  /* Fast */

  PORTC->FUNC = 0x15504010; /* Main Function для RESET WE & CLOCK & KEYS*/
  PORTC->ANALOG = 0xFFFF;   /* Digital */
  PORTC->PWR = 0x0008C010;  /* Fast */
}

void InitExtBus(void) { EXT_BUS_CNTRL->EXT_BUS_CONTROL = 0x0000A001; }

void SetCrystal(LCD_Crystal num) {
  // PORTE->RXTX = ((num + 1) << 4);
  // PORTE->OE = 0x30;
  // Pause();
  CurrentCrystal = num;
}

void WriteLCD_Cmd(u32 val) {
  LCD_CMD(CurrentCrystal) = val;
  Pause();
}

void WriteLCD_Data(u32 val) {
  LCD_DATA(CurrentCrystal) = val;
  Pause();
}

u32 ReadLCD_Cmd(void) {
  u32 ret = LCD_CMD(CurrentCrystal);
  Pause();
  return ret;
}

u32 ReadLCD_Data() {
  u32 ret;
  LCD_DATA(CurrentCrystal); /* Первое чтение - необходимо для получения
                               корректных данных */
  Pause();
  ret = LCD_DATA(CurrentCrystal);
  Pause();
  return ret;
}

void LCD_INIT(void) {
  u32 crystal;

  RST_CLK->PER_CLOCK = 0xFFFFFFFF;

  InitPortLCD(); /* Инициализация портов внешней шины и выводов для работы с
                    экраном */
  InitExtBus();  /* Инициализация внешней шины */
  ResetLCD();    /* Прошраммный сброс экрана */

  /* Инициализация всех кристаллов */
  for (crystal = LCD_CRYSTAL1; crystal < NUM_LCD_CRYSTALS; crystal++) {
    SetCrystal((LCD_Crystal)crystal);
    WAIT_BUSY;
    LCD_ON;
    WAIT_ON;
    LCD_START_LINE(0);
  }
}

void LCD_CLS(void) {
  u32 i, j, crystal;

  /* Очистка данных для всех кристаллов */
  for (crystal = LCD_CRYSTAL1; crystal < NUM_LCD_CRYSTALS; crystal++) {
    SetCrystal((LCD_Crystal)crystal);
    WAIT_BUSY;
    LCD_OFF;
    LCD_SET_ADDRESS(0);
    for (i = 0; i < 8; i++) {
      LCD_SET_PAGE(i);
      for (j = 0; j < 64; j++) WriteLCD_Data(0x00);
    }
    LCD_ON;
  }
}
