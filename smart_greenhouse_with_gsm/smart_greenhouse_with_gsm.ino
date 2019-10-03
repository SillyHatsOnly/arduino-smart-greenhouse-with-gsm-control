#include "Wire.h"
#include "iarduino_I2C_Relay.h"
#include "LiquidCrystal_I2C.h"
#include "iarduino_AM2320.h"
#include "iarduino_RTC.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "EEPROM.h"
//------------------------------------------------------------------------//
#define GSM_USING_FLAG                    1
//------------------------------------------------------------------------//
#define DEFAULT_SCREEN                    0
//------------------------------------------------------------------------//
#define LIGHT_SCREEN                      1
#define AIRING_ROOM_SCREEN                2
#define PUMP_SCREEN                       3
#define SETTINGS_SCREEN                   4
//------------------------------------------------------------------------//
#define LIGHT_RESUME_SCREEN               11
#define LIGHT_MIN_LIGHT_SCREEN            12
#define AIRING_ROOM_RESUME_SCREEN         21
#define AIRING_ROOM_MIN_TEMP_SCREEN       22
#define AIRING_ROOM_MAX_TEMP_SCREEN       23
#define PUMP_RESUME_SCREEN                31
#define PUMP_WORKING_TIME_SCREEN          32
#define PUMP_DELAY_TIME_SCREEN            33
#define SETTINGS_RTC_SCREEN               41
#define SETTINGS_RELE_SCREEN              42
#define SETTINGS_ZUMMER_SCREEN            43
//------------------------------------------------------------------------//
#define LIGHTING_RESUME_SET               111
#define LIGHTING_MIN_VALUE_SET            121
#define AIRING_ROOM_RESUME_SET            211
#define AIRING_ROOM_MIN_TEMP_SET          221
#define AIRING_ROOM_MAX_TEMP_SET          231
#define AUTOPUMP_RESUME_SET               311
#define AUTOPUMP_TIME_SET                 321
#define AUTOPUMP_WORKING_DELAY_SET        331
#define SETTINGS_RTC_HOUR_SCREEN          411
#define SETTINGS_RTC_MINUTES_SCREEN       412
#define SETTINGS_RTC_SECONDS_SCREEN       413
#define SETTINGS_RTC_WEEK_DAY_SCREEN      414
#define SETTINGS_RTC_DAY_SCREEN           415
#define SETTINGS_RTC_MONTH_SCREEN         416
#define SETTINGS_RTC_YEAR_SCREEN          417
#define SETTINGS_RELE_1_SCREEN            421
#define SETTINGS_RELE_2_SCREEN            422
#define SETTINGS_RELE_3_SCREEN            423
#define SETTINGS_RELE_4_SCREEN            424
#define ZUMMER_RESUME_SET                 431
//------------------------------------------------------------------------//
#define RTC_HOUR_SET                      4111
#define RTC_MINUTE_SET                    4121
#define RTC_SECONDS_SET                   4131
#define RTC_WEEK_DAY_SET                  4141
#define RTC_DAY_SET                       4151
#define RTC_MONTH_SET                     4161
#define RTC_YEAR_SET                      4171
#define RELE_1_TEST_SET                   4211
#define RELE_2_TEST_SET                   4221
#define RELE_3_TEST_SET                   4231
#define RELE_4_TEST_SET                   4241
//------------------------------------------------------------------------//
#define HOURS                             0
#define MINUTES                           1
#define SECONDS                           2
#define WEEK_DAY                          3
#define DAY                               4
#define MONTH                             5
#define YEAR                              6
//------------------------------------------------------------------------//
#define BTN_UP                            0
#define BTN_DWN                           1
#define BTN_OK                            2
#define BTN_ESC                           3
//------------------------------------------------------------------------//
#define  MIN_HUMIDITY                     0
#define  MIN_TEMPERATURE_FOR_AIRING_ROOM  1
#define  MAX_TEMPERATURE_FOR_AIRING_ROOM  2
#define  TIME_FOR_START_PUMP              3
#define  DELAY_TIME_FOR_PUMP_WORKING      4
#define  SELECTED_LIGHTING_RESUME         5
#define  SELECTED_AIRING_ROOM_RESUME      6
#define  SELECTED_PUMP_WORK_RESUME        7
#define  SELECTED_RELE_1_RESUME           8
#define  SELECTED_RELE_2_RESUME           9
#define  SELECTED_RELE_3_RESUME           10
#define  SELECTED_RELE_4_RESUME           11
#define  SELECTED_ZUMMER_RESUME           12
//------------------------------------------------------------------------//
#define SMS_LIGHT_RESUME                  0
#define SMS_LIGHTING_MIN_VALUE_SET        1
#define SMS_AIRING_ROOM_RESUME            2
#define SMS_AIRING_ROOM_MIN_TEMP_SET      3
#define SMS_AIRING_ROOM_MAX_TEMP_SET      4
#define SMS_AUTOPUMP_RESUME               5
#define SMS_AUTOPUMP_WORKING_TIME         6
#define SMS_AUTOPUMP_WORKING_DELAY        7
#define SMS_SET_NEW_RTC_TIME              8
#define SMS_GET_FULL_STATE_INFO           9
#define SMS_GET_ALL_COMMAND_LIST          10
//------------------------------------------------------------------------//
#define LIGHT_OFF                         0
#define LIGHT_ON                          1
#define LIGHT_AUTO                        2
//------------------------------------------------------------------------//
#define AIRING_ROOM_OFF                   0
#define AIRING_ROOM_OPEN                  1
#define AIRING_ROOM_CLOSE                 2
#define AIRING_ROOM_AUTO                  3
//------------------------------------------------------------------------//
#define PUMP_OFF                          0
#define PUMP_ON                           1
#define PUMP_AUTO                         2
//------------------------------------------------------------------------//
#define ZUMMER_OFF                        0
#define ZUMMER_ON                         1
#define ZUMMER_AUTO                       2
//------------------------------------------------------------------------//
#define RELE_OFF                          0
#define RELE_ON                           1
//------------------------------------------------------------------------//
const int TEMPERATURE_SENSOR_PIN = 2;
OneWire oneWire(TEMPERATURE_SENSOR_PIN);
DallasTemperature TEMPERATURE_SENSOR(&oneWire);

LiquidCrystal_I2C LCD(0x27, 16, 2);

iarduino_RTC RTC_MODULE(RTC_DS3231);

iarduino_I2C_Relay I2C_RELAY_1(0x09);
iarduino_I2C_Relay I2C_RELAY_2(0x0A);

iarduino_AM2320 TEMPERATURE_AND_HUMIDITY_SENSOR;

#if GSM_USING_FLAG
    #include "iarduino_GSM.h"
    iarduino_GSM GSM_MODULE;
    #include "SoftwareSerial.h"
    SoftwareSerial SOFT_SERIAL(7, 8);

    const char*     SMS_REQUEST_TEXT[] = { "LR", "LMIN", "ARR", "ARMIN", "ARMAX", "APR", "APTIME", "APDELAY", "TIME", "INFO", "HELP"};
    String          StrSmsTxt;
    char            SmsTxt[161];
    char            SmsNum[13];
    char            Number[13]  = "7_ВАШ_НОМЕР_ТЕЛЕФОНА";
    uint8_t         ColonSymbol;
    uint8_t         SmsCommandNumber;
    String          SmsTextAnswer_1, SmsTextAnswer_2, SmsTextAnswer_3, SmsTextAnswer_4, SmsTextAnswer_5, SmsTextAnswer_6;
#endif

const uint8_t   GROUND_HUMIDITY_PIN                   = A0;
const uint8_t   LIGHT_SENSOR_PIN                      = A1;
const uint8_t   ZUMMER_PIN                            = 4;
const uint8_t   DOWN_BUTTON_PIN                       = 5;
const uint8_t   UP_BUTTON_PIN                         = 6;
const uint8_t   OK_BUTTON_PIN                         = 9;
const uint8_t   ESC_BUTTON_PIN                        = 10;

const uint32_t  SENSOR_CHECK_TIME                     = 1000;
const uint16_t  MIN_GROUND_RAW_HUMIDITY               = 650;
const uint16_t  MAX_GROUND_RAW_HUMIDITY               = 325;
const uint8_t   MIN_GROUND_HUMIDITY_PERCENT           = 60;
const uint8_t   MAX_GROUND_HUMIDITY_PERCENT           = 99;
const uint8_t   HUMIDITY_GAP                          = 50;
const uint32_t  EEPROM_UPDATE_TIME                    = 30000;
const uint32_t  DEFAULT_SCREEN_RETURNING_TIME         = 10000;
const uint32_t  RELE_TEST_FULL_TIME                   = 2000;
const uint32_t  ZUMMER_WORKING_TIME                   = 1000;
const uint32_t  ZUMMER_NOT_WORKING_TIME               = 1000;
const uint32_t  OPEN_WINDOW_TIME                      = 10000;
const uint32_t  CLOSE_WINDOW_TIME                     = 10000;
const uint32_t  DEFAULT_SCREEN_CHANGE_PAGE_TIME       = 10000;
const uint8_t   DEGREE_SYMBOL[8]                      = {0xC, 0x12, 0x12, 0xC, 0x0, 0x0, 0x0, 0x0};

int             RealGroundTemperature                 = 0;
uint16_t        RealRawGroundHumidity                 = 0;
uint8_t         RealGroundHumidityPercent             = 0;
uint16_t        RealLightingValue                     = 0;
int             RealAirTemperature                    = 0;
uint8_t         RealAirHumidityPercent                = 0;
int             MenuItem                              = 0;
int32_t         RtcCurrentValue[]                     = {  0,  0,  0, 0,  0, 0, 0                   };
bool            ButtonState[]                         = {  0,  0,  0, 0                             };
int32_t         CurrentResumeOrBorderSettingValue[]   = {  0,  0,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0 };
const int32_t   DefaultResumeOrBorderSettingValue[]   = {320, 22, 30, 7, 30, 0, 0, 0, 0, 0, 0, 0, 0 };

bool            ButtonUpPushFlag                      = 0;
bool            ButtonDownPushFlag                    = 0;
bool            ButtonOkPushFlag                      = 0;
bool            ButtonEscPushFlag                     = 0;
bool            ChangeMenuScreenFlag                  = 1;
bool            SetNewResumeOrBorderValueFlag         = 1;
bool            LaunchDefaultMenuFlag                 = 1;
bool            StartPumpWorkingFlag                  = 1;
bool            StartZummerWorkingFlag                = 1;
bool            StartOpeningWindowFlag                = 1;
bool            StartClosingWindowFlag                = 1;
int             TemporaryOldValue                     = 0;
int             TemporaryNewValue                     = 0;
uint32_t        ChangeDefaultPageTimer_1              = millis();
uint32_t        ChangeDefaultPageTimer_2              = 0;
uint32_t        DefaultMenuTimer                      = 0;
uint32_t        ReleTestTimer                         = 0;
uint32_t        AutopumpWorkingTimeTimer              = 0;
uint32_t        ZummerWorkingTimeTimer                = 0;
uint32_t        ZummerNotWorkingTimeTimer             = 0;
uint32_t        OpeningWindowTimer                    = 0;
uint32_t        ClosingWindowTimer                    = 0;

void MENU_DEPTH();
void SENSORS_POLLING_FUNC();
void BUTTON_UP_FUNC();
void BUTTON_DOWN_FUNC();
void BUTTON_OK_FUNC();
void BUTTON_ESC_FUNC();
void SCENARIO_FUNC();
void EEPROM_UPDATE_FUNC();
void GSM_USE_FUNC();
void NOTIFICATION_NEW_VALUE_SAVED_FUNC();

