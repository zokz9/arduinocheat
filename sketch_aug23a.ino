#define OLED_SOFT_BUFFER_64
#include <GyverOLED.h>
#include <avr/pgmspace.h>
#include <GyverButton.h>
#include <EEPROM.h>

// --- ПИНЫ КНОПОК ---
#define BTN_UP_PIN     2
#define BTN_DOWN_PIN   3
#define BTN_A_PIN      4
#define BTN_B_PIN      5

// --- АДРЕСА НАСТРОЕК В EEPROM ---
#define EEPROM_ADDR_INVERT      0
#define EEPROM_ADDR_CONTRAST    1
#define EEPROM_ADDR_INIT_FLAG   2
#define EEPROM_ADDR_SCREENSAVER 3

// --- ЗНАЧЕНИЯ ПО УМОЛЧАНИЮ ---
#define DEFAULT_INVERT          false
#define DEFAULT_CONTRAST        127
#define DEFAULT_SCREENSAVER     0 // 0 = выкл, 1=30с, 2=1м, 3=2м, 4=5м
#define SCREENSAVER_CONTRAST    1 // Яркость в режиме ожидания

// --- ОБЪЕКТЫ КНОПОК ---
GButton btnUp(BTN_UP_PIN, HIGH_PULL, NORM_OPEN);
GButton btnDown(BTN_DOWN_PIN, HIGH_PULL, NORM_OPEN);
GButton btnA(BTN_A_PIN, HIGH_PULL, NORM_OPEN);
GButton btnB(BTN_B_PIN, HIGH_PULL, NORM_OPEN);

// --- СТРУКТУРА ПУНКТА МЕНЮ ---
struct MenuItem {
  const char* name;
  const char* info;
};

// --- ДАННЫЕ МЕНЮ (ХРАНЯТСЯ В ПЗУ) ---
const char algebra_name[] PROGMEM = "Алгебра";
const char algebra_info[] PROGMEM = "ax2+bx+c=0\nD=b2-4ac\nx=(-b+-sqrt(D))/2a\n(a+b)2=a2+2ab+b2\n(a-b)2=a2-2ab+b2\na2-b2=(a-b)(a+b)\nsqrt(a*b)=sqrt(a)*sqrt(b)\nsqrt(a/b)=sqrt(a)/sqrt(b)\nlog_a(b)=c\nlog_a(xy)=log_a(x)+log_a(y)\nlog_a(x/y)=log_a(x)-log_a(y)\nlog_a(x^n)=n*log_a(x)";
const char physics_name[] PROGMEM = "Физика";
const char physics_info[] PROGMEM = "F=m*a (Сила)\nI=U/R (Ток)\np=m*v (Импульс)\nE=m*g*h (Энергия)\nA=F*s (Работа)\nP=A/t (Мощность)\nF=G*(m1*m2)/r2..\n(Тяготение)\nT=2*3.14*sqrt(l/g)..\n(Маятник)\nPV=nRT (Газы)";
const char chemistry_name[] PROGMEM = "Химия";
const char chemistry_info[] PROGMEM = "2H2+O2=2H2O\nCH4+2O2=CO2+2H2O\nH2SO4 - серная кислота\nNaCl - поваренная соль\nH2O - вода\nCO2 - углекислый газ\nO2 - кислород\nN2 - азот\nFe - железо\nCu - медь\nZn - цинк\nMg - магний";
const char history_name[] PROGMEM = "История";
const char history_info[] PROGMEM = "Великая Отечественная\nвойна 1941-1945 гг.\nСталинградская битва\nЛенинградская блокада\nКурская битва\nБерлинская операция\nПобеда 9 мая 1945 года\nКрещение Руси 988 год\nМонголо-татары 13 век\nПетр I и реформы\nОтечественная 1812\nОктябрьская революция\nСССР 1922-1991 гг.";
const char literature_name[] PROGMEM = "Литература";
const char literature_info[] PROGMEM = "Пушкин А.С.\n\"Евгений Онегин\"\nЛермонтов М.Ю.\n\"Герой нашего времени\"\nГоголь Н.В.\n\"Мертвые души\"\nКрылов И.А. Басни\nТолстой Л.Н.\n\"Война и мир\"\nНекрасов Н.А. Поэзия\nФет А.А. Поэзия\nДостоевский Ф.М.\n\"Преступление...\"\nЧехов А.П. Рассказы";
const char geography_name[] PROGMEM = "География";
const char geography_info[] PROGMEM = "Солнечная система\nПланеты Солнца\nЗемля - наш дом\nГоры и равнины\nРеки и озера\nОкеаны Земли\nКлимат и погода\nПриродные зоны\nНаселение планеты\nМатерики Евразия\nМатерики Африка\nМатерики Сев. Америка\nМатерики Юж. Америка\nАвстралия и Антарктида";
const char biology_name[] PROGMEM = "Биология";
const char biology_info[] PROGMEM = "Клетка - основа жизни\nСтроение клетки\nТкани организма\nОрганы и системы\nОпорно-двигательная\nКровеносная система\nДыхательная система\nПищеварительная\nНервная система\nОрганы чувств\nЭндокринная система\nВыделительная\nРазмножение\nНаследственность\nЭволюция";

const MenuItem menuItems[] PROGMEM = {
  { algebra_name, algebra_info },
  { physics_name, physics_info },
  { chemistry_name, chemistry_info },
  { history_name, history_info },
  { literature_name, literature_info },
  { geography_name, geography_info },
  { biology_name, biology_info }
};
const uint8_t MENU_SIZE = sizeof(menuItems) / sizeof(menuItems[0]);

