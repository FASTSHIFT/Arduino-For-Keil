GPIO 函数库
函数为 void TIM3_Int_Init(u16 arr,u16 psc) 其功能初始化定时器3的配置



定时时间为：t=(arr*psc)/72  (us)

使用时需更改配置：
				如需使用其他定时器，把各项的定时器项（TIM3）换成 TIM_x (应为通用定时器 TIM2-TIM7)
				