void setup() {
  pinMode(GROUND_HUMIDITY_PIN,  INPUT);
  pinMode(LIGHT_SENSOR_PIN,     INPUT);
  pinMode(ZUMMER_PIN,           OUTPUT);
  pinMode(UP_BUTTON_PIN,        INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN,      INPUT_PULLUP);
  pinMode(OK_BUTTON_PIN,        INPUT_PULLUP);
  pinMode(ESC_BUTTON_PIN,       INPUT_PULLUP);
  
  TEMPERATURE_AND_HUMIDITY_SENSOR.begin();
  TEMPERATURE_SENSOR.begin();

  I2C_RELAY_1.begin();
  I2C_RELAY_2.begin();
  I2C_RELAY_1.digitalWrite(ALL_CHANNEL, LOW);
  I2C_RELAY_2.digitalWrite(ALL_CHANNEL, LOW);
  
  RTC_MODULE.begin();
  RTC_MODULE.gettime();

  LCD.createChar(1, DEGREE_SYMBOL);

  LCD.init();
  LCD.backlight();
  LCD.clear();
  LCD.setCursor(4, 0);
  LCD.print("Welcome!");

#if GSM_USING_FLAG
  GSM_MODULE.begin(SOFT_SERIAL);
  while (GSM_MODULE.status() != GSM_OK) {
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("Waiting GSM...");
    delay(1000);
  }
  GSM_MODULE.TXTsendCodingDetect("п");
  GSM_MODULE.SMSsend( F("Инициализация прошла успешно."), Number);
#endif

  RtcCurrentValue[HOURS]    = RTC_MODULE.Hours;   // часы (в 24 часовом формате)
  RtcCurrentValue[MINUTES]  = RTC_MODULE.minutes; // минуты
  RtcCurrentValue[SECONDS]  = RTC_MODULE.seconds; // секунды
  RtcCurrentValue[WEEK_DAY] = RTC_MODULE.weekday; // день недели: 0 (воскресение), 1 (понедельник), ... , 6 (суббота)
  RtcCurrentValue[DAY]      = RTC_MODULE.day;     // день
  RtcCurrentValue[MONTH]    = RTC_MODULE.month;   // месяц
  RtcCurrentValue[YEAR]     = RTC_MODULE.year;    // год

  /*

    // ОЧИСТКА ВСЕЙ ПАМЯТИ EEPROM
    for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
    }

  */
  
  if (EEPROM.read(100) == 1) {
    for (int i = 0; i < (sizeof(CurrentResumeOrBorderSettingValue) / sizeof(CurrentResumeOrBorderSettingValue[0])); i++) {
      byte HiByte           = EEPROM.read(i * 2);
      byte LowByte          = EEPROM.read(i * 2 + 1);
      int  EepromValue      = word(HiByte, LowByte);
      CurrentResumeOrBorderSettingValue[i] = EepromValue;
    }
  }

  else {
    for (int i = 0; i < (sizeof(CurrentResumeOrBorderSettingValue) / sizeof(CurrentResumeOrBorderSettingValue[0])); i++) {
      CurrentResumeOrBorderSettingValue[i] = DefaultResumeOrBorderSettingValue[i];
    }
  }
  
  RealLightingValue         = analogRead(LIGHT_SENSOR_PIN);
  TEMPERATURE_SENSOR.requestTemperatures();
  
  RealGroundTemperature     = TEMPERATURE_SENSOR.getTempCByIndex(0);
  TEMPERATURE_AND_HUMIDITY_SENSOR.read();
  
  RealAirTemperature        = TEMPERATURE_AND_HUMIDITY_SENSOR.tem;
  RealAirHumidityPercent    = TEMPERATURE_AND_HUMIDITY_SENSOR.hum;

  RealRawGroundHumidity     = analogRead(GROUND_HUMIDITY_PIN);
  RealGroundHumidityPercent = map(RealRawGroundHumidity, MIN_GROUND_RAW_HUMIDITY, MAX_GROUND_RAW_HUMIDITY, 0, 100 );

  delay(1000);
  LCD.clear();
}

void loop() {
  SENSORS_POLLING_FUNC();
  MENU_DEPTH();
  SCENARIO_FUNC();
  BUTTON_UP_FUNC();
  BUTTON_DOWN_FUNC();
  BUTTON_OK_FUNC();
  BUTTON_ESC_FUNC();
  
  #if GSM_USING_FLAG
    GSM_USE_FUNC();
  #endif

  EEPROM_UPDATE_FUNC();
  if ((DefaultMenuTimer + DEFAULT_SCREEN_RETURNING_TIME) < millis() && MenuItem < 10) {
    MenuItem = 0;
  }
}

void NOTIFICATION_NEW_VALUE_SAVED_FUNC(){
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("NEW VALUE SAVED!");
  delay(500);
}

void EEPROM_UPDATE_FUNC() {
  if (millis() % EEPROM_UPDATE_TIME < 10) {
    delay(10);
    EEPROM.update(100, 1);
    for (int i = 0; i < (sizeof(CurrentResumeOrBorderSettingValue) / sizeof(CurrentResumeOrBorderSettingValue[0])); i++) {
      byte HiByte  = highByte(CurrentResumeOrBorderSettingValue[i]);
      byte LowByte = lowByte(CurrentResumeOrBorderSettingValue[i]);
      EEPROM.update((i * 2),     HiByte);
      EEPROM.update((i * 2 + 1), LowByte);
    }
  }
}

void SENSORS_POLLING_FUNC() {
  ButtonState[BTN_UP]         = digitalRead(UP_BUTTON_PIN);
  ButtonState[BTN_DWN]        = digitalRead(DOWN_BUTTON_PIN);
  ButtonState[BTN_OK]         = digitalRead(OK_BUTTON_PIN);
  ButtonState[BTN_ESC]        = digitalRead(ESC_BUTTON_PIN);
  
  RTC_MODULE.gettime();

  RtcCurrentValue[HOURS]      = RTC_MODULE.Hours;   // часы в 24 часовом формате
  RtcCurrentValue[MINUTES]    = RTC_MODULE.minutes; // минуты
  RtcCurrentValue[SECONDS]    = RTC_MODULE.seconds; // секунды
  RtcCurrentValue[WEEK_DAY]   = RTC_MODULE.weekday; // день недели: 0 (воскресение), 1 (понедельник), ... , 6 (суббота)
  RtcCurrentValue[DAY]        = RTC_MODULE.day;     // день
  RtcCurrentValue[MONTH]      = RTC_MODULE.month;   // месяц
  RtcCurrentValue[YEAR]       = RTC_MODULE.year;    // год
  
  if (millis() % SENSOR_CHECK_TIME < 20) {
    delay(20);

    RealLightingValue           = analogRead(LIGHT_SENSOR_PIN);
  
    TEMPERATURE_SENSOR.requestTemperatures();
    RealGroundTemperature     = TEMPERATURE_SENSOR.getTempCByIndex(0);

    TEMPERATURE_AND_HUMIDITY_SENSOR.read();
    RealAirTemperature        = TEMPERATURE_AND_HUMIDITY_SENSOR.tem;
    RealAirHumidityPercent    = TEMPERATURE_AND_HUMIDITY_SENSOR.hum;

    RealRawGroundHumidity       = analogRead(GROUND_HUMIDITY_PIN);
    RealGroundHumidityPercent   = map(RealRawGroundHumidity, MIN_GROUND_RAW_HUMIDITY, MAX_GROUND_RAW_HUMIDITY, 0, 100 );
  }
}

void BUTTON_DOWN_FUNC() {
  if (ButtonState[BTN_DWN] && !ButtonDownPushFlag) {
    DefaultMenuTimer      = millis();
    ButtonDownPushFlag    = true;
    ChangeMenuScreenFlag  = true;

    if (MenuItem == DEFAULT_SCREEN)                                         MenuItem = 1;     else
    
    if (MenuItem > 0   && MenuItem < 10 ){ MenuItem++; if (MenuItem > 4 )   MenuItem =  1;}   else
    if (MenuItem > 10  && MenuItem < 20 ){ MenuItem++; if (MenuItem > 12)   MenuItem = 11;}   else
    if (MenuItem > 20  && MenuItem < 30 ){ MenuItem++; if (MenuItem > 23)   MenuItem = 21;}   else
    if (MenuItem > 30  && MenuItem < 40 ){ MenuItem++; if (MenuItem > 33)   MenuItem = 31;}   else
    if (MenuItem > 40  && MenuItem < 50 ){ MenuItem++; if (MenuItem > 43)   MenuItem = 41;}   else
    if (MenuItem > 410 && MenuItem < 420){ MenuItem++; if (MenuItem > 417)  MenuItem = 411;}  else
    if (MenuItem > 420 && MenuItem < 430){ MenuItem++; if (MenuItem > 424)  MenuItem = 421;}  else
    
    if (MenuItem == LIGHTING_RESUME_SET)       { TemporaryNewValue++;     if(TemporaryNewValue > 2)                                                                   TemporaryNewValue = 0;                                                                      } else
    if (MenuItem == LIGHTING_MIN_VALUE_SET)    { TemporaryNewValue += 50; if(TemporaryNewValue >= 1000)                                                               TemporaryNewValue = 1023;                                                                   } else
    if (MenuItem == AIRING_ROOM_RESUME_SET)    { TemporaryNewValue++;     if(TemporaryNewValue > 3)                                                                   TemporaryNewValue = 0;                                                                      } else
    if (MenuItem == AIRING_ROOM_MIN_TEMP_SET)  { TemporaryNewValue++;     if(TemporaryNewValue >= CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM]) TemporaryNewValue = CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] - 1; } else
    if (MenuItem == AIRING_ROOM_MAX_TEMP_SET)  { TemporaryNewValue++;     if(TemporaryNewValue > 50)                                                                  TemporaryNewValue = 50;                                                                     } else
    if (MenuItem == AUTOPUMP_RESUME_SET)       { TemporaryNewValue++;     if(TemporaryNewValue > 2)                                                                   TemporaryNewValue = 0;                                                                      } else
    if (MenuItem == AUTOPUMP_TIME_SET)         { TemporaryNewValue++;     if(TemporaryNewValue > 23)                                                                  TemporaryNewValue = 0;                                                                      } else
    if (MenuItem == AUTOPUMP_WORKING_DELAY_SET){ TemporaryNewValue += 10; if(TemporaryNewValue > 300)                                                                 TemporaryNewValue = 10;                                                                     } else
    if (MenuItem == ZUMMER_RESUME_SET)         { TemporaryNewValue++;     if(TemporaryNewValue > 2)                                                                   TemporaryNewValue = 0;                                                                      } else
    if (MenuItem == RTC_HOUR_SET)              { TemporaryNewValue++;     if(TemporaryNewValue > 23)                                                                  TemporaryNewValue = 0;                                                                      } else
    if (MenuItem == RTC_MINUTE_SET)            { TemporaryNewValue++;     if(TemporaryNewValue > 59)                                                                  TemporaryNewValue = 0;                                                                      } else
    if (MenuItem == RTC_SECONDS_SET)           { TemporaryNewValue++;     if(TemporaryNewValue > 59)                                                                  TemporaryNewValue = 0;                                                                      } else
    if (MenuItem == RTC_WEEK_DAY_SET)          { TemporaryNewValue++;     if(TemporaryNewValue > 6)                                                                   TemporaryNewValue = 0;                                                                      } else
    if (MenuItem == RTC_DAY_SET)               { TemporaryNewValue++;     if(TemporaryNewValue > 31)                                                                  TemporaryNewValue = 1;                                                                      } else
    if (MenuItem == RTC_MONTH_SET)             { TemporaryNewValue++;     if(TemporaryNewValue > 12)                                                                  TemporaryNewValue = 1;                                                                      } else
    if (MenuItem == RTC_YEAR_SET)              { TemporaryNewValue++;     if(TemporaryNewValue > 99)                                                                  TemporaryNewValue = 0;                                                                      }
  }
  else if (!ButtonState[BTN_DWN] && ButtonDownPushFlag) {
    ButtonDownPushFlag = false;
  }
}

