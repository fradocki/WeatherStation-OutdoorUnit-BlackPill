/*
 * fr_stm32.c
 *      Author: Filip
 */
#include "fr_stm32.h"


// Function to put the device in standby mode
void sleep(){
    // backup WiFi mode state
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR18, tryb_wifi);

    // disable wake up pin
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);

    // deactivate wake up timer
    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

    // deactivate RTC alarm
    HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

    // clear power wake up flag
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    // clear RTC alarm flag
    __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);

    // get current time
    pobierz_czas();

    // set the next alarm
    SetNextAlarm();

    // enter standby mode
    HAL_PWR_EnterSTANDBYMode();
}

// function to get the current time
void pobierz_czas(){

    // get current time
    HAL_RTC_GetTime(&hrtc, &time_get, RTC_FORMAT_BIN);

    // get current date
    HAL_RTC_GetDate(&hrtc, &data_get, RTC_FORMAT_BIN);
}

// function to set the next alarm
void SetNextAlarm(void)
{
    RTC_AlarmTypeDef sAlarm = {0}; // RTC alarm structure to store the next alarm setting

    /* if the current time is 23:59, set the alarm hour to 0 (midnight)
       Otherwise, set the alarm hour to the current hour plus 1 */
    if((time_get.Hours + ((time_get.Minutes + 1) / 60)) == 24){
        sAlarm.AlarmTime.Hours = 0;
    }
    else{
        sAlarm.AlarmTime.Hours = time_get.Hours + ((time_get.Minutes + 1) / 60);
    }

    // set the alarm minute to the current minute plus 1, wrapping around to 0 if it exceeds 59
    sAlarm.AlarmTime.Minutes = (time_get.Minutes + 1) % 60;

    // set the alarm seconds to the current seconds
    sAlarm.AlarmTime.Seconds = time_get.Seconds;

    // set the sub-seconds, daylight saving, store operation, alarm mask, sub-second mask, weekday select, date, and alarm A
    sAlarm.AlarmTime.SubSeconds = 0;
    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    sAlarm.AlarmDateWeekDay = data_get.Date;
    sAlarm.Alarm = RTC_ALARM_A;

    // set the alarm
    // if setting the alarm fails, call the error handler
    if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
}

