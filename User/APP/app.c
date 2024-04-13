/*
 * @Author: DuRuofu duruofu@qq.com
 * @Date: 2023-08-02 12-02-17
 * @LastEditors: DuRuofu
 * @LastEditTime: 2024-04-13 18-57-41
 * @FilePath: \STM32(simple)\User\APP\app.c
 * @Description: 主程序入口，包含主循环和中断回调函数，以及一些全局变量的定义
 * Copyright (c) 2023 by duruofu@foxmail.com All Rights Reserved.
 */

#include "app.h"
#include "Buzzer.h"
#include "string.h"
#include <math.h>

//--------------------------舵机参考系---------------------
// 中心位置处参数
extern uint16_t Centre_A = 1460;
extern uint16_t Centre_B = 1417;
// 舵机A（最值）
extern uint16_t Servo_MAX_A = 2000;
extern uint16_t Servo_MIN_A = 1000;
// 舵机B （最值）
extern uint16_t Servo_MIN_B = 1000;
extern uint16_t Servo_MAX_B = 2000;

// 实时位置
extern uint16_t pwm_A;
extern uint16_t pwm_B;

/**
 * @description: 系统应用初始化
 * @return {*}
 */
void App_Init(void)
{
    // OLED_Init();          // 0.96oled初始化
    // OLED_Clear();
    USART_IT_Config(); // 总串口接收初始化
    PWM_Init();
    Yuntai_Control();              // 云台初始化
    HAL_TIM_Base_Start_IT(&htim4); // 启动定时器4
}

/**
 * @description: 系统应用循环任务
 * @return {*}
 */
int App_Task(void)
{
    // 参数限幅
    if (pwm_A > 1613)
        pwm_A = Servo_MAX_A;
    if (pwm_A < Servo_MIN_A)
        pwm_A = Servo_MIN_A;
    if (pwm_B > Servo_MAX_B)
        pwm_B = Servo_MAX_B;
    if (pwm_B < Servo_MIN_B)
        pwm_B = Servo_MIN_B;

    // 控制
    Yuntaiz_AB_Move(pwm_A, pwm_B, 1);
}

// 定时器中断回调函数(1ms一次)
uint8_t LED_Heartbeat = 0; // LED心跳
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim4) // 判断中断是否来自于定时器4
    {
        // 心跳(50ms一次)
        LED_Heartbeat++;
        if (LED_Heartbeat == 30)
        {
            LED_Toggle(1);
        }
    }
}