void BUTTON_UP_FUNC() {
    if (ButtonState[BTN_UP] && !ButtonUpPushFlag) {
    DefaultMenuTimer = millis();
    ButtonUpPushFlag = true;
    ChangeMenuScreenFlag = true;
    
    if (MenuItem == DEFAULT_SCREEN)                                      MenuItem = 1;                   else
    
    if (MenuItem > 0    &&  MenuItem < 10   ) { MenuItem--; if (MenuItem < 1 )   MenuItem =  4;        } else
    if (MenuItem > 10   &&  MenuItem < 20   ) { MenuItem--; if (MenuItem < 11)   MenuItem = 12;        } else
    if (MenuItem > 20   &&  MenuItem < 30   ) { MenuItem--; if (MenuItem < 21)   MenuItem = 23;        } else
    if (MenuItem > 30   &&  MenuItem < 40   ) { MenuItem--; if (MenuItem < 31)   MenuItem = 33;        } else
    if (MenuItem > 40   &&  MenuItem < 50   ) { MenuItem--; if (MenuItem < 41)   MenuItem = 43;        } else
    if (MenuItem > 410  &&  MenuItem < 420  ) { MenuItem--; if (MenuItem < 411)  MenuItem = 417;       } else
    if (MenuItem > 420  &&  MenuItem < 430  ) { MenuItem--; if (MenuItem < 421)  MenuItem = 424;       } else
    
    if (MenuItem == LIGHTING_RESUME_SET)        { TemporaryNewValue--;            if(TemporaryNewValue < 0)                                                                   TemporaryNewValue = 2;                                                                      } else
    if (MenuItem == LIGHTING_MIN_VALUE_SET)     { TemporaryNewValue -= 50;        if(TemporaryNewValue < 50)                                                                  TemporaryNewValue = 0;                                                                      } else
    if (MenuItem == AIRING_ROOM_RESUME_SET)     { TemporaryNewValue--;            if(TemporaryNewValue < 0)                                                                   TemporaryNewValue = 3;                                                                      } else
    if (MenuItem == AIRING_ROOM_MIN_TEMP_SET)   { TemporaryNewValue--;            if(TemporaryNewValue < 0)                                                                   TemporaryNewValue = 0;                                                                      } else
    if (MenuItem == AIRING_ROOM_MAX_TEMP_SET)   { TemporaryNewValue--;            if(TemporaryNewValue <= CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM]) TemporaryNewValue = CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM] + 1; } else
    if (MenuItem == AUTOPUMP_RESUME_SET)        { TemporaryNewValue--;            if(TemporaryNewValue < 0)                                                                   TemporaryNewValue = 2;                                                                      } else
    if (MenuItem == AUTOPUMP_TIME_SET)          { TemporaryNewValue--;            if(TemporaryNewValue < 0)                                                                   TemporaryNewValue = 23;                                                                     } else
    if (MenuItem == AUTOPUMP_WORKING_DELAY_SET) { TemporaryNewValue -= 10;        if(TemporaryNewValue < 10)                                                                  TemporaryNewValue = 300;                                                                    } else
    if (MenuItem == ZUMMER_RESUME_SET)          { TemporaryNewValue--;            if(TemporaryNewValue < 0)                                                                   TemporaryNewValue = 2;                                                                      } else
    if (MenuItem == RTC_HOUR_SET)               { TemporaryNewValue--;            if(TemporaryNewValue < 0)                                                                   TemporaryNewValue = 23;                                                                     } else
    if (MenuItem == RTC_MINUTE_SET)             { TemporaryNewValue--;            if(TemporaryNewValue < 0)                                                                   TemporaryNewValue = 59;                                                                     } else
    if (MenuItem == RTC_SECONDS_SET)            { TemporaryNewValue--;            if(TemporaryNewValue < 0)                                                                   TemporaryNewValue = 59;                                                                     } else
    if (MenuItem == RTC_WEEK_DAY_SET)           { TemporaryNewValue--;            if(TemporaryNewValue < 0)                                                                   TemporaryNewValue = 6;                                                                      } else
    if (MenuItem == RTC_DAY_SET)                { TemporaryNewValue--;            if(TemporaryNewValue < 1)                                                                   TemporaryNewValue = 31;                                                                     } else
    if (MenuItem == RTC_MONTH_SET)              { TemporaryNewValue--;            if(TemporaryNewValue < 1)                                                                   TemporaryNewValue = 12;                                                                     } else
    if (MenuItem == RTC_YEAR_SET)               { TemporaryNewValue--;            if(TemporaryNewValue < 0)                                                                   TemporaryNewValue = 99;                                                                     }
  }
  else if (!ButtonState[BTN_UP] && ButtonUpPushFlag) {
    ButtonUpPushFlag = false;
  }
}

void BUTTON_ESC_FUNC() {
  if (ButtonState[BTN_ESC] && !ButtonEscPushFlag) {
    DefaultMenuTimer = millis();
    ButtonEscPushFlag = true;
    ChangeMenuScreenFlag = true;
    if (MenuItem > 0 && MenuItem < 10)  MenuItem = 0;                   else 
    if (MenuItem > 10)                  MenuItem = (MenuItem - 1) / 10;
  }
  
  else if (!ButtonState[BTN_ESC] && ButtonEscPushFlag) {
    ButtonEscPushFlag = false;
  }
}

void BUTTON_OK_FUNC() {
  if (ButtonState[BTN_OK] && !ButtonOkPushFlag) {
    DefaultMenuTimer = millis();
    ButtonOkPushFlag = true;
    ChangeMenuScreenFlag = true;
    SetNewResumeOrBorderValueFlag = true;
    if (MenuItem == DEFAULT_SCREEN)                                   MenuItem = 1;
    else if (MenuItem > 0    &&  MenuItem < 50   )                    MenuItem = MenuItem * 10 + 1;
    else if (MenuItem > 400  &&  MenuItem < 500 && MenuItem != 431 )  MenuItem = MenuItem * 10 + 1;
    else if (MenuItem == RELE_1_TEST_SET) {
      CurrentResumeOrBorderSettingValue[SELECTED_RELE_1_RESUME]             = 1;
      ReleTestTimer = millis();
    }
    else if (MenuItem == RELE_2_TEST_SET) {
      CurrentResumeOrBorderSettingValue[SELECTED_RELE_2_RESUME]             = 1;
      ReleTestTimer = millis();
    }
    else if (MenuItem == RELE_3_TEST_SET) {
      CurrentResumeOrBorderSettingValue[SELECTED_RELE_3_RESUME]             = 1;
      ReleTestTimer = millis();
    }
    else if (MenuItem == RELE_4_TEST_SET) {
      CurrentResumeOrBorderSettingValue[SELECTED_RELE_4_RESUME]             = 1;
      ReleTestTimer = millis();
    }
    else {
      if (     MenuItem == LIGHTING_RESUME_SET)         CurrentResumeOrBorderSettingValue[SELECTED_LIGHTING_RESUME]         = TemporaryNewValue;
      else if (MenuItem == LIGHTING_MIN_VALUE_SET)      CurrentResumeOrBorderSettingValue[MIN_HUMIDITY]                     = TemporaryNewValue;
      else if (MenuItem == AIRING_ROOM_RESUME_SET)     {
                                                        CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME]      = TemporaryNewValue;
                                                     if(CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME]    == AIRING_ROOM_AUTO)     {StartClosingWindowFlag = true;  StartOpeningWindowFlag = true;  } else
                                                     if(CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME]    == AIRING_ROOM_CLOSE)    {StartClosingWindowFlag = false; StartOpeningWindowFlag = true;  } else
                                                     if(CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME]    == AIRING_ROOM_OPEN)     {StartClosingWindowFlag = true;  StartOpeningWindowFlag = false; }
      }
      else if (MenuItem == AIRING_ROOM_MIN_TEMP_SET)    CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM]  = TemporaryNewValue;
      else if (MenuItem == AIRING_ROOM_MAX_TEMP_SET)    CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM]  = TemporaryNewValue;
      else if (MenuItem == AUTOPUMP_RESUME_SET)        {
                                                        CurrentResumeOrBorderSettingValue[SELECTED_PUMP_WORK_RESUME]        = TemporaryNewValue;
                                                        if (CurrentResumeOrBorderSettingValue[SELECTED_PUMP_WORK_RESUME]  == PUMP_AUTO)            StartPumpWorkingFlag = true;
      }
      else if (MenuItem == AUTOPUMP_TIME_SET)          {CurrentResumeOrBorderSettingValue[TIME_FOR_START_PUMP]              = TemporaryNewValue;   StartPumpWorkingFlag = true;}
      else if (MenuItem == AUTOPUMP_WORKING_DELAY_SET)  CurrentResumeOrBorderSettingValue[DELAY_TIME_FOR_PUMP_WORKING]      = TemporaryNewValue;
      else if (MenuItem == ZUMMER_RESUME_SET)           CurrentResumeOrBorderSettingValue[SELECTED_ZUMMER_RESUME]           = TemporaryNewValue;
      else if (MenuItem == RTC_HOUR_SET)               {
                                                        RtcCurrentValue[HOURS]                                              = TemporaryNewValue;
                                                        RTC_MODULE.settime( RtcCurrentValue[SECONDS],  -1, RtcCurrentValue[HOURS],  -1, -1, -1, -1);
      }
      else if (MenuItem == RTC_MINUTE_SET)             {
                                                        RtcCurrentValue[MINUTES]                                            = TemporaryNewValue;
                                                        RTC_MODULE.settime( RtcCurrentValue[SECONDS],  RtcCurrentValue[MINUTES], -1,  -1, -1, -1, -1);
      }
      else if (MenuItem == RTC_SECONDS_SET)            {
                                                        RtcCurrentValue[SECONDS]                                            = TemporaryNewValue;
                                                        RTC_MODULE.settime( RtcCurrentValue[SECONDS],  -1, -1,  -1, -1, -1, -1);
      }
      else if (MenuItem == RTC_WEEK_DAY_SET)           {
                                                        RtcCurrentValue[WEEK_DAY]                                           = TemporaryNewValue;
                                                        RTC_MODULE.settime( RtcCurrentValue[SECONDS],  -1, -1,  -1, -1, -1, RtcCurrentValue[WEEK_DAY]);
      }
      else if (MenuItem == RTC_DAY_SET)                {
                                                        RtcCurrentValue[DAY]                                                = TemporaryNewValue;
                                                        RTC_MODULE.settime( RtcCurrentValue[SECONDS],  -1, -1,  RtcCurrentValue[DAY], -1, -1, -1);
      }
      else if (MenuItem == RTC_MONTH_SET)              {
                                                        RtcCurrentValue[MONTH]                                              = TemporaryNewValue;
                                                        RTC_MODULE.settime( RtcCurrentValue[SECONDS],  -1, -1,  -1, RtcCurrentValue[MONTH], -1, -1);
      }
      else if (MenuItem == RTC_YEAR_SET)               {
                                                        RtcCurrentValue[YEAR]                                               = TemporaryNewValue;
                                                        RTC_MODULE.settime( RtcCurrentValue[SECONDS],  -1, -1,  -1, -1, RtcCurrentValue[YEAR], -1);
      }
        NOTIFICATION_NEW_VALUE_SAVED_FUNC();
    }
  }
  else if (!ButtonState[BTN_OK] && ButtonOkPushFlag) {
    ButtonOkPushFlag = false;
  }
}

