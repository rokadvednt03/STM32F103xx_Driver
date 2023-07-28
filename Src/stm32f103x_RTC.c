#include "stm32f103x_RTC.h"

void RTC_Init(void){
	uint16_t presclr;
	RCC->APB1ENR|=RCC_APB1ENR_PWREN;
	RCC->APB1ENR|=RCC_APB1ENR_BKPEN;
	
	PWR->CR|=PWR_CR_DBP;
	
	
	RCC->BDCR|=RCC_BDCR_LSEON;
	
	while (!RCC->BDCR>>RCC_BDCR_LSERDY);
	RCC->BDCR|=0x8100;
	
	while(!RTC->CRL>>RTC_CRL_RSF);
	RCC->BDCR&=~RCC_BDCR_BDRST;
	
		
  
	RTC->CRL|=(1<<4);
	while(!RTC->CRL>>5);
	RTC->PRLL=0x7FFF;
	RTC->CRL&=~(1<<4);
	while(!RTC->CRL>>5);
	presclr=RTC->PRLL;
	//PWR->CR&=~PWR_CR_DBP;


	
}

p_sec=0;   //((RTC->CNTH<<16)+(RTC->CNTL));
uint32_t c_sec=0;
void RTC_GetTime(RTC_Time *time){
	uint16_t counter_low=RTC->CNTL;
	uint16_t counter_high=RTC->CNTH;
	uint32_t counter=(counter_high<<16)+counter_low;
	c_sec=counter;
	uint8_t sec=c_sec-p_sec;
	if( (time->seconds+sec)>=60){
		if(time->minutes>=60){
			if(time->hours>=23){
				time->hours=0;
				time->minutes=0;
				time->seconds=0;
				p_sec=c_sec;
			}
			else{
				time->hours++;
				time->minutes=0;
				time->seconds=0;
				p_sec=c_sec;
			}
		}
		else if((time->minutes)<60){
			time->minutes++;
			time->seconds=0;
			 p_sec=c_sec;
		}
	}
	else if(time->seconds<60){
		time->seconds+=sec;
		p_sec=c_sec;
	}
	
	
	/*
	time->hours=ctr_val/0xE10;
	time->minutes=(ctr_val%0xE10)/0x3C;
	time->seconds=(ctr_val%0xE10)%0x3C;*/
}

static uint32_t p_days=0;//((RTC->CNTH<<16)+(RTC->CNTL))/86400;
uint32_t c_days=0;


void RTC_GetDate(RTC_Date *date){
	uint16_t counter_low=RTC->CNTL;
	uint16_t counter_high=RTC->CNTH;
	uint32_t counter=(counter_high<<16)+counter_low;
	c_days=counter/86400;
	uint8_t days=c_days-p_days;
	/////////////// IF MONTH IS OF 31 DAYS ///////////////////////////
	if ( (date->month%2 & date->month<=7) || (~date->month%2 & date->month>7) & (date->month!=2) ){
		if((date->day+c_days)==31){
			if(date->month==12){
				date->year++;
				date->month=1;
				date->day=1;
				p_days=c_days;
			}
			else{
				date->day+=days;
				p_days=c_days;
			}
		}
		else if((date->day+days)<31){
			date->day+=days;
			p_days=c_days;
		}
	}
	/////////////// IF MONTH IS OF 30 DAYS ////////////////////////////
	else if(date->month!=2){
		if((date->day+days)==30){
			date->month++;
			date->day=1;
			p_days=c_days;
		}
		else if((date->day+days)<30){
			date->day+=days;
			p_days=c_days;
		}
	}
	//////////////// IF MONTH IS FEBRUARY /////////////////////////////
	else{
		if(Leap_Year(date->year)){
			if((date->day+days)==29){
				date->month++;
				date->day=1;
				p_days=c_days;
			}
			else if(date->day+days){
				date->day+=days;
				p_days=c_days;
			}
		}
		else{
			if((date->day+days)){
				date->month++;
				date->day=1;
				p_days=c_days;
			}
			else{
				date->day+=days;
				p_days=c_days;
			}
		}
	}
}



uint8_t Leap_Year(uint32_t year){
	if (year%400)
		return 1;
	else{
		if(year%100)
				return 0;
		else{
			if(year%4)
					return 1;
			else 
				return 0;
		}
	}
}

void RTC_SetAlarm(uint8_t hour,uint8_t minutes,uint8_t seconds){
	uint16_t alarm;
	uint16_t counter=(hour*3600)+(minutes*60)+seconds;
	RTC->CRH|=(1<<1);
	RTC->CRL|=(1<<4);
	while(!RTC->CRL>>5);
	RTC->ALRL=counter;
	RTC->CRL&=~(1<<4);
	while(!RTC->CRL>>5);
	alarm=RTC->ALRL;

}


	