// --- ГЛАВНОЕ МЕНЮ ---
const char main_menu_answers[] PROGMEM = "Ответы";
const char main_menu_calculator[] PROGMEM = "Калькулятор";
const char main_menu_tetris[] PROGMEM = "Тетрис";
const char main_menu_settings[] PROGMEM = "Настройки";

const char* const mainMenuItems[] PROGMEM = {
  main_menu_answers, main_menu_calculator, main_menu_tetris, main_menu_settings
};
const uint8_t MAIN_MENU_SIZE = sizeof(mainMenuItems) / sizeof(mainMenuItems[0]);

// --- МЕНЮ НАСТРОЕК ---
const char settings_menu_invert[] PROGMEM = "Инверсия";
const char settings_menu_contrast[] PROGMEM = "Контраст";
const char settings_menu_screensaver[] PROGMEM = "Скринсейвер";

const char* const settingsMenuItems[] PROGMEM = {
  settings_menu_invert, settings_menu_contrast, settings_menu_screensaver
};
const uint8_t SETTINGS_MENU_SIZE = sizeof(settingsMenuItems) / sizeof(settingsMenuItems[0]);

// --- ОПЦИИ СКРИНСЕЙВЕРА ---
const char screensaver_off[] PROGMEM = "Выкл";
const char screensaver_30s[] PROGMEM = "30 сек";
const char screensaver_1m[] PROGMEM = "1 мин";
const char screensaver_2m[] PROGMEM = "2 мин";
const char screensaver_5m[] PROGMEM = "5 мин";
const char* const screensaverOptions[] PROGMEM = {
  screensaver_off, screensaver_30s, screensaver_1m, screensaver_2m, screensaver_5m
};
const uint8_t SCREENSAVER_OPTIONS_SIZE = sizeof(screensaverOptions) / sizeof(screensaverOptions[0]);

// --- ОБЪЕКТ ДИСПЛЕЯ ---
GyverOLED<SSD1306_128x64> oled;

// --- СОСТОЯНИЯ МЕНЮ ---
enum MenuState {
  STATE_SPLASH = 0, STATE_TOP_MENU, STATE_ANSWERS_MENU, STATE_INFO_VIEW,
  STATE_CALC_INPUT, STATE_TETRIS, STATE_SETTINGS_MENU, STATE_CONTRAST_ADJUST,
  STATE_SCREENSAVER_ADJUST, STATE_SCREENSAVER
};
MenuState currentState = STATE_SPLASH;

// --- НАСТРОЙКИ ---
bool invertEnabled = DEFAULT_INVERT;
uint8_t contrastLevel = DEFAULT_CONTRAST;
uint8_t screensaverTimeIndex = DEFAULT_SCREENSAVER;

// --- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ НАСТРОЕК ---
unsigned long getScreensaverDelayMs(uint8_t index) {
  switch (index) {
    case 0: return 0;
    case 1: return 30000UL;
    case 2: return 60000UL;
    case 3: return 120000UL;
    case 4: return 300000UL;
    default: return 0;
  }
}
char getCursorChar() { return '>'; } // Фиксированный курсор

// --- ПЕРЕМЕННЫЕ МЕНЮ ---
int8_t currentTopMenuIndex = 0;
int8_t currentMenuIndex = 0;
int8_t topVisibleIndex = 0;
const uint8_t LINES_PER_SCREEN = 8;
int8_t currentInfoPageIndex = 0;
int8_t currentSettingsMenuIndex = 0;
uint8_t tempContrastValue = 0; // Для временного хранения контраста

// --- ПЕРЕМЕННЫЕ СКРИНСЕЙВЕРА ---
unsigned long lastActivityTime = 0;
unsigned long screensaverTimeout = 0;

// --- ПЕРЕМЕННЫЕ КАЛЬКУЛЯТОРА ---
enum CalcState { CALC_FIRST_NUM, CALC_OPERATOR, CALC_SECOND_NUM, CALC_RESULT };
CalcState calcState = CALC_FIRST_NUM;
int8_t currentDigit = 0;
long firstNumber = 0;
long secondNumber = 0;
char operatorChar = '+';
char calcHistory[100]; // Уменьшенный буфер для экономии SRAM

// --- ПЕРЕМЕННЫЕ ТЕТРИСА ---
const uint8_t FIELD_WIDTH = 20;
const uint8_t FIELD_HEIGHT = 10;
const uint8_t BLOCK_SIZE = 6;
uint8_t playfield[FIELD_HEIGHT][FIELD_WIDTH] = {0};
int8_t currentPieceX = 0;
int8_t currentPieceY = 0;
uint8_t currentPiece[4][4];
uint8_t currentPieceType = 0;
bool gameOver = false;
unsigned long lastMoveTime = 0;
unsigned long lastAccelerateTime = 0;
const unsigned long MOVE_DELAY = 500;
const unsigned long ACCELERATE_DELAY = 30;

const uint8_t tetrominoes[7][4][4] PROGMEM = {
  {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}, // I
  {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}}, // O
  {{0,0,0,0}, {0,1,0,0}, {1,1,1,0}, {0,0,0,0}}, // T
  {{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}}, // S
  {{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}}, // Z
  {{0,0,0,0}, {1,0,0,0}, {1,1,1,0}, {0,0,0,0}}, // J
  {{0,0,0,0}, {0,0,1,0}, {1,1,1,0}, {0,0,0,0}}  // L
};

