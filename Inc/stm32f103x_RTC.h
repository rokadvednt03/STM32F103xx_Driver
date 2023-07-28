#include "stm32f10x.h"
#include <stdint.h>


/*Structure for getting time in HH:MM:SS format*/
typedef struct{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
}RTC_Time;

/*Structure for getting time in HH:MM:SS format*/
typedef struct{
	uint8_t year;
	uint8_t month;
	uint8_t day;
}RTC_Date;

/* APIS */
void RTC_Init(void);
void RTC_GetTime(RTC_Time *time);
void RTC_GetDate(RTC_Date *date);
uint8_t Leap_Year(uint32_t year);
void RTC_SetAlarm(uint8_t hour,uint8_t minutes,uint8_t seconds);
