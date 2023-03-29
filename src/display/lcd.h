// Утилиты нижнего уровня для работы с LCD-экраном МЭЛТ MT-12864J

#ifndef __LCD_H
#define __LCD_H

#include "../types.h"

// Номера LCD-кристаллов
typedef enum {
  LCD_CRYSTAL1 = 0,
  LCD_CRYSTAL2 = 1,
  NUM_LCD_CRYSTALS
} LCD_Crystal;

// Флаги состояния LCD
typedef enum {
  BUSY = 7,
  ONOFF = 5,
  RESET_St = 4,
} LCD_Status;

// Методы вывода битов и байтов
typedef enum {
  MET_OR = 0,
  MET_XOR = 1,
  MET_NOT_OR = 2,
  MET_NOT_XOR = 3,
  MET_AND = 4,
  MET_FIX = 5,
} LCD_Method;

// Рарешение экрана LCD (в пикселях)
#define MAX_X 127
#define MAX_Y 63

// Порты LCD-кристаллов
typedef struct tag_LCD_Ports {
  u32 Data;
  u32 Cmd;
} LCD_Ports;

extern const LCD_Ports CrystalPorts[NUM_LCD_CRYSTALS];

#define LCD_DATA(x) (*((vu32 *)CrystalPorts[x].Data))
#define LCD_CMD(x) (*((vu32 *)CrystalPorts[x].Cmd))

// Контекст подсистемы отрисовки
extern LCD_Crystal CurrentCrystal;  // Текущий выбранный кристал
extern LCD_Method CurrentMethod;  // Текущий метод отрисовки

// Утилиты работы с LCD
void ResetLCD(void);
void InitPortLCD(void);
void InitExtBus(void);
void LCD_INIT(void);

// write data to LCD
void SetCrystal(LCD_Crystal num);
void WriteLCD_Cmd(u32 val);
void WriteLCD_Data(u32 val);
void LCD_CLS(void);

// read data from LCD
u32 ReadLCD_Cmd(void);
u32 ReadLCD_Data(void);

static u32 GetStatus(void);
static void WaitStatus(LCD_Status status);
static void Pause(void);

static void WaitStatus(LCD_Status status) {
  u32 stat;
  for (stat = GetStatus(); stat == (1 << status); stat = GetStatus())
    ;
}

static void Pause(void) {
  u32 i;
  for (i = 15; i > 0; i--)
    ;
}

static u32 GetStatus(void) {
  u32 ret;

  Pause();
  ret = ReadLCD_Cmd();
  Pause();
  return ret;
}

// --- LCD buffer --- //

#define CRYSTAL_WIDTH 64
#define PAGE_HEIGHT 8
#define PAGE_COUNT 8
#define CRYSTAL_COUNT 2

// buffer struct
typedef struct LCD_Buffer {
  int8_t buffer[CRYSTAL_COUNT][PAGE_COUNT][CRYSTAL_WIDTH];
  int8_t prev_buffer[CRYSTAL_COUNT][PAGE_COUNT][CRYSTAL_WIDTH];
  int8_t changes[CRYSTAL_COUNT][CRYSTAL_WIDTH];
  // each byte's bits indicates changes in column's rows
} LCD_Buffer;

// TODO: remove changes array

extern LCD_Buffer Buffer;

void InitBuffer(void);
void ClearBuffer(void);
void SetBufferByte(int8_t addr, int8_t page, int8_t byte);
void SetBufferPixel(int8_t x, int8_t y, int8_t pixel);
void UpdateBufferChanges(void);
void UpdatePrevBuffer(void);
void RestorePrevBuffer(void);
void DrawBuffer(bool clearCurrentBuffer);
void DrawBuffer(void);
void InvertBuffer(void);

int8_t GetBufferByte(int8_t addr, int8_t page);

// Утилиты для работы с буфером

#define WAIT_BUSY WaitStatus(BUSY)
#define WAIT_RESET WaitStatus(RESET)
#define WAIT_ON WaitStatus(ONOFF)

// Специфические команды управления LCD
#define LCD_ON WriteLCD_Cmd(0x3F)
#define LCD_OFF WriteLCD_Cmd(0x3E)
#define LCD_START_LINE(x) WriteLCD_Cmd(0xC0 | x)
#define LCD_SET_PAGE(x) WriteLCD_Cmd(0xB8 | x)
#define LCD_SET_ADDRESS(x) WriteLCD_Cmd(0x40 | x)

#endif /* __LCD_H */
