// Утилиты нижнего уровня для работы с LCD-экраном МЭЛТ MT-12864J

#include "lcd.h"

#include "string.h"

// Контекст подсистемы отрисовки
LCD_Crystal CurrentCrystal = LCD_CRYSTAL1;  // Текущий выбранный кристал
LCD_Method CurrentMethod = MET_OR;  // Текущий метод отрисовки

// Порты LCD-кристаллов
const LCD_Ports CrystalPorts[NUM_LCD_CRYSTALS] = {
    {0x18100000,   // LCD_DATA1
     0x10100000},  // LCD_CMD1
    {0x18200000,   // LCD_DATA2
     0x10200000}   // LCD_CMD2
};

// Утилиты работы с LCD

void ResetLCD(void) {
  u32 i;

  PORTC->RXTX = 0x00000200;
  PORTC->OE = 0x00000200;
  for (i = 0; i < 255; i++) PORTC->RXTX = 0x00000000;
  PORTC->RXTX = 0x00000200;
}

void InitPortLCD(void) {
  PORTA->FUNC = 0x00005555;  // Main Function для DATA[7:0]
  PORTA->ANALOG = 0xFFFF;    // Digital
  PORTA->PWR = 0x00005555;   // Fast

  PORTE->FUNC = 0x00400500;  // Main Function для ADDR[20,21,27]
  PORTE->ANALOG = 0xFFFF;    // Digital
  PORTE->PWR = 0x00400500;   // Fast

  PORTC->FUNC = 0x15504010;  // Main Function для RESET WE & CLOCK & KEYS
  PORTC->ANALOG = 0xFFFF;    // Digital
  PORTC->PWR = 0x0008C010;   // Fast
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
  u32 ret = LCD_DATA(CurrentCrystal);
  // Первое чтение - необходимо для получения корректных данных
  Pause();
  ret = LCD_DATA(CurrentCrystal);
  Pause();
  return ret;
}

void LCD_INIT(void) {
  u32 crystal;

  RST_CLK->PER_CLOCK = 0xFFFFFFFF;

  InitPortLCD();  // Инициализация внешней шины и выводов для работы с экраном
  InitExtBus();   // Инициализация внешней шины
  ResetLCD();     // Прошраммный сброс экрана

  // Инициализация всех кристаллов
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

  // Очистка данных для всех кристаллов
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

// Утилиты работы с буфером

// global buffer
LCD_Buffer Buffer;

// init buffer (set all bytes to 0)
void InitBuffer() {
  memset(Buffer.buffer, 0, sizeof(Buffer.buffer));
  memset(Buffer.prev_buffer, 0, sizeof(Buffer.prev_buffer));
  memset(Buffer.changes, 0, sizeof(Buffer.changes));
}

// set whole byte in buffer
void SetBufferByte(int8_t addr, int8_t page, int8_t byte) {
  Buffer.buffer[addr / CRYSTAL_WIDTH][page % PAGE_COUNT][addr % CRYSTAL_WIDTH] =
      byte;
}

// set pixel in buffer
void SetBufferPixel(int8_t x, int8_t y, int8_t pixel) {
  if (x < 0 || x >= CRYSTAL_WIDTH * CRYSTAL_COUNT || y < 0 ||
      y >= PAGE_HEIGHT * PAGE_COUNT)
    return;

  int8_t crystal = x / CRYSTAL_WIDTH;
  int8_t page = y / PAGE_HEIGHT;
  int8_t addr = x % CRYSTAL_WIDTH;
  int8_t bit = y % PAGE_HEIGHT;  // which bit of byte to set
  if (pixel) {
    Buffer.buffer[crystal][page][addr] |= (1 << bit);
  } else {
    Buffer.buffer[crystal][page][addr] &= ~(1 << bit);
  }
}

// function to update changes (optimal - use bit operations)
void UpdateBufferChanges() {
  int i, j, k;
  for (i = 0; i < CRYSTAL_COUNT; i++) {
    for (j = 0; j < CRYSTAL_WIDTH; j++) {
      Buffer.changes[i][j] = 0;
      for (k = 0; k < PAGE_COUNT; k++) {
        if (Buffer.buffer[i][k][j] != Buffer.prev_buffer[i][k][j]) {
          Buffer.changes[i][j] |= (1 << k);
        }
      }
    }
  }
}

void ClearBuffer(void) { memset(Buffer.buffer, 0, sizeof(Buffer.buffer)); }

void UpdatePrevBuffer(bool clearBuffer) {
  memcpy(Buffer.prev_buffer, Buffer.buffer, sizeof(Buffer.buffer));
  if (clearBuffer) ClearBuffer();
}

void RestorePrevBuffer(void) {
  memcpy(Buffer.buffer, Buffer.prev_buffer, sizeof(Buffer.buffer));
}

void DrawBuffer(bool clearBuffer) {
  int i, j, k, mask;
  int next = CRYSTAL_WIDTH;
  // update changes
  UpdateBufferChanges();
  // draw only changed bytes
  for (i = 0; i < CRYSTAL_COUNT; i++) {
    SetCrystal((LCD_Crystal)i);
    WAIT_BUSY;
    LCD_OFF;  // выключаем кристалл
    LCD_SET_ADDRESS(0);
    for (k = 0; k < PAGE_COUNT; k++) {
      LCD_SET_PAGE(k);
      mask = 1 << k;
      for (j = 0; j < CRYSTAL_WIDTH; j++) {
        // send data to crystal
        // iterate through rows (0-7) and check if bit is set in changes[i][j]
        if (Buffer.changes[i][j] & mask) {
          // set address only if it's not in sequence
          if (j != next) {
            LCD_SET_ADDRESS(j);
            next = j;
          }
          next++;
          // send row data
          WriteLCD_Data(Buffer.buffer[i][k][j]);
        }
      }
    }
    LCD_ON;  // не забываем включить кристалл обратно
  }
  UpdatePrevBuffer(clearBuffer);  // current buffer is now previous buffer
}

void DrawBuffer() { DrawBuffer(true); }

void InvertBuffer() {
  int i, j, k;
  for (i = 0; i < CRYSTAL_COUNT; i++) {
    for (j = 0; j < CRYSTAL_WIDTH; j++) {
      for (k = 0; k < PAGE_COUNT; k++) {
        Buffer.buffer[i][k][j] = ~Buffer.buffer[i][k][j];
      }
    }
  }
}

int8_t GetBufferByte(int8_t addr, int8_t page) {
  return Buffer
      .buffer[addr / CRYSTAL_WIDTH][page % PAGE_COUNT][addr % CRYSTAL_WIDTH];
}