void SCENARIO_FUNC() {
  
  if (CurrentResumeOrBorderSettingValue[SELECTED_LIGHTING_RESUME]         == LIGHT_AUTO) {
    if ( RealLightingValue < CurrentResumeOrBorderSettingValue[MIN_HUMIDITY])                                             I2C_RELAY_2.digitalWrite(1, HIGH);
    else if(RealLightingValue > CurrentResumeOrBorderSettingValue[MIN_HUMIDITY] + HUMIDITY_GAP)                           I2C_RELAY_2.digitalWrite(1, LOW);
  }
  else if (CurrentResumeOrBorderSettingValue[SELECTED_LIGHTING_RESUME]    == LIGHT_ON)                                    I2C_RELAY_2.digitalWrite(1, HIGH);
  else                                                                                                                    I2C_RELAY_2.digitalWrite(1, LOW);
  
  if (CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME]      == AIRING_ROOM_AUTO) {
    
    if (RealAirTemperature > CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM]) {
      if (StartOpeningWindowFlag) {
        StartOpeningWindowFlag = false;
        StartClosingWindowFlag = true;
        OpeningWindowTimer = millis();
      }
      if (OpeningWindowTimer + OPEN_WINDOW_TIME > millis()){                                                              I2C_RELAY_1.digitalWrite(1, HIGH);  I2C_RELAY_1.digitalWrite(2, LOW);}
      else {                                                                                                              I2C_RELAY_1.digitalWrite(1, LOW);   I2C_RELAY_1.digitalWrite(2, LOW);}
    }
    
    else if (RealAirTemperature <= CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM]) {
      if (StartClosingWindowFlag) {
        StartClosingWindowFlag = false;
        StartOpeningWindowFlag = true;
        ClosingWindowTimer = millis();
      }
      if (ClosingWindowTimer + CLOSE_WINDOW_TIME > millis()){                                                             I2C_RELAY_1.digitalWrite(1, LOW);   I2C_RELAY_1.digitalWrite(2, HIGH);}
      else  {                                                                                                             I2C_RELAY_1.digitalWrite(1, LOW);   I2C_RELAY_1.digitalWrite(2, LOW); }
    }
  }
  else if (CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME] == AIRING_ROOM_CLOSE){                          I2C_RELAY_1.digitalWrite(1, LOW);   I2C_RELAY_1.digitalWrite(2, HIGH);}
  else if (CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME] == AIRING_ROOM_OPEN){                           I2C_RELAY_1.digitalWrite(1, HIGH);  I2C_RELAY_1.digitalWrite(2, LOW);}
  else {                                                                                                                  I2C_RELAY_1.digitalWrite(1, LOW);   I2C_RELAY_1.digitalWrite(2, LOW);}

  if (CurrentResumeOrBorderSettingValue[SELECTED_PUMP_WORK_RESUME]        == PUMP_AUTO) {

    if (CurrentResumeOrBorderSettingValue[TIME_FOR_START_PUMP] == RtcCurrentValue[HOURS] && RtcCurrentValue[MINUTES] == 0 && RtcCurrentValue[SECONDS] >= 0 && RtcCurrentValue[SECONDS] <= 1 && StartPumpWorkingFlag) {
        StartPumpWorkingFlag = false;
        AutopumpWorkingTimeTimer = millis();
    }
    if ((AutopumpWorkingTimeTimer + (CurrentResumeOrBorderSettingValue[DELAY_TIME_FOR_PUMP_WORKING] * 1000)) > millis() && !StartPumpWorkingFlag){
                                                                                                                          I2C_RELAY_2.digitalWrite(2, HIGH);
    }
    else {                                                                                                                I2C_RELAY_2.digitalWrite(2, LOW);
                                                                                                                          if(RtcCurrentValue[HOURS] == 0 && RtcCurrentValue[MINUTES] == 0 && RtcCurrentValue[SECONDS] >= 0 && RtcCurrentValue[SECONDS] <= 1){
                                                                                                                            StartPumpWorkingFlag = true;
                                                                                                                          }
    }
  }
  else if (CurrentResumeOrBorderSettingValue[SELECTED_PUMP_WORK_RESUME]   == PUMP_ON)                                     I2C_RELAY_2.digitalWrite(2, HIGH);
  else                                                                                                                    I2C_RELAY_2.digitalWrite(2, LOW);

  if (CurrentResumeOrBorderSettingValue[SELECTED_RELE_1_RESUME]           == RELE_ON) {
    while ((ReleTestTimer + RELE_TEST_FULL_TIME) > millis())                                                             {I2C_RELAY_1.digitalWrite(1, HIGH);}
                                                                                                                          I2C_RELAY_1.digitalWrite(1, LOW);
    CurrentResumeOrBorderSettingValue[SELECTED_RELE_1_RESUME] = RELE_OFF;
  }
  
  if (CurrentResumeOrBorderSettingValue[SELECTED_RELE_2_RESUME]           == RELE_ON) {
    while ((ReleTestTimer + RELE_TEST_FULL_TIME) > millis())                                                             {I2C_RELAY_1.digitalWrite(2, HIGH);}
                                                                                                                          I2C_RELAY_1.digitalWrite(2, LOW);
    CurrentResumeOrBorderSettingValue[SELECTED_RELE_2_RESUME] = RELE_OFF;
  }
  
  if (CurrentResumeOrBorderSettingValue[SELECTED_RELE_3_RESUME]           == RELE_ON) {
    while ((ReleTestTimer + RELE_TEST_FULL_TIME) > millis())                                                             {I2C_RELAY_2.digitalWrite(1, HIGH);}
                                                                                                                          I2C_RELAY_2.digitalWrite(1, LOW);
    CurrentResumeOrBorderSettingValue[SELECTED_RELE_3_RESUME] = RELE_OFF;
  }
  if (CurrentResumeOrBorderSettingValue[SELECTED_RELE_4_RESUME]           == RELE_ON) {
    while ((ReleTestTimer + RELE_TEST_FULL_TIME) > millis())                                                             {I2C_RELAY_2.digitalWrite(2, HIGH);}
                                                                                                                          I2C_RELAY_2.digitalWrite(2, LOW);
    CurrentResumeOrBorderSettingValue[SELECTED_RELE_4_RESUME] = RELE_OFF;
  }
  if (CurrentResumeOrBorderSettingValue[SELECTED_ZUMMER_RESUME]           == ZUMMER_AUTO) {
    if (CurrentResumeOrBorderSettingValue[SELECTED_LIGHTING_RESUME] != 2 || CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME] != 2 || CurrentResumeOrBorderSettingValue[SELECTED_PUMP_WORK_RESUME] != 2) {
      if (RealAirTemperature < CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM] || RealAirTemperature > CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] || RealGroundHumidityPercent < MIN_GROUND_HUMIDITY_PERCENT || RealGroundHumidityPercent > MAX_GROUND_HUMIDITY_PERCENT ) {
        if (StartZummerWorkingFlag) {
          StartZummerWorkingFlag = false;
          ZummerWorkingTimeTimer = millis();
        }
      }
      if ((ZummerWorkingTimeTimer + ZUMMER_WORKING_TIME) > millis())                                                     {digitalWrite(ZUMMER_PIN, HIGH); ZummerNotWorkingTimeTimer = millis();}
      else {                                                                                                              digitalWrite(ZUMMER_PIN, LOW);  if(millis() > (ZummerNotWorkingTimeTimer + ZUMMER_NOT_WORKING_TIME)) StartZummerWorkingFlag = true;}
    }
    else                                                                                                                  digitalWrite(ZUMMER_PIN, LOW);
  }
  else if (CurrentResumeOrBorderSettingValue[SELECTED_ZUMMER_RESUME]      == ZUMMER_ON)                                   digitalWrite(ZUMMER_PIN, HIGH);
  else                                                                                                                    digitalWrite(ZUMMER_PIN, LOW);
}