// --- ДАННЫЕ ЗАСТАВКИ ---
const uint8_t prt_bmp[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x70, 0x58, 0x4C, 0x66, 0x7E, 0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x80, 0xC0, 0xE0, 0xF0, 0x18, 0xCC, 0xE6, 0x37, 0x5E, 0x38, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0x38, 0x9E, 0xA7, 0x91, 0xE4, 0xF2, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB, 0xFD, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x80, 0xFF, 0xE0, 0x1E, 0xB3, 0x64, 0xCF, 0x1D, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xF8, 0xDE, 0x07, 0xF1, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xE3, 0xE1, 0xE1, 0xE0, 0xF0, 0xFE, 0x9F, 0x8F, 0x8F, 0xE7, 0x73, 0x79, 0x7C, 0x82, 0xF9, 0x64, 0x1E, 0xD6, 0xFE, 0x7F, 0x7F, 0x60, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1E, 0x18, 0x1B, 0x19, 0x19, 0x99, 0x99, 0x19, 0x79, 0xE1, 0x83, 0x0F, 0x3F, 0x3F, 0x3F, 0x9F, 0xCF, 0xE7, 0x33, 0x19, 0xFC, 0xFE, 0xFF, 0x03, 0xF9, 0xFC, 0x0E, 0xF7, 0xFB, 0xFD, 0xFE, 0x7F, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x7E, 0xE3, 0xC1, 0x3C, 0x02, 0xF0, 0xF9, 0x3D, 0x5F, 0x5F, 0x0F, 0xE7, 0xF3, 0x39, 0x9C, 0xCF, 0xF7, 0xFB, 0x7D, 0xFE, 0xEF, 0xF7, 0x1B, 0x8C, 0xC7, 0x63, 0x39, 0x1C, 0x0E, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x04, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xF1, 0x70, 0x3B, 0x1F, 0x0D, 0x06, 0x03, 0x01, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned long splashStartTime = 0;
const unsigned long SPLASH_DURATION = 1500;

// --- ФУНКЦИЯ ДЛЯ ПРОВЕРКИ СВОБОДНОЙ ПАМЯТИ ---
int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

// --- ФУНКЦИИ НАСТРОЕК ---
void loadSettingsFromEEPROM() {
  if (EEPROM.read(EEPROM_ADDR_INIT_FLAG) != 0xAA) {
    EEPROM.update(EEPROM_ADDR_INVERT, DEFAULT_INVERT ? 0x01 : 0x00);
    EEPROM.update(EEPROM_ADDR_CONTRAST, DEFAULT_CONTRAST);
    EEPROM.update(EEPROM_ADDR_SCREENSAVER, DEFAULT_SCREENSAVER);
    EEPROM.update(EEPROM_ADDR_INIT_FLAG, 0xAA);
    invertEnabled = DEFAULT_INVERT;
    contrastLevel = DEFAULT_CONTRAST;
    screensaverTimeIndex = DEFAULT_SCREENSAVER;
  } else {
    invertEnabled = EEPROM.read(EEPROM_ADDR_INVERT) != 0x00;
    contrastLevel = EEPROM.read(EEPROM_ADDR_CONTRAST);
    screensaverTimeIndex = EEPROM.read(EEPROM_ADDR_SCREENSAVER);
  }
  oled.invertDisplay(invertEnabled);
  oled.setContrast(contrastLevel);
  screensaverTimeout = getScreensaverDelayMs(screensaverTimeIndex);
  lastActivityTime = millis();
}

void saveSettingsToEEPROM() {
  EEPROM.update(EEPROM_ADDR_INVERT, invertEnabled ? 0x01 : 0x00);
  EEPROM.update(EEPROM_ADDR_CONTRAST, contrastLevel);
  EEPROM.update(EEPROM_ADDR_SCREENSAVER, screensaverTimeIndex);
  if (EEPROM.read(EEPROM_ADDR_INIT_FLAG) != 0xAA) EEPROM.update(EEPROM_ADDR_INIT_FLAG, 0xAA);
  screensaverTimeout = getScreensaverDelayMs(screensaverTimeIndex);
}

// --- ФУНКЦИИ ТЕТРИСА ---
void spawnNewPiece() {
  currentPieceType = random(7);
  for (uint8_t y = 0; y < 4; y++)
    for (uint8_t x = 0; x < 4; x++)
      currentPiece[y][x] = pgm_read_byte(&tetrominoes[currentPieceType][y][x]);
  currentPieceX = FIELD_WIDTH - 4;
  currentPieceY = FIELD_HEIGHT / 2 - 2;
  if (!canPlacePiece()) gameOver = true;
}

bool canPlacePiece() {
  for (uint8_t y = 0; y < 4; y++)
    for (uint8_t x = 0; x < 4; x++) {
      if (currentPiece[y][x]) {
        int newX = currentPieceX + x, newY = currentPieceY + y;
        if (newX < 0 || newX >= FIELD_WIDTH || newY < 0 || newY >= FIELD_HEIGHT || (newX >= 0 && playfield[newY][newX]))
          return false;
      }
    }
  return true;
}

void placePiece() {
  for (uint8_t y = 0; y < 4; y++)
    for (uint8_t x = 0; x < 4; x++) {
      if (currentPiece[y][x]) {
        int newX = currentPieceX + x, newY = currentPieceY + y;
        if (newX >= 0 && newX < FIELD_WIDTH && newY >= 0 && newY < FIELD_HEIGHT)
          playfield[newY][newX] = 1;
      }
    }
}

void clearLines() {
  for (int8_t x = 0; x < FIELD_WIDTH; x++) {
    bool isFull = true;
    for (uint8_t y = 0; y < FIELD_HEIGHT; y++) { if (!playfield[y][x]) { isFull = false; break; } }
    if (isFull) {
      for (int8_t col = x; col < FIELD_WIDTH - 1; col++)
        for (uint8_t y = 0; y < FIELD_HEIGHT; y++) playfield[y][col] = playfield[y][col + 1];
      for (uint8_t y = 0; y < FIELD_HEIGHT; y++) playfield[y][FIELD_WIDTH - 1] = 0;
      x--;
    }
  }
}

void rotatePiece() {
  uint8_t temp[4][4];
  for (uint8_t y = 0; y < 4; y++) for (uint8_t x = 0; x < 4; x++) temp[x][3 - y] = currentPiece[y][x];
  for (uint8_t y = 0; y < 4; y++) for (uint8_t x = 0; x < 4; x++) currentPiece[y][x] = temp[y][x];
  if (!canPlacePiece()) {
    for (uint8_t y = 0; y < 4; y++) for (uint8_t x = 0; x < 4; x++) temp[x][3 - y] = currentPiece[y][x];
    for (uint8_t y = 0; y < 4; y++) for (uint8_t x = 0; x < 4; x++) currentPiece[y][x] = temp[y][x];
  }
}

void displayTetris() {
  oled.clear();
  oled.setScale(1);
  for (uint8_t y = 0; y < FIELD_HEIGHT; y++)
    for (uint8_t x = 0; x < FIELD_WIDTH; x++) {
      if (playfield[y][x]) {
        int drawX = x * BLOCK_SIZE, drawY = y * BLOCK_SIZE;
        if (drawX < 128 && drawY < 64)
          oled.rect(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1, OLED_FILL);
      }
    }
  for (uint8_t y = 0; y < 4; y++)
    for (uint8_t x = 0; x < 4; x++) {
      if (currentPiece[y][x]) {
        int drawX = (currentPieceX + x) * BLOCK_SIZE, drawY = (currentPieceY + y) * BLOCK_SIZE;
        if (drawX >= 0 && drawX < 128 && drawY >= 0 && drawY < 64)
          oled.rect(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1, OLED_FILL);
      }
    }
  if (gameOver) {
    oled.setCursor(0, 3); oled.print(F("Game Over"));
    oled.setCursor(0, 4); oled.print(F("A: Restart"));
    oled.setCursor(0, 5); oled.print(F("Hold A: Menu"));
  }
  oled.update();
}

void handleTetrisButtons() {
  btnUp.tick(); btnDown.tick(); btnA.tick(); btnB.tick();
  if (gameOver) {
    if (btnA.isClick()) {
      for (uint8_t y = 0; y < FIELD_HEIGHT; y++) for (uint8_t x = 0; x < FIELD_WIDTH; x++) playfield[y][x] = 0;
      gameOver = false; spawnNewPiece(); lastMoveTime = millis(); lastAccelerateTime = 0;
      displayTetris(); lastActivityTime = millis();
    }
    if (btnA.isHold()) { currentState = STATE_TOP_MENU; displayTopMenu(); lastActivityTime = millis(); }
    return;
  }
  if (btnUp.isClick()) { currentPieceY--; if (!canPlacePiece()) currentPieceY++; displayTetris(); lastActivityTime = millis(); }
  if (btnDown.isClick()) { currentPieceY++; if (!canPlacePiece()) currentPieceY--; displayTetris(); lastActivityTime = millis(); }
  if (btnA.isClick()) { rotatePiece(); displayTetris(); lastActivityTime = millis(); }
  if (btnA.isHold()) { currentState = STATE_TOP_MENU; displayTopMenu(); lastActivityTime = millis(); return; }
  if (btnB.state()) {
    if (millis() - lastAccelerateTime > ACCELERATE_DELAY) {
      currentPieceX--; if (!canPlacePiece()) { currentPieceX++; placePiece(); clearLines(); spawnNewPiece(); if (gameOver) { displayTetris(); return; } }
      displayTetris(); lastAccelerateTime = millis(); lastActivityTime = millis();
    }
  }
  if (millis() - lastMoveTime >= MOVE_DELAY) {
    currentPieceX--; if (!canPlacePiece()) { currentPieceX++; placePiece(); clearLines(); spawnNewPiece(); if (gameOver) { displayTetris(); return; } }
    lastMoveTime = millis(); displayTetris();
  }
}

// --- ДРУГИЕ ФУНКЦИИ ---
int getTotalPages(uint8_t menuIndex) {
  PGM_P infoPtr = (PGM_P)pgm_read_ptr(&(menuItems[menuIndex].info));
  int lines = 1; PGM_P ptr = infoPtr;
  while (pgm_read_byte(ptr) != '\0') { if (pgm_read_byte(ptr) == '\n') lines++; ptr++; }
  return (lines + LINES_PER_SCREEN - 1) / LINES_PER_SCREEN;
}

void displaySplash() {
  oled.clear();
  oled.drawBitmap(-5, 0, prt_bmp, 63, 64);
  oled.setCursor(80, 7); oled.print(F("By ZoKz"));
  oled.setCursor(55, 2); oled.print(F("Электронная"));
  oled.setCursor(61, 3); oled.print(F("Шпаргалка"));
  oled.update(); splashStartTime = millis(); lastActivityTime = millis();
}

void displayTopMenu() {
  oled.clear(); oled.home(); oled.autoPrintln(false);
  for (uint8_t i = 0; i < MAIN_MENU_SIZE; i++) {
    oled.setCursor(0, i);
    if (i == currentTopMenuIndex) { oled.print(getCursorChar()); oled.print(F(" ")); }
    else oled.print(F("  "));
    char buffer[25]; PGM_P ptr = (PGM_P)pgm_read_ptr(&mainMenuItems[i]);
    strcpy_P(buffer, ptr); oled.println(buffer);
  }
  oled.update();
}

void displayAnswersMenu() {
  oled.clear(); oled.home(); oled.autoPrintln(false);
  uint8_t itemsToShow = min(LINES_PER_SCREEN, static_cast<uint8_t>(MENU_SIZE - topVisibleIndex));
  for (uint8_t i = 0; i < itemsToShow; i++) {
    uint8_t itemIndex = topVisibleIndex + i;
    oled.setCursor(0, i);
    if (itemIndex == currentMenuIndex) { oled.print(getCursorChar()); oled.print(F(" ")); }
    else oled.print(F("  "));
    char buffer[25]; PGM_P ptr = (PGM_P)pgm_read_ptr(&(menuItems[itemIndex].name));
    strcpy_P(buffer, ptr); oled.println(buffer);
  }
  oled.update();
}

void displayInfo() {
  PGM_P infoPtr = (PGM_P)pgm_read_ptr(&(menuItems[currentMenuIndex].info));
  oled.clear(); oled.home(); oled.autoPrintln(false); oled.setScale(1);
  PGM_P textPtr = infoPtr; int currentPage = 0; const uint8_t MAX_LINES_PER_PAGE = LINES_PER_SCREEN; uint8_t currentLineOnPage = 0;
  while (currentPage < currentInfoPageIndex && pgm_read_byte(textPtr) != '\0') {
    char c = pgm_read_byte(textPtr);
    if (c == '\n') { currentLineOnPage++; if (currentLineOnPage >= MAX_LINES_PER_PAGE) { currentPage++; currentLineOnPage = 0; } }
    textPtr++;
  }
  if (currentPage < currentInfoPageIndex) { oled.update(); return; }
  currentLineOnPage = 0; oled.setCursor(0, currentLineOnPage);
  while (pgm_read_byte(textPtr) != '\0' && currentLineOnPage < MAX_LINES_PER_PAGE) {
    char c = pgm_read_byte(textPtr);
    if (c == '\n') { currentLineOnPage++; if (currentLineOnPage < MAX_LINES_PER_PAGE) oled.setCursor(0, currentLineOnPage); else break; }
    else oled.print(c);
    textPtr++;
  }
  oled.update();
}

void displayCalculator() {
  oled.clear(); oled.home(); oled.autoPrintln(false); oled.setScale(1);
  // Подсчет строк в истории
  int totalLines = 0;
  char *ptr = calcHistory;
  while (*ptr) { if (*ptr++ == '\n') totalLines++; }
  if (calcHistory[0] != '\0') totalLines++; // Последняя строка без \n
  const uint8_t maxHistoryLines = LINES_PER_SCREEN - 1;
  int skipLines = max(0, totalLines - maxHistoryLines);
  int currentLine = 0;
  ptr = calcHistory;
  char lineBuf[20]; // Уменьшенный буфер для строки
  int idx = 0;
  while (*ptr && currentLine < totalLines) {
    if (*ptr == '\n') {
      lineBuf[idx] = '\0';
      if (currentLine >= skipLines) {
        oled.setCursor(0, currentLine - skipLines);
        oled.print(lineBuf);
      }
      currentLine++;
      idx = 0;
      ptr++;
      continue;
    }
    if (idx < sizeof(lineBuf) - 1) lineBuf[idx++] = *ptr;
    ptr++;
  }
  if (idx > 0 && currentLine >= skipLines) {
    lineBuf[idx] = '\0';
    oled.setCursor(0, currentLine - skipLines);
    oled.print(lineBuf);
  }
  // Отображение текущего ввода
  oled.setCursor(0, LINES_PER_SCREEN - 1);
  char numBuf[8]; // Буфер для чисел
  if (calcState == CALC_FIRST_NUM) {
    ltoa(firstNumber, numBuf, 10);
    oled.print(numBuf); oled.print(' '); oled.print((char)('0' + currentDigit));
  } else if (calcState == CALC_OPERATOR) {
    ltoa(firstNumber, numBuf, 10);
    oled.print(numBuf); oled.print(' '); oled.print(operatorChar);
  } else if (calcState == CALC_SECOND_NUM) {
    ltoa(firstNumber, numBuf, 10);
    oled.print(numBuf); oled.print(' '); oled.print(operatorChar); oled.print(' ');
    ltoa(secondNumber, numBuf, 10);
    oled.print(numBuf); oled.print(' '); oled.print((char)('0' + currentDigit));
  } else if (calcState == CALC_RESULT) {
    oled.print(F(""));
  }
  oled.update();
}

void displaySettingsMenu() {
  oled.clear(); oled.home(); oled.autoPrintln(false);
  for (uint8_t i = 0; i < SETTINGS_MENU_SIZE; i++) {
    oled.setCursor(0, i);
    if (i == currentSettingsMenuIndex) { oled.print(getCursorChar()); oled.print(F(" ")); }
    else oled.print(F("  "));
    char buffer[25]; PGM_P ptr = (PGM_P)pgm_read_ptr(&settingsMenuItems[i]);
    strcpy_P(buffer, ptr); oled.println(buffer);
  }
  oled.update();
}

void displayContrastAdjust() {
  oled.clear(); oled.home(); oled.autoPrintln(false);
  oled.setCursor(0, 0); oled.print(F("Контраст: ")); oled.print(tempContrastValue);
  oled.setCursor(0, 2); oled.print(F("Up/Down: Изменить"));
  oled.setCursor(0, 3); oled.print(F("A: Применить"));
  oled.setCursor(0, 4); oled.print(F("B: Отмена"));
  oled.update();
}

void displayScreensaverAdjust() {
  oled.clear(); oled.home(); oled.autoPrintln(false);
  oled.setCursor(0, 0); oled.print(F("Скринсейвер:"));
  oled.setCursor(0, 1);
  char buffer[15]; PGM_P ptr = (PGM_P)pgm_read_ptr(&screensaverOptions[screensaverTimeIndex]);
  strcpy_P(buffer, ptr); oled.println(buffer);
  oled.setCursor(0, 3); oled.print(F("Up/Down: Выбор"));
  oled.setCursor(0, 4); oled.print(F("A: Применить"));
  oled.setCursor(0, 5); oled.print(F("B: Отмена"));
  oled.update();
}

// --- ОБРАБОТЧИКИ КНОПОК ---
void handleSplashButtons() {
  btnUp.tick(); btnDown.tick(); btnA.tick(); btnB.tick();
  if (btnUp.isClick() || btnDown.isClick() || btnA.isClick() || btnB.isClick()) {
    currentState = STATE_TOP_MENU; displayTopMenu(); lastActivityTime = millis(); return;
  }
  if (millis() - splashStartTime >= SPLASH_DURATION) {
    currentState = STATE_TOP_MENU; displayTopMenu(); lastActivityTime = millis();
  }
}

void handleTopMenuButtons() {
  btnUp.tick(); btnDown.tick(); btnA.tick(); btnB.tick();
  if (btnDown.isClick()) { currentTopMenuIndex = (currentTopMenuIndex + 1) % MAIN_MENU_SIZE; displayTopMenu(); lastActivityTime = millis(); }
  if (btnUp.isClick()) { currentTopMenuIndex = (currentTopMenuIndex - 1 + MAIN_MENU_SIZE) % MAIN_MENU_SIZE; displayTopMenu(); lastActivityTime = millis(); }
  if (btnA.isClick()) {
    if (currentTopMenuIndex == 0) { // Ответы
      currentState = STATE_ANSWERS_MENU; currentMenuIndex = 0; topVisibleIndex = 0; displayAnswersMenu(); lastActivityTime = millis();
    } else if (currentTopMenuIndex == 1) { // Калькулятор
      calcState = CALC_FIRST_NUM; currentDigit = 0; firstNumber = 0; secondNumber = 0; operatorChar = '+'; calcHistory[0] = '\0';
      currentState = STATE_CALC_INPUT; displayCalculator(); lastActivityTime = millis();
    } else if (currentTopMenuIndex == 2) { // Тетрис
      currentState = STATE_TETRIS;
      for (uint8_t y = 0; y < FIELD_HEIGHT; y++) for (uint8_t x = 0; x < FIELD_WIDTH; x++) playfield[y][x] = 0;
      gameOver = false; spawnNewPiece(); lastMoveTime = millis(); lastAccelerateTime = 0; displayTetris(); lastActivityTime = millis();
    } else if (currentTopMenuIndex == 3) { // Настройки
      currentState = STATE_SETTINGS_MENU; currentSettingsMenuIndex = 0; displaySettingsMenu(); lastActivityTime = millis();
    }
  }
}

void handleAnswersMenuButtons() {
  btnUp.tick(); btnDown.tick(); btnA.tick(); btnB.tick();
  if (btnDown.isClick()) {
    currentMenuIndex++; if (currentMenuIndex >= MENU_SIZE) { currentMenuIndex = 0; topVisibleIndex = 0; }
    else if (currentMenuIndex >= topVisibleIndex + LINES_PER_SCREEN) topVisibleIndex++;
    displayAnswersMenu(); lastActivityTime = millis();
  }
  if (btnUp.isClick()) {
    currentMenuIndex--; if (currentMenuIndex < 0) { currentMenuIndex = MENU_SIZE - 1; topVisibleIndex = (MENU_SIZE > LINES_PER_SCREEN) ? (MENU_SIZE - LINES_PER_SCREEN) : 0; }
    else if (currentMenuIndex < topVisibleIndex) topVisibleIndex--;
    displayAnswersMenu(); lastActivityTime = millis();
  }
  if (btnA.isClick()) { currentState = STATE_INFO_VIEW; currentInfoPageIndex = 0; displayInfo(); lastActivityTime = millis(); }
  if (btnA.isHold()) { currentState = STATE_TOP_MENU; displayTopMenu(); lastActivityTime = millis(); }
}

void handleInfoViewButtons() {
  btnUp.tick(); btnDown.tick(); btnA.tick(); btnB.tick();
  if (btnDown.isClick()) { currentInfoPageIndex++; displayInfo(); lastActivityTime = millis(); }
  if (btnUp.isClick()) { currentInfoPageIndex = max(0, currentInfoPageIndex - 1); displayInfo(); lastActivityTime = millis(); }
  if (btnA.isClick()) { currentState = STATE_ANSWERS_MENU; displayAnswersMenu(); lastActivityTime = millis(); }
  if (btnA.isHold()) { currentState = STATE_TOP_MENU; displayTopMenu(); lastActivityTime = millis(); }
  if (btnB.isClick()) { int totalPages = getTotalPages(currentMenuIndex); currentInfoPageIndex = totalPages - 1; displayInfo(); lastActivityTime = millis(); }
  if (btnB.isHold()) { currentInfoPageIndex = 0; displayInfo(); lastActivityTime = millis(); }
}

void handleCalculatorButtons() {
  btnUp.tick(); btnDown.tick(); btnA.tick(); btnB.tick();
  if (btnA.isHold()) { currentState = STATE_TOP_MENU; displayTopMenu(); lastActivityTime = millis(); return; }
  if (calcState == CALC_FIRST_NUM || calcState == CALC_SECOND_NUM) {
    if (btnUp.isClick()) { currentDigit = (currentDigit + 1) % 10; displayCalculator(); lastActivityTime = millis(); }
    if (btnDown.isClick()) { currentDigit = (currentDigit - 1 + 10) % 10; displayCalculator(); lastActivityTime = millis(); }
    if (btnA.isClick()) {
      if (calcState == CALC_FIRST_NUM) {
        if (firstNumber < 100000) firstNumber = firstNumber * 10 + currentDigit; // Уменьшено ограничение
      } else {
        if (secondNumber < 100000) secondNumber = secondNumber * 10 + currentDigit; // Уменьшено ограничение
      }
      currentDigit = 0; displayCalculator(); lastActivityTime = millis();
    }
    if (btnB.isClick()) {
      if (currentDigit != 0) {
        if (calcState == CALC_FIRST_NUM) {
          if (firstNumber < 100000) firstNumber = firstNumber * 10 + currentDigit;
        } else {
          if (secondNumber < 100000) secondNumber = secondNumber * 10 + currentDigit;
        }
        currentDigit = 0;
      }
      if (calcState == CALC_FIRST_NUM) calcState = CALC_OPERATOR;
      else if (calcState == CALC_SECOND_NUM) calcState = CALC_RESULT;
      displayCalculator(); lastActivityTime = millis();
    }
  } else if (calcState == CALC_OPERATOR) {
    if (btnUp.isClick()) {
      switch (operatorChar) { case '+': operatorChar = '-'; break; case '-': operatorChar = '*'; break; case '*': operatorChar = '/'; break; case '/': operatorChar = '+'; break; }
      displayCalculator(); lastActivityTime = millis();
    }
    if (btnDown.isClick()) {
      switch (operatorChar) { case '+': operatorChar = '/'; break; case '/': operatorChar = '*'; break; case '*': operatorChar = '-'; break; case '-': operatorChar = '+'; break; }
      displayCalculator(); lastActivityTime = millis();
    }
    if (btnA.isClick() || btnB.isClick()) { calcState = CALC_SECOND_NUM; currentDigit = 0; displayCalculator(); lastActivityTime = millis(); }
  } else if (calcState == CALC_RESULT) {
    if (btnA.isClick()) {
      char resultStr[12]; // Уменьшенный буфер
      resultStr[0] = '\0';
      if (operatorChar == '/' && secondNumber == 0) {
        strcpy_P(resultStr, PSTR("Error"));
      } else if (operatorChar == '/') {
        float result = (float)firstNumber / (float)secondNumber;
        dtostrf(result, 6, 2, resultStr);
        // Удаление пробелов
        char *p = resultStr;
        while (*p == ' ') p++;
        memmove(resultStr, p, strlen(p) + 1);
        p = resultStr + strlen(resultStr) - 1;
        while (p >= resultStr && *p == ' ') *p-- = '\0';
      } else {
        long result = 0;
        switch (operatorChar) {
          case '+': result = firstNumber + secondNumber; break;
          case '-': result = firstNumber - secondNumber; break;
          case '*': result = firstNumber * secondNumber; break;
        }
        ltoa(result, resultStr, 10);
      }
      char historyEntry[20]; // Уменьшенный буфер
      sprintf(historyEntry, "%ld%c%ld=%s\n", firstNumber, operatorChar, secondNumber, resultStr);
      // Управление историей
      size_t histLen = strlen(calcHistory);
      size_t entryLen = strlen(historyEntry);
      if (histLen + entryLen >= sizeof(calcHistory)) {
        char *firstNewline = strchr(calcHistory, '\n');
        if (firstNewline) memmove(calcHistory, firstNewline + 1, strlen(firstNewline + 1) + 1);
        else calcHistory[0] = '\0';
      }
      strcat(calcHistory, historyEntry);
      calcState = CALC_FIRST_NUM; firstNumber = 0; secondNumber = 0; operatorChar = '+'; currentDigit = 0;
      displayCalculator(); lastActivityTime = millis();
    }
  }
}

void handleSettingsMenuButtons() {
  btnUp.tick(); btnDown.tick(); btnA.tick(); btnB.tick();
  if (btnDown.isClick()) { currentSettingsMenuIndex = (currentSettingsMenuIndex + 1) % SETTINGS_MENU_SIZE; displaySettingsMenu(); lastActivityTime = millis(); }
  if (btnUp.isClick()) { currentSettingsMenuIndex = (currentSettingsMenuIndex - 1 + SETTINGS_MENU_SIZE) % SETTINGS_MENU_SIZE; displaySettingsMenu(); lastActivityTime = millis(); }
  if (btnA.isClick()) {
    if (currentSettingsMenuIndex == 0) { // Инверсия
      invertEnabled = !invertEnabled; oled.invertDisplay(invertEnabled); saveSettingsToEEPROM(); displaySettingsMenu(); lastActivityTime = millis();
    } else if (currentSettingsMenuIndex == 1) { // Контраст
      tempContrastValue = contrastLevel; currentState = STATE_CONTRAST_ADJUST; displayContrastAdjust(); lastActivityTime = millis();
    } else if (currentSettingsMenuIndex == 2) { // Скринсейвер
      currentState = STATE_SCREENSAVER_ADJUST; displayScreensaverAdjust(); lastActivityTime = millis();
    }
  }
  if (btnA.isHold()) { currentState = STATE_TOP_MENU; displayTopMenu(); lastActivityTime = millis(); }
}

void handleContrastAdjustButtons() {
  btnUp.tick(); btnDown.tick(); btnA.tick(); btnB.tick();
  if (btnUp.isClick()) { tempContrastValue = min((uint8_t)(tempContrastValue + 5), (uint8_t)255); oled.setContrast(tempContrastValue); displayContrastAdjust(); lastActivityTime = millis(); }
  if (btnDown.isClick()) { tempContrastValue = max((uint8_t)(tempContrastValue - 5), (uint8_t)0); oled.setContrast(tempContrastValue); displayContrastAdjust(); lastActivityTime = millis(); }
  if (btnA.isClick()) { contrastLevel = tempContrastValue; saveSettingsToEEPROM(); currentState = STATE_SETTINGS_MENU; displaySettingsMenu(); lastActivityTime = millis(); }
  if (btnB.isClick()) { oled.setContrast(contrastLevel); currentState = STATE_SETTINGS_MENU; displaySettingsMenu(); lastActivityTime = millis(); }
}

void handleScreensaverAdjustButtons() {
  btnUp.tick(); btnDown.tick(); btnA.tick(); btnB.tick();
  if (btnUp.isClick()) { screensaverTimeIndex = (screensaverTimeIndex + 1) % SCREENSAVER_OPTIONS_SIZE; displayScreensaverAdjust(); lastActivityTime = millis(); }
  if (btnDown.isClick()) { screensaverTimeIndex = (screensaverTimeIndex - 1 + SCREENSAVER_OPTIONS_SIZE) % SCREENSAVER_OPTIONS_SIZE; displayScreensaverAdjust(); lastActivityTime = millis(); }
  if (btnA.isClick()) { saveSettingsToEEPROM(); currentState = STATE_SETTINGS_MENU; displaySettingsMenu(); lastActivityTime = millis(); }
  if (btnB.isClick()) {
    screensaverTimeIndex = EEPROM.read(EEPROM_ADDR_SCREENSAVER);
    currentState = STATE_SETTINGS_MENU; displaySettingsMenu(); lastActivityTime = millis();
  }
}

void displayScreensaver() {
  oled.clear(); oled.home(); oled.setScale(1);
  oled.setCursor(20, 3); oled.print(F("Спящий режим"));
  oled.setCursor(0, 7); oled.print(F("Нажмите любую кнопку"));
  oled.update();
}

void handleScreensaverButtons() {
  btnUp.tick(); btnDown.tick(); btnA.tick(); btnB.tick();
  if (btnUp.isPress() || btnDown.isPress() || btnA.isPress() || btnB.isPress() ||
      btnUp.isHold() || btnDown.isHold() || btnA.isHold() || btnB.isHold()) {
    oled.setContrast(contrastLevel);
    currentState = STATE_TOP_MENU; displayTopMenu();
    lastActivityTime = millis();
  }
}

void setup() {
  oled.init(); oled.clear(); oled.update();
  unsigned long seed = millis();
  for (uint8_t i = 0; i < 5; i++) { seed += analogRead(0); delay(1); }
  randomSeed(seed);
  loadSettingsFromEEPROM();
  calcHistory[0] = '\0'; // Инициализация истории
  displaySplash();
}

void loop() {
  if (screensaverTimeout > 0 && currentState != STATE_SPLASH && currentState != STATE_SCREENSAVER) {
    if (millis() - lastActivityTime > screensaverTimeout) {
      currentState = STATE_SCREENSAVER;
      oled.setContrast(SCREENSAVER_CONTRAST);
      displayScreensaver();
    }
  }
  switch (currentState) {
    case STATE_SPLASH: handleSplashButtons(); break;
    case STATE_TOP_MENU: handleTopMenuButtons(); break;
    case STATE_ANSWERS_MENU: handleAnswersMenuButtons(); break;
    case STATE_INFO_VIEW: handleInfoViewButtons(); break;
    case STATE_CALC_INPUT: handleCalculatorButtons(); break;
    case STATE_TETRIS: handleTetrisButtons(); break;
    case STATE_SETTINGS_MENU: handleSettingsMenuButtons(); break;
    case STATE_CONTRAST_ADJUST: handleContrastAdjustButtons(); break;
    case STATE_SCREENSAVER_ADJUST: handleScreensaverAdjustButtons(); break;
    case STATE_SCREENSAVER: handleScreensaverButtons(); break;
  }
  delay(5);
}