void MENU_DEPTH() {
  if (ChangeMenuScreenFlag) {
    ChangeMenuScreenFlag = false;
    LCD.clear();
  }
  switch (MenuItem) {
    case DEFAULT_SCREEN:
      if (ChangeDefaultPageTimer_1 + DEFAULT_SCREEN_CHANGE_PAGE_TIME > millis()) {
        
        LCD.setCursor(0, 0);          LCD.print("T/B(g)=");
        if (RealGroundTemperature < 10) {
          LCD.setCursor(8, 0);        LCD.print(" ");
          LCD.setCursor(9, 0);        LCD.print(RealGroundTemperature);
        }
        else{LCD.setCursor(8, 0);     LCD.print(RealGroundTemperature);}
        LCD.setCursor(10, 0);         LCD.print("\1");
        LCD.setCursor(11, 0);         LCD.print("/");
        if (RealGroundHumidityPercent < 10) {
          LCD.setCursor(12, 0);       LCD.print("  ");
          LCD.setCursor(14, 0);       LCD.print(RealGroundHumidityPercent);
        }
        else if (RealGroundHumidityPercent < 100) {
          LCD.setCursor(12, 0);       LCD.print(" ");
          LCD.setCursor(13, 0);       LCD.print(RealGroundHumidityPercent);
        }
        else{LCD.setCursor(12, 0);    LCD.print(RealGroundHumidityPercent);}
        LCD.setCursor(15, 0);         LCD.print("%");
        
        LCD.setCursor(0, 1);          LCD.print("T/B(a)=");
        if (RealAirTemperature < 10) {
          LCD.setCursor(8, 1);        LCD.print(" ");
          LCD.setCursor(9, 1);        LCD.print(RealAirTemperature);
        }
        else{LCD.setCursor(8, 1);     LCD.print(RealAirTemperature);}
        LCD.setCursor(10, 1);         LCD.print("\1");
        LCD.setCursor(11, 1);         LCD.print("/");
        if (RealAirHumidityPercent < 10) {
          LCD.setCursor(12, 1);       LCD.print("  ");
          LCD.setCursor(14, 1);       LCD.print(RealAirHumidityPercent);
        }
        else if (RealAirHumidityPercent < 100) {
          LCD.setCursor(12, 1);       LCD.print(" ");
          LCD.setCursor(13, 1);       LCD.print(RealAirHumidityPercent);
        }
        else{LCD.setCursor(13, 1);    LCD.print(RealAirHumidityPercent);}
        LCD.setCursor(15, 1);         LCD.print("%");
        ChangeDefaultPageTimer_2 = millis();
      }
      
      else {
        if (ChangeDefaultPageTimer_2 + DEFAULT_SCREEN_CHANGE_PAGE_TIME > millis()) {

          if (RtcCurrentValue[HOURS] > 10) {
            LCD.setCursor(0, 0);      LCD.print(RtcCurrentValue[HOURS]);
          }
          else {
            LCD.setCursor(0, 0);      LCD.print("0");
            LCD.setCursor(1, 0);      LCD.print(RtcCurrentValue[HOURS]);
          }
          LCD.setCursor(2, 0);        LCD.print(":");
          
          if (RtcCurrentValue[MINUTES] > 10) {
            LCD.setCursor(3, 0);      LCD.print(RtcCurrentValue[MINUTES]);
          }
          else {
            LCD.setCursor(3, 0);      LCD.print("0");
            LCD.setCursor(4, 0);      LCD.print(RtcCurrentValue[MINUTES]);
          }
          LCD.setCursor(5, 0);        LCD.print(" ");
          LCD.setCursor(6, 0);        LCD.print("LIGHT=");
          
          if (RealLightingValue < 10) {
            LCD.setCursor(12, 0);     LCD.print("   ");
            LCD.setCursor(15, 0);     LCD.print(RealLightingValue);
          }
          
          else if (RealLightingValue < 100 ) {
            LCD.setCursor(12, 0);     LCD.print("  ");
            LCD.setCursor(14, 0);     LCD.print(RealLightingValue);
          }

          else if (RealLightingValue < 1000 ) {
            LCD.setCursor(12, 0);     LCD.print(" ");
            LCD.setCursor(13, 0);     LCD.print(RealLightingValue);
          }

          else {LCD.setCursor(12, 0); LCD.print(RealLightingValue);}

          LCD.setCursor(0, 1);        LCD.print("LR=");
          LCD.setCursor(3, 1);
          switch (CurrentResumeOrBorderSettingValue[SELECTED_LIGHTING_RESUME]) {
            case LIGHT_OFF:           LCD.print("-"); break;
            case LIGHT_ON:            LCD.print("+"); break;
            case LIGHT_AUTO:          LCD.print("A"); break;
          }

          LCD.setCursor(4, 1);        LCD.print(" ");
          LCD.setCursor(5, 1);        LCD.print("ARR=");
          LCD.setCursor(9, 1);
          switch (CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME]) {
            case AIRING_ROOM_OFF:     LCD.print("-"); break;
            case AIRING_ROOM_OPEN:    LCD.print("O"); break;
            case AIRING_ROOM_CLOSE:   LCD.print("C"); break;
            case AIRING_ROOM_AUTO:    LCD.print("A"); break;
          }

          LCD.setCursor(10, 1);       LCD.print(" ");
          LCD.setCursor(11, 1);       LCD.print("APR=");
          LCD.setCursor(15, 1);
          switch (CurrentResumeOrBorderSettingValue[SELECTED_PUMP_WORK_RESUME]) {
            case PUMP_OFF:            LCD.print("-"); break;
            case PUMP_ON:             LCD.print("+"); break;
            case PUMP_AUTO:           LCD.print("A"); break;
          }
        }
        else { ChangeDefaultPageTimer_1 = millis();  LCD.clear();}
      }
      break;
    
    case LIGHT_SCREEN:
      LCD.setCursor(0, 0);            LCD.print(">");
      LCD.setCursor(1, 0);            LCD.print("LIGHT");
      break;
    
    case LIGHT_RESUME_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("LIGHT");
      LCD.setCursor(6, 0);            LCD.print("REAL=");
      LCD.setCursor(11, 0);
      
      if ( RealLightingValue < 10) {
        LCD.setCursor(11, 0);         LCD.print("   ");
        LCD.setCursor(14, 0);         LCD.print(RealLightingValue);
      }
      else if ( RealLightingValue < 100) {
        LCD.setCursor(11, 0);         LCD.print("  ");
        LCD.setCursor(13, 0);         LCD.print(RealLightingValue);
      }
      else if (RealLightingValue < 1000) {
        LCD.setCursor(11, 0);         LCD.print(" ");
        LCD.setCursor(12, 0);         LCD.print(RealLightingValue);
      }
      else {
        LCD.setCursor(11, 0);         LCD.print(RealLightingValue);
      }
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("RESUME");
      LCD.setCursor(11, 1);
      switch (CurrentResumeOrBorderSettingValue[SELECTED_LIGHTING_RESUME]) {
        case LIGHT_OFF:               LCD.print(" OFF");  break;
        case LIGHT_ON:                LCD.print("  ON");  break;
        case LIGHT_AUTO:              LCD.print("AUTO");  break;
      }
      break;

    case LIGHT_MIN_LIGHT_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("LIGHT");
      LCD.setCursor(6, 0);            LCD.print("REAL=");
      LCD.setCursor(11, 0);
      if ( RealLightingValue < 10) {
        LCD.setCursor(11, 0);         LCD.print("   ");
        LCD.setCursor(14, 0);         LCD.print(RealLightingValue);
      }
      else if ( RealLightingValue < 100) {
        LCD.setCursor(11, 0);         LCD.print("  ");
        LCD.setCursor(13, 0);         LCD.print(RealLightingValue);
      }
      else if (RealLightingValue < 1000) {
        LCD.setCursor(11, 0);         LCD.print(" ");
        LCD.setCursor(12, 0);         LCD.print(RealLightingValue);
      }
      else {LCD.setCursor(11, 0);     LCD.print(RealLightingValue);}
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("MIN VALUE");
      LCD.setCursor(11, 1);           LCD.print(CurrentResumeOrBorderSettingValue[MIN_HUMIDITY]);
      break;
    
    case LIGHTING_RESUME_SET:
      LCD.setCursor(0, 0);            LCD.print("LIGHT");
      LCD.setCursor(6, 0);            LCD.print("REAL=");
      LCD.setCursor(11, 0);
      
      if ( RealLightingValue < 10) {
        LCD.setCursor(11, 0);         LCD.print("   ");
        LCD.setCursor(14, 0);         LCD.print(RealLightingValue);
      }
      else if ( RealLightingValue < 100) {
        LCD.setCursor(11, 0);         LCD.print("  ");
        LCD.setCursor(13, 0);         LCD.print(RealLightingValue);
      }
      else if (RealLightingValue < 1000) {
        LCD.setCursor(11, 0);         LCD.print(" ");
        LCD.setCursor(12, 0);         LCD.print(RealLightingValue);
      }
      else {
        LCD.setCursor(11, 0);         LCD.print(RealLightingValue);
      }
      LCD.setCursor(0, 1);            LCD.print("RESUME");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = CurrentResumeOrBorderSettingValue[SELECTED_LIGHTING_RESUME];
        TemporaryNewValue = TemporaryOldValue;
      }
      switch (TemporaryNewValue) {
        case LIGHT_OFF:               LCD.print(" OFF");  break;
        case LIGHT_ON:                LCD.print("  ON");  break;
        case LIGHT_AUTO:              LCD.print("AUTO");  break;
      }
      break;
    
    case LIGHTING_MIN_VALUE_SET:
      LCD.setCursor(0, 0);            LCD.print("LIGHT");
      LCD.setCursor(6, 0);            LCD.print("REAL=");
      LCD.setCursor(11, 0);
      
      if ( RealLightingValue < 10) {
        LCD.setCursor(11, 0);         LCD.print("   ");
        LCD.setCursor(14, 0);         LCD.print(RealLightingValue);
      }
      else if ( RealLightingValue < 100) {
        LCD.setCursor(11, 0);         LCD.print("  ");
        LCD.setCursor(13, 0);         LCD.print(RealLightingValue);
      }
      else if (RealLightingValue < 1000) {
        LCD.setCursor(11, 0);         LCD.print(" ");
        LCD.setCursor(12, 0);         LCD.print(RealLightingValue);
      }
      else {LCD.setCursor(11, 0);     LCD.print(RealLightingValue);}
      LCD.setCursor(0, 1);            LCD.print("MIN VALUE");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = CurrentResumeOrBorderSettingValue[MIN_HUMIDITY];
        TemporaryNewValue = TemporaryOldValue;
      }
                                      LCD.print(TemporaryNewValue);
      break;
    
    case AIRING_ROOM_SCREEN:
      LCD.setCursor(0, 0);            LCD.print(">");
      LCD.setCursor(1, 0);            LCD.print("AIRING ROOM");
      break;

    case AIRING_ROOM_RESUME_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("AIRING ROOM");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("RESUME");
      LCD.setCursor(11, 1);
      switch (CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME]) {
        case AIRING_ROOM_OFF:         LCD.print("  OFF"); break;
        case AIRING_ROOM_OPEN:        LCD.print(" OPEN"); break;
        case AIRING_ROOM_CLOSE:       LCD.print("CLOSE"); break;
        case AIRING_ROOM_AUTO:        LCD.print(" AUTO"); break;
      }
      break;
    
    case AIRING_ROOM_MIN_TEMP_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("AIRING ROOM");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("MIN VALUE");
      LCD.setCursor(11, 1);           LCD.print(CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM]);
      LCD.setCursor(13, 1);           LCD.print("\1");
      break;
    
    case AIRING_ROOM_MAX_TEMP_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("AIRING ROOM");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("MAX VALUE");
      LCD.setCursor(11, 1);           LCD.print(CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM]);
      LCD.setCursor(13, 1);           LCD.print("\1");
      break;
    
    case AIRING_ROOM_RESUME_SET:
      LCD.setCursor(0, 0);            LCD.print("AIRING ROOM");
      LCD.setCursor(0, 1);            LCD.print("RESUME");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME];
        TemporaryNewValue = TemporaryOldValue;
      }
      switch (TemporaryNewValue) {
        case AIRING_ROOM_OFF:         LCD.print("  OFF"); break;
        case AIRING_ROOM_OPEN:        LCD.print(" OPEN"); break;
        case AIRING_ROOM_CLOSE:       LCD.print("CLOSE"); break;
        case AIRING_ROOM_AUTO:        LCD.print(" AUTO"); break;
      }
      break;
    
    case AIRING_ROOM_MIN_TEMP_SET:
      LCD.setCursor(0, 0);            LCD.print("AIRING ROOM");
      LCD.setCursor(0, 1);            LCD.print("MIN VALUE");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM];
        TemporaryNewValue = TemporaryOldValue;
      }
                                      LCD.print(TemporaryNewValue);
      LCD.setCursor(13, 1);           LCD.print("\1");
      break;
    
    case AIRING_ROOM_MAX_TEMP_SET:
      LCD.setCursor(0, 0);            LCD.print("AIRING ROOM");
      LCD.setCursor(0, 1);            LCD.print("MAX VALUE");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM];
        TemporaryNewValue = TemporaryOldValue;
      }
                                      LCD.print(TemporaryNewValue);
      LCD.setCursor(13, 1);           LCD.print("\1");
      break;
    
    case PUMP_SCREEN:
      LCD.setCursor(0, 0);            LCD.print(">");
      LCD.setCursor(1, 0);            LCD.print("AUTOPUMP");
      break;
    
    case PUMP_RESUME_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("AUTOPUMP");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("RESUME");
      LCD.setCursor(11, 1);
      switch (CurrentResumeOrBorderSettingValue[SELECTED_PUMP_WORK_RESUME]) {
        case PUMP_OFF:                LCD.print(" OFF");  break;
        case PUMP_ON:                 LCD.print("  ON");  break;
        case PUMP_AUTO:               LCD.print("AUTO");  break;
      }
      break;
    
    case PUMP_WORKING_TIME_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("AUTOPUMP");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("HOUR");
      LCD.setCursor(11, 1);           LCD.print(CurrentResumeOrBorderSettingValue[TIME_FOR_START_PUMP]);
      break;
    
    case PUMP_DELAY_TIME_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("AUTOPUMP");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("HOW LONG");
      LCD.setCursor(11, 1);           LCD.print(CurrentResumeOrBorderSettingValue[DELAY_TIME_FOR_PUMP_WORKING]);
      break;
    
    case AUTOPUMP_RESUME_SET:
      LCD.setCursor(0, 0);            LCD.print("AUTOPUMP");
      LCD.setCursor(0, 1);            LCD.print("RESUME");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = CurrentResumeOrBorderSettingValue[SELECTED_PUMP_WORK_RESUME];
        TemporaryNewValue = TemporaryOldValue;
      }
      switch (TemporaryNewValue) {
        case PUMP_OFF:                LCD.print(" OFF");  break;
        case PUMP_ON:                 LCD.print("  ON");  break;
        case PUMP_AUTO:               LCD.print("AUTO");  break;
      }
      break;
    
    case AUTOPUMP_TIME_SET:
      LCD.setCursor(0, 0);            LCD.print("AUTOPUMP");
      LCD.setCursor(0, 1);            LCD.print("HOUR");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = CurrentResumeOrBorderSettingValue[TIME_FOR_START_PUMP];
        TemporaryNewValue = TemporaryOldValue;
      }
                                      LCD.print(TemporaryNewValue);
      break;
    
    case AUTOPUMP_WORKING_DELAY_SET:
      LCD.setCursor(0, 0);            LCD.print("AUTOPUMP");
      LCD.setCursor(0, 1);            LCD.print("HOW LONG");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = CurrentResumeOrBorderSettingValue[DELAY_TIME_FOR_PUMP_WORKING];
        TemporaryNewValue = TemporaryOldValue;
      }
                                      LCD.print(TemporaryNewValue);
      break;

    case SETTINGS_SCREEN:
      LCD.setCursor(0, 0);            LCD.print(">");
      LCD.setCursor(1, 0);            LCD.print("SETTINGS");
      break;
    
    case SETTINGS_RTC_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("RTC SETTINGS");
      break;
    
    case SETTINGS_RTC_HOUR_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("HOURS");
      LCD.setCursor(11, 1);           LCD.print(RtcCurrentValue[HOURS]);
      break;
    
    case SETTINGS_RTC_MINUTES_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("MINUTES");
      LCD.setCursor(11, 1);           LCD.print(RtcCurrentValue[MINUTES]);
      break;
    
    case SETTINGS_RTC_SECONDS_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("SECONDS");
      LCD.setCursor(11, 1);           LCD.print(RtcCurrentValue[SECONDS]);
      break;
    
    case SETTINGS_RTC_WEEK_DAY_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("WEEK DAY");
      LCD.setCursor(11, 1);           LCD.print(RtcCurrentValue[WEEK_DAY]);
      break;

    case SETTINGS_RTC_DAY_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("DAY");
      LCD.setCursor(11, 1);           LCD.print(RtcCurrentValue[DAY]);
      break;

    case SETTINGS_RTC_MONTH_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("MONTH");
      LCD.setCursor(11, 1);           LCD.print(RtcCurrentValue[MONTH]);
      break;
    
    case SETTINGS_RTC_YEAR_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("YEAR");
      LCD.setCursor(11, 1);           LCD.print(RtcCurrentValue[YEAR]);
      break;
    
    case RTC_HOUR_SET:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print("HOURS");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = RtcCurrentValue[HOURS];
        TemporaryNewValue = TemporaryOldValue;
      }
                                      LCD.print(TemporaryNewValue);
      break;
    
    case RTC_MINUTE_SET:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print("MINUTES");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = RtcCurrentValue[MINUTES];
        TemporaryNewValue = TemporaryOldValue;
      }
                                      LCD.print(TemporaryNewValue);
      break;

    case RTC_SECONDS_SET:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print("SECONDS");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = RtcCurrentValue[SECONDS];
        TemporaryNewValue = TemporaryOldValue;
      }
                                      LCD.print(TemporaryNewValue);
      break;
    
    case RTC_WEEK_DAY_SET:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print("WEEK DAY");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = RtcCurrentValue[WEEK_DAY];
        TemporaryNewValue = TemporaryOldValue;
      }
                                      LCD.print(TemporaryNewValue);
      break;

    case RTC_DAY_SET:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print("DAY");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = RtcCurrentValue[DAY];
        TemporaryNewValue = TemporaryOldValue;
      }
                                      LCD.print(TemporaryNewValue);
      break;
    
    case RTC_MONTH_SET:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print("MONTH");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = RtcCurrentValue[MONTH];
        TemporaryNewValue = TemporaryOldValue;
      }
                                      LCD.print(TemporaryNewValue);
      break;

    case RTC_YEAR_SET:
      LCD.setCursor(0, 0);            LCD.print("RTC SETTINGS");
      LCD.setCursor(0, 1);            LCD.print("YEAR");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = RtcCurrentValue[YEAR];
        TemporaryNewValue = TemporaryOldValue;
      }
                                      LCD.print(TemporaryNewValue);
      break;

    case SETTINGS_RELE_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("RELE SETTINGS");
      break;

    case SETTINGS_RELE_1_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("RELE SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("RELE 1");
      LCD.setCursor(11, 1);           LCD.print("TEST");
      break;

    case SETTINGS_RELE_2_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("RELE SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("RELE 2");
      LCD.setCursor(11, 1);           LCD.print("TEST");
      break;

    case SETTINGS_RELE_3_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("RELE SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("RELE 3");
      LCD.setCursor(11, 1);           LCD.print("TEST");
      break;

    case SETTINGS_RELE_4_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("RELE SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("RELE 4");
      LCD.setCursor(11, 1);           LCD.print("TEST");
      break;

    case RELE_1_TEST_SET:
      LCD.setCursor(0, 0);            LCD.print("RELE SETTINGS");
      LCD.setCursor(0, 1);            LCD.print("RELE 1");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);           LCD.print("TEST");
      break;

    case RELE_2_TEST_SET:
      LCD.setCursor(0, 0);            LCD.print("RELE SETTINGS");
      LCD.setCursor(0, 1);            LCD.print("RELE 2");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);           LCD.print("TEST");
      break;

    case RELE_3_TEST_SET:
      LCD.setCursor(0, 0);            LCD.print("RELE SETTINGS");
      LCD.setCursor(0, 1);            LCD.print("RELE 3");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);           LCD.print("TEST");
      break;

    case RELE_4_TEST_SET:
      LCD.setCursor(0, 0);            LCD.print("RELE SETTINGS");
      LCD.setCursor(0, 1);            LCD.print("RELE 4");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);           LCD.print("TEST");
      break;

    case SETTINGS_ZUMMER_SCREEN:
      LCD.setCursor(0, 0);            LCD.print("SETTINGS");
      LCD.setCursor(0, 1);            LCD.print(">");
      LCD.setCursor(1, 1);            LCD.print("ZUMMER");
      LCD.setCursor(11, 1);
      switch (CurrentResumeOrBorderSettingValue[SELECTED_ZUMMER_RESUME]) {
        case ZUMMER_OFF:              LCD.print(" OFF");  break;
        case ZUMMER_ON:               LCD.print("  ON");  break;
        case ZUMMER_AUTO:             LCD.print("AUTO");  break;
      }
      break;

    case ZUMMER_RESUME_SET:
      LCD.setCursor(0, 0);            LCD.print("SETTINGS");
      LCD.setCursor(0, 1);            LCD.print("ZUMMER");
      LCD.setCursor(10, 1);           LCD.print(">");
      LCD.setCursor(11, 1);
      if (SetNewResumeOrBorderValueFlag) {
        SetNewResumeOrBorderValueFlag = false;
        TemporaryOldValue = CurrentResumeOrBorderSettingValue[SELECTED_ZUMMER_RESUME];
        TemporaryNewValue = TemporaryOldValue;
      }
      switch (TemporaryNewValue) {
        case ZUMMER_OFF:              LCD.print(" OFF");  break;
        case ZUMMER_ON:               LCD.print("  ON");  break;
        case ZUMMER_AUTO:             LCD.print("AUTO");  break;
      }
      break;
  }
}

#if GSM_USING_FLAG

void GSM_USE_FUNC() {
  if (millis() % 1000 < 5) {
    delay(5);
    if (GSM_MODULE.SMSavailable()) {
      
      GSM_MODULE.SMSread(SmsTxt, SmsNum);
      
      SmsTextAnswer_1 = "";
      SmsTextAnswer_2 = "";
      SmsTextAnswer_3 = "";
      SmsTextAnswer_4 = "";
      SmsTextAnswer_5 = "";
      SmsTextAnswer_6 = "";
      
      StrSmsTxt = SmsTxt;
      for (int m = 0; m < sizeof(SMS_REQUEST_TEXT) / sizeof(SMS_REQUEST_TEXT[0]); m++) {
        if (StrSmsTxt.indexOf(SMS_REQUEST_TEXT[m]) > -1 && SMS_REQUEST_TEXT[m] != 0) {
          ColonSymbol = m;

          
          if (ColonSymbol == SMS_LIGHT_RESUME) {
            if (strchr(SmsTxt, ' ')) {
              ColonSymbol = uint8_t(strchr(SmsTxt, ' ' ) - SmsTxt);
              if (SmsTxt[ColonSymbol + 1] == '-' || SmsTxt[ColonSymbol + 1] == '+' || SmsTxt[ColonSymbol + 1] == 'A') {
                if (SmsTxt[ColonSymbol + 1] == '-') {
                  CurrentResumeOrBorderSettingValue[SELECTED_LIGHTING_RESUME] = LIGHT_OFF;
                  SmsTextAnswer_1 = String(F("Режим освещения успешно изменён на: выключено (-)"));
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
                }
                else if (SmsTxt[ColonSymbol + 1] == '+') {
                  CurrentResumeOrBorderSettingValue[SELECTED_LIGHTING_RESUME] = LIGHT_ON;
                  SmsTextAnswer_1 = String(F("Режим освещения успешно изменён на: включено (+)"));
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
                }
                else if (SmsTxt[ColonSymbol + 1] == 'A') {
                  CurrentResumeOrBorderSettingValue[SELECTED_LIGHTING_RESUME] = LIGHT_AUTO;
                  SmsTextAnswer_1 = String(F("Режим освещения успешно изменён на: авто (A)"));
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
                }
              }
              else                                          GSM_MODULE.SMSsend(F("Ошибка при изменении режима освещения!"), Number);
            }
          }

          
          else if (ColonSymbol == SMS_LIGHTING_MIN_VALUE_SET) {
            
            bool              SetNewHumidityValueOkFlag                 = false;
            bool              SetNewHumidityValueErrorFlag              = false;
            
            if (strchr(SmsTxt, ' ')) {
              ColonSymbol = uint8_t(strchr(SmsTxt, ' ' ) - SmsTxt);
              if (SmsTxt[ColonSymbol + 1] >= '0' && SmsTxt[ColonSymbol + 1] <= '9') {
                if (SmsTxt[ColonSymbol + 2] >= '0' && SmsTxt[ColonSymbol + 2] <= '9') {
                  if (SmsTxt[ColonSymbol + 3] >= '0' && SmsTxt[ColonSymbol + 3] <= '9') {
                    if (SmsTxt[ColonSymbol + 4] >= '0' && SmsTxt[ColonSymbol + 4] <= '9' && SmsTxt[ColonSymbol + 1] == '1' && SmsTxt[ColonSymbol + 2] == '0') {
                      if (SmsTxt[ColonSymbol + 3] == '0' || SmsTxt[ColonSymbol + 3] == '1') {
                        CurrentResumeOrBorderSettingValue[MIN_HUMIDITY] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 1000 + uint8_t(SmsTxt[ColonSymbol + 2] - '0') * 100 + uint8_t(SmsTxt[ColonSymbol + 3] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 4] - '0');
                              SetNewHumidityValueOkFlag                 = true;
                      }
                      else if (SmsTxt[ColonSymbol + 3] == '2') {
                        if (SmsTxt[ColonSymbol + 4] <= '3') {
                          CurrentResumeOrBorderSettingValue[MIN_HUMIDITY] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 1000 + uint8_t(SmsTxt[ColonSymbol + 2] - '0') * 100 + uint8_t(SmsTxt[ColonSymbol + 3] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 4] - '0');
                              SetNewHumidityValueOkFlag                 = true;
                        }
                        else  SetNewHumidityValueErrorFlag              = true;
                      }
                      else    SetNewHumidityValueErrorFlag              = true;
                    }
                    else {
                      CurrentResumeOrBorderSettingValue[MIN_HUMIDITY] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 100 + uint8_t(SmsTxt[ColonSymbol + 2] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 3] - '0');
                              SetNewHumidityValueOkFlag                 = true;
                    }
                  }
                  else {
                    CurrentResumeOrBorderSettingValue[MIN_HUMIDITY] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 2] - '0');
                              SetNewHumidityValueOkFlag                 = true;
                  }
                }
                else {
                  CurrentResumeOrBorderSettingValue[MIN_HUMIDITY] = uint8_t(SmsTxt[ColonSymbol + 1] - '0');
                              SetNewHumidityValueOkFlag                 = true;
                }
              }
              else            SetNewHumidityValueErrorFlag              = true;
            }
            else              SetNewHumidityValueErrorFlag              = true;
            if (SetNewHumidityValueOkFlag) {
              SmsTextAnswer_1 = String(F("Новая граница освещённости успешно изменена на: ")) + CurrentResumeOrBorderSettingValue[MIN_HUMIDITY];
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
            }
            if (SetNewHumidityValueErrorFlag)               GSM_MODULE.SMSsend(F("Ошибка при изменении границы освещённости!"), Number);
          }

          
          else if (ColonSymbol == SMS_AIRING_ROOM_RESUME) {
            if (strchr(SmsTxt, ' ')) {
              ColonSymbol = uint8_t(strchr(SmsTxt, ' ' ) - SmsTxt);
              if (SmsTxt[ColonSymbol + 1] == '-' || SmsTxt[ColonSymbol + 1] == 'O' || SmsTxt[ColonSymbol + 1] == 'C' || SmsTxt[ColonSymbol + 1] == 'A') {
                if (SmsTxt[ColonSymbol + 1] == '-') {
                  CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME] = AIRING_ROOM_OFF;
                  SmsTextAnswer_1 = String(F("Режим проветривания успешно изменён на: выключено (-)"));
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
                }
                else if (SmsTxt[ColonSymbol + 1] == 'O') {
                  CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME] = AIRING_ROOM_OPEN;
                              StartClosingWindowFlag                    = true;
                              StartOpeningWindowFlag                    = false;
                  SmsTextAnswer_1 = String(F("Режим проветривания успешно изменён на: открыть (O)"));
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
                }
                else if (SmsTxt[ColonSymbol + 1] == 'C') {
                  CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME] = AIRING_ROOM_CLOSE;
                              StartClosingWindowFlag                    = false;
                              StartOpeningWindowFlag                    = true;
                  SmsTextAnswer_1 = String(F("Режим проветривания успешно изменён на: закрыть (C)"));
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
                }
                else if (SmsTxt[ColonSymbol + 1] == 'A') {
                  CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME] = AIRING_ROOM_AUTO;
                              StartClosingWindowFlag                    = true;
                              StartOpeningWindowFlag                    = true;
                  SmsTextAnswer_1 = String(F("Режим проветривания успешно изменён на: авто (A)"));
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
                }
              }
              else                                          GSM_MODULE.SMSsend(F("Ошибка при изменении режима проветривания!"), Number);
            }
          }
          
          
          else if (ColonSymbol == SMS_AIRING_ROOM_MIN_TEMP_SET) {

            bool              SetNewMinTempValueOkFlag                  = false;
            bool              SetNewMinTempValueErrorFlag               = false;

            if (strchr(SmsTxt, ' ')) {
              ColonSymbol = uint8_t(strchr(SmsTxt, ' ' ) - SmsTxt);
              if (SmsTxt[ColonSymbol + 1] >= '0' && SmsTxt[ColonSymbol + 1] <= '9') {
                if (SmsTxt[ColonSymbol + 2] >= '0' && SmsTxt[ColonSymbol + 2] <= '9') {
                  if (SmsTxt[ColonSymbol + 1] >= '0' && SmsTxt[ColonSymbol + 1] <= '4') {
                    CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 2] - '0');
                    if (CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM] >= CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM]) {
                      CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM] = CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] - 1;
                    }
                              SetNewMinTempValueOkFlag                  = true;
                  }
                  else        SetNewMinTempValueErrorFlag               = true;
                }
                else {
                  CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM] = uint8_t(SmsTxt[ColonSymbol + 1] - '0');
                  if (CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM] >= CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM]) {
                    CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM] = CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] - 1;
                  }
                              SetNewMinTempValueOkFlag                  = true;
                }
              }
              else            SetNewMinTempValueErrorFlag               = true;
            }
            else              SetNewMinTempValueErrorFlag               = true;
            if (SetNewMinTempValueOkFlag) {
              SmsTextAnswer_1 = String(F("Минимальная температура проветривания успешно изменена на: ")) + CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM];
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
            }
            if (SetNewMinTempValueErrorFlag)                GSM_MODULE.SMSsend(F("Ошибка при изменении минимальной температуры проветривания полива!"), Number);
          }


          else if (ColonSymbol == SMS_AIRING_ROOM_MAX_TEMP_SET) {

            bool              SetNewMaxTempValueOkFlag                  = false;
            bool              SetNewMaxTempValueErrorFlag               = false;

            if (strchr(SmsTxt, ' ')) {
              ColonSymbol = uint8_t(strchr(SmsTxt, ' ' ) - SmsTxt);
              if (SmsTxt[ColonSymbol + 1] >= '1' && SmsTxt[ColonSymbol + 1] <= '9') {
                if (SmsTxt[ColonSymbol + 2] >= '0' && SmsTxt[ColonSymbol + 2] <= '9') {
                  if (SmsTxt[ColonSymbol + 1] >= '0' && SmsTxt[ColonSymbol + 1] <= '4') {
                    CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 2] - '0');
                    if (CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] <= CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM]) {
                      CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] = CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM] + 1;
                    }
                              SetNewMaxTempValueOkFlag                  = true;
                  }
                  else if (SmsTxt[ColonSymbol + 1] == '5') {
                    if (SmsTxt[ColonSymbol + 1] == '0') {
                      CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 2] - '0');
                      if (CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] <= CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM]) {
                        CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] = CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM] + 1;
                      }
                              SetNewMaxTempValueOkFlag                  = true;
                    }
                    else      SetNewMaxTempValueErrorFlag               = true;
                  }
                  else        SetNewMaxTempValueErrorFlag               = true;
                }
                else {
                  CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] = uint8_t(SmsTxt[ColonSymbol + 1] - '0');
                  if (CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] <= CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM]) {
                    CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] = CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM] + 1;
                  }
                              SetNewMaxTempValueOkFlag                  = true;
                }
              }
              else            SetNewMaxTempValueErrorFlag               = true;
            }
            else              SetNewMaxTempValueErrorFlag               = true;
            if (SetNewMaxTempValueOkFlag) {
              SmsTextAnswer_1 = String(F("Минимальная температура проветривания успешно изменена на: ")) + CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM];
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
            }
            if (SetNewMaxTempValueErrorFlag)                GSM_MODULE.SMSsend(F("Ошибка при изменении максимальной температуры проветривания полива!"), Number);
          }

          
          else if (ColonSymbol == SMS_AUTOPUMP_RESUME) {
            if (strchr(SmsTxt, ' ')) {
              ColonSymbol = uint8_t(strchr(SmsTxt, ' ' ) - SmsTxt);
              if (SmsTxt[ColonSymbol + 1] == '-' || SmsTxt[ColonSymbol + 1] == '+' || SmsTxt[ColonSymbol + 1] == 'A') {
                if (SmsTxt[ColonSymbol + 1] == '-') {
                  CurrentResumeOrBorderSettingValue[SELECTED_PUMP_WORK_RESUME] = PUMP_OFF;
                  SmsTextAnswer_1 = String(F("Режим полива успешно изменён на: выключено (-)"));
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
                }
                else if (SmsTxt[ColonSymbol + 1] == '+') {
                  CurrentResumeOrBorderSettingValue[SELECTED_PUMP_WORK_RESUME] = PUMP_ON;
                  SmsTextAnswer_1 = String(F("Режим полива успешно изменён на: включено (+)"));
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
                }
                else if (SmsTxt[ColonSymbol + 1] == 'A') {
                  CurrentResumeOrBorderSettingValue[SELECTED_PUMP_WORK_RESUME] = PUMP_AUTO;
                              StartPumpWorkingFlag                      = true;
                  SmsTextAnswer_1 = String(F("Режим полива успешно изменён на: авто (A)"));
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
                }
              }
              else                                          GSM_MODULE.SMSsend(F("Ошибка при изменении режима полива!"), Number);
            }
            else                                            GSM_MODULE.SMSsend(F("Ошибка при изменении режима полива!"), Number);
          }


          else if (ColonSymbol == SMS_AUTOPUMP_WORKING_TIME) {

            bool              SetNewStartPumpTimeValueOkFlag            = false;
            bool              SetNewStartPumpTimeValueErrorFlag         = false;

            if (strchr(SmsTxt, ' ')) {
              ColonSymbol = uint8_t(strchr(SmsTxt, ' ' ) - SmsTxt);
              if (SmsTxt[ColonSymbol + 1] >= '0' && SmsTxt[ColonSymbol + 1] <= '9') {
                if (SmsTxt[ColonSymbol + 2] >= '0' && SmsTxt[ColonSymbol + 2] <= '9') {
                  if (SmsTxt[ColonSymbol + 1] >= '0' && SmsTxt[ColonSymbol + 1] <= '2') {
                    if (SmsTxt[ColonSymbol + 1] == '0' || SmsTxt[ColonSymbol + 1] == '1') {
                      CurrentResumeOrBorderSettingValue[TIME_FOR_START_PUMP] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 2] - '0');
                              StartPumpWorkingFlag                      = true;
                              SetNewStartPumpTimeValueOkFlag            = true;
                    }
                    else if (SmsTxt[ColonSymbol + 1] == '2') {
                      if (SmsTxt[ColonSymbol + 2] <= '3') {
                        CurrentResumeOrBorderSettingValue[TIME_FOR_START_PUMP] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 2] - '0');
                              StartPumpWorkingFlag                      = true;
                              SetNewStartPumpTimeValueOkFlag            = true;
                      }
                      else    SetNewStartPumpTimeValueErrorFlag         = true;
                    }
                  }
                  else        SetNewStartPumpTimeValueErrorFlag         = true;
                }
                else {
                  CurrentResumeOrBorderSettingValue[TIME_FOR_START_PUMP] = uint8_t(SmsTxt[ColonSymbol + 1] - '0');
                              StartPumpWorkingFlag                      = true;
                              SetNewStartPumpTimeValueOkFlag            = true;
                }
              }
              else            SetNewStartPumpTimeValueErrorFlag         = true;
            }
            else              SetNewStartPumpTimeValueErrorFlag         = true;
            if (SetNewStartPumpTimeValueOkFlag) {
              SmsTextAnswer_1 = String(F("Время включения полива успешно изменено на: ")) + CurrentResumeOrBorderSettingValue[TIME_FOR_START_PUMP];
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
            }
            if (SetNewStartPumpTimeValueErrorFlag)          GSM_MODULE.SMSsend(F("Ошибка при изменении времени начала полива!"), Number);
          }

          
          else if (ColonSymbol == SMS_AUTOPUMP_WORKING_DELAY) {

            bool              SetNewStartPumpWorkingDelayValueOkFlag    = false;
            bool              SetNewStartPumpWorkingDelayValueErrorFlag = false;

            if (strchr(SmsTxt, ' ')) {
              ColonSymbol = uint8_t(strchr(SmsTxt, ' ' ) - SmsTxt);
              if (SmsTxt[ColonSymbol + 1] >= '0' && SmsTxt[ColonSymbol + 1] <= '9') {
                if (SmsTxt[ColonSymbol + 2] >= '0' && SmsTxt[ColonSymbol + 2] <= '9') {
                  if (SmsTxt[ColonSymbol + 3] >= '0' && SmsTxt[ColonSymbol + 3] <= '9') {
                    if (SmsTxt[ColonSymbol + 2] >= '0' && SmsTxt[ColonSymbol + 2] < '3') {
                      CurrentResumeOrBorderSettingValue[DELAY_TIME_FOR_PUMP_WORKING] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 100 + uint8_t(SmsTxt[ColonSymbol + 2] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 3] - '0');
                              SetNewStartPumpWorkingDelayValueOkFlag    = true;
                    }
                    else if (SmsTxt[ColonSymbol + 1] == '3') {
                      if (SmsTxt[ColonSymbol + 2] == '0') {
                        if (SmsTxt[ColonSymbol + 3] == '0') {
                          CurrentResumeOrBorderSettingValue[DELAY_TIME_FOR_PUMP_WORKING] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 100 + uint8_t(SmsTxt[ColonSymbol + 2] - '0') * 100 + uint8_t(SmsTxt[ColonSymbol + 3] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 4] - '0');
                              SetNewStartPumpWorkingDelayValueOkFlag    = true;
                        }
                        else  SetNewStartPumpWorkingDelayValueErrorFlag = true;
                      }
                      else    SetNewStartPumpWorkingDelayValueErrorFlag = true;
                    }
                    else      SetNewStartPumpWorkingDelayValueErrorFlag = true;
                  }
                  else {
                    CurrentResumeOrBorderSettingValue[DELAY_TIME_FOR_PUMP_WORKING] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 2] - '0');
                              SetNewStartPumpWorkingDelayValueOkFlag    = true;
                  }
                }
                else          SetNewStartPumpWorkingDelayValueErrorFlag = true;
              }
              else            SetNewStartPumpWorkingDelayValueErrorFlag = true;
            }
            else              SetNewStartPumpWorkingDelayValueErrorFlag = true;
            if (SetNewStartPumpWorkingDelayValueOkFlag) {
              SmsTextAnswer_1 = String(F("Новая длительность полива успешно изменена на: ")) + CurrentResumeOrBorderSettingValue[DELAY_TIME_FOR_PUMP_WORKING] + F(" секунд");
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
            }
            if (SetNewStartPumpWorkingDelayValueErrorFlag)  GSM_MODULE.SMSsend(F("Ошибка при изменении длительности полива!"), Number);
          }

          
          else if (ColonSymbol == SMS_SET_NEW_RTC_TIME) {

            bool              SetNewRtcHoursValueOkFlag                 = false;
            bool              SetNewRtcHoursValueErrorFlag              = false;
            bool              SetNewRtcMinutesValueOkFlag               = false;
            bool              SetNewRtcMinutesValueErrorFlag            = false;

            if (strchr(SmsTxt, ':')) {
              ColonSymbol = uint8_t(strchr(SmsTxt, ':' ) - SmsTxt);
              if (SmsTxt[ColonSymbol - 1] >= '0' && SmsTxt[ColonSymbol - 1] <= '9') {
                if (SmsTxt[ColonSymbol - 2] >= '0' && SmsTxt[ColonSymbol - 2] <= '9') {
                  if (SmsTxt[ColonSymbol - 2] == '1') {
                    RtcCurrentValue[HOURS] = uint8_t(SmsTxt[ColonSymbol - 2] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol - 1] - '0');
                    //                                     s        m                   h       d    m   y  wd
                    RTC_MODULE.settime( RtcCurrentValue[SECONDS],  -1, RtcCurrentValue[HOURS],  -1, -1, -1, -1);
                              SetNewRtcHoursValueOkFlag                 = true;
                  }
                  else if (SmsTxt[ColonSymbol - 2] >= '2') {
                    if (SmsTxt[ColonSymbol - 1] < '3') {
                      RtcCurrentValue[HOURS] = uint8_t(SmsTxt[ColonSymbol - 2] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol - 1] - '0');
                      RTC_MODULE.settime( RtcCurrentValue[SECONDS],  -1, RtcCurrentValue[HOURS],  -1, -1, -1, -1);
                              SetNewRtcHoursValueOkFlag                 = true;
                    }
                    else      SetNewRtcHoursValueErrorFlag              = true;
                  }
                }
                else {
                  RtcCurrentValue[HOURS] = uint8_t(SmsTxt[ColonSymbol - 1] - '0');
                  RTC_MODULE.settime( RtcCurrentValue[SECONDS],  -1, RtcCurrentValue[HOURS],  -1, -1, -1, -1);
                              SetNewRtcHoursValueOkFlag                 = true;
                }
              }
              else            SetNewRtcHoursValueErrorFlag              = true;
              if (SmsTxt[ColonSymbol + 1] >= '0' && SmsTxt[ColonSymbol + 1] <= '9') {
                if (SmsTxt[ColonSymbol + 2] >= '0' && SmsTxt[ColonSymbol + 2] <= '9') {
                  if (SmsTxt[ColonSymbol + 1] >= '0' && SmsTxt[ColonSymbol + 1] <= '5') {
                    RtcCurrentValue[MINUTES] = uint8_t(SmsTxt[ColonSymbol + 1] - '0') * 10 + uint8_t(SmsTxt[ColonSymbol + 2] - '0');
                    RTC_MODULE.settime( RtcCurrentValue[SECONDS],  RtcCurrentValue[MINUTES], -1,  -1, -1, -1, -1);
                              SetNewRtcMinutesValueOkFlag               = true;
                  }
                  else        SetNewRtcMinutesValueErrorFlag            = true;
                }
                else {
                  RtcCurrentValue[MINUTES] = uint8_t(SmsTxt[ColonSymbol + 1] - '0');
                  RTC_MODULE.settime( RtcCurrentValue[SECONDS],  RtcCurrentValue[MINUTES], -1,  -1, -1, -1, -1);
                              SetNewRtcMinutesValueOkFlag               = true;
                }
              }
              else            SetNewRtcMinutesValueErrorFlag            = true;
              if (SetNewRtcHoursValueErrorFlag || SetNewRtcMinutesValueErrorFlag)
                                                            GSM_MODULE.SMSsend(F("Ошибка при изменении времени!"), Number);
              if (SetNewRtcHoursValueOkFlag && SetNewRtcMinutesValueOkFlag) {
                SmsTextAnswer_1 = String(F("Время успешно изменено на: ")) + RtcCurrentValue[HOURS] + ":" + RtcCurrentValue[MINUTES];
                                                            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number);
              }
            }
            else                                            GSM_MODULE.SMSsend(F("Ошибка при изменении времени RTC!"), Number);
          }


          else if (ColonSymbol == SMS_GET_FULL_STATE_INFO) {
            SmsTextAnswer_1 += String (F("Время:")) + RtcCurrentValue[HOURS] + ":" + RtcCurrentValue[MINUTES] + "\r\n";
            SmsTextAnswer_1 += String (F("Температура:")) + "\r\n" + F("Земли:") + RealGroundTemperature + "\r\n" + F("Воздуха:") + RealAirTemperature + "\r\n";
            SmsTextAnswer_2 += String (F("Влажность:")) + "\r\n" + F("Земли:") + RealGroundHumidityPercent + "\r\n" + F("Воздуха:") + RealAirHumidityPercent + "\r\n";
            SmsTextAnswer_2 += String (F("Освещённость:")) + RealLightingValue + "\r\n";

            switch (CurrentResumeOrBorderSettingValue[SELECTED_LIGHTING_RESUME]) {
              case LIGHT_OFF:         SmsTextAnswer_3 += String (F("LR: off(-)\r\n"));     break;
              case LIGHT_ON:          SmsTextAnswer_3 += String (F("LR: on(+)\r\n"));      break;
              case LIGHT_AUTO:        SmsTextAnswer_3 += String (F("LR: auto(A)\r\n"));    break;
            }

            SmsTextAnswer_3 += String(F("Граница включения света:")) + CurrentResumeOrBorderSettingValue[MIN_HUMIDITY] + "\r\n";

            switch (CurrentResumeOrBorderSettingValue[SELECTED_AIRING_ROOM_RESUME]) {
              case AIRING_ROOM_OFF:   SmsTextAnswer_3 += String (F("ARR: off(-)\r\n"));    break;
              case AIRING_ROOM_OPEN:  SmsTextAnswer_3 += String (F("ARR: open(O)\r\n"));   break;
              case AIRING_ROOM_CLOSE: SmsTextAnswer_3 += String (F("ARR: close(C)\r\n"));  break;
              case AIRING_ROOM_AUTO:  SmsTextAnswer_3 += String (F("ARR: auto(A)\r\n"));   break;
            }

            SmsTextAnswer_4 += String(F("Заданная MIN T:")) + CurrentResumeOrBorderSettingValue[MIN_TEMPERATURE_FOR_AIRING_ROOM] + F("\r\nЗаданная MAX T:") + CurrentResumeOrBorderSettingValue[MAX_TEMPERATURE_FOR_AIRING_ROOM] + "\r\n";

            switch (CurrentResumeOrBorderSettingValue[SELECTED_PUMP_WORK_RESUME]) {
              case PUMP_OFF:          SmsTextAnswer_4 += String (F("APR: off(-)\r\n"));    break;
              case PUMP_ON:           SmsTextAnswer_4 += String (F("APR: on(+)\r\n"));     break;
              case PUMP_AUTO:         SmsTextAnswer_4 += String (F("APR: auto(A)\r\n"));   break;
            }

            SmsTextAnswer_5 += String("Час включения насоса:") + CurrentResumeOrBorderSettingValue[TIME_FOR_START_PUMP] + "\r\nДлительность работы:" + CurrentResumeOrBorderSettingValue[DELAY_TIME_FOR_PUMP_WORKING] + "\r\n";

            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number, 0x1234, 5, 1); // Отправляем 1 часть составного (длинного) SMS
            GSM_MODULE.SMSsend(SmsTextAnswer_2, Number, 0x1234, 5, 2); // Отправляем 1 часть составного (длинного) SMS
            GSM_MODULE.SMSsend(SmsTextAnswer_3, Number, 0x1234, 5, 3); // Отправляем 1 часть составного (длинного) SMS
            GSM_MODULE.SMSsend(SmsTextAnswer_4, Number, 0x1234, 5, 4); // Отправляем 1 часть составного (длинного) SMS
            GSM_MODULE.SMSsend(SmsTextAnswer_5, Number, 0x1234, 5, 5); // Отправляем 1 часть составного (длинного) SMS
          }

          else if (ColonSymbol == SMS_GET_ALL_COMMAND_LIST) {

            SmsTextAnswer_1 = String(F("Режим света:\r\nLR -/+/A\r\nЗадать MIN освещённость:\r\nLMIN 0-1023\r"));
            SmsTextAnswer_2 = String(F("\nРежим проветривания:\r\nARR -/O/C/A\r\nЗадать MIN температуру:\r\nAR"));
            SmsTextAnswer_3 = String(F("MIN 0-49\r\nЗадать MAX температуру:\r\nARMAX 1-50\r\nРежим автополива:"));
            SmsTextAnswer_4 = String(F("\r\nAPR -/+/A\r\nЗадать час полива:\r\nAPTIME 0-23\r\nЗадать длительно"));
            SmsTextAnswer_5 = String(F("сть полива:\r\nAPDELAY 10-300\r\nЗадать время RTC:\r\nTIME 0-23:0-59\r"));
            SmsTextAnswer_6 = String(F("\nОтчёт о состоянии:\r\nINFO\r\nПолный список команд:\r\nHELP"));

            GSM_MODULE.SMSsend(SmsTextAnswer_1, Number, 0x1234, 6, 1);
            GSM_MODULE.SMSsend(SmsTextAnswer_2, Number, 0x1234, 6, 2);
            GSM_MODULE.SMSsend(SmsTextAnswer_3, Number, 0x1234, 6, 3);
            GSM_MODULE.SMSsend(SmsTextAnswer_4, Number, 0x1234, 6, 4);
            GSM_MODULE.SMSsend(SmsTextAnswer_5, Number, 0x1234, 6, 5);
            GSM_MODULE.SMSsend(SmsTextAnswer_6, Number, 0x1234, 6, 6);
          }
        }
      }
    }
  }
}

#endif
