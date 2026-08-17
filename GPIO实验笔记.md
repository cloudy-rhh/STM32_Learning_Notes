#1.跑马灯实验（GPIO输出实验）

	一般使用HAL外设函数库（在HAL文件里的stm32f1xx_hal_gpio.c里）来实现，直接操作硬件（如函数HAL_GPIO_WritePin来改变LED灯的电平状态，来改变灯的亮灭）；

	函数用法：HAL_GPIO_WritePin(GPIOX(X为要操作的引脚组别), GPIO_PIN_X(X为引脚标号), 电平状态);

	（注：GPIO_PIN_SET为高电平，GPIO_PIN_RESET为低电平）;

	要注意使用延时函数（delay_ms，在SYSTEM文件中的delay.c中有定义）。

#2.按键输入实验（GPIO输入实验，轮询方式）

	得自己创建新的文件（key.h，key.c文件，创建在main.c同级目录下，即User文件（应用层））（期间得调用HAL库的函数，故得引用HAL库头文件）；

	##key文件内容：

	1.定义引脚的宏（如KEY0_PIN, KEY0_PORT）（PIN是引脚序号，PORT是引脚组别）（引脚注意查找原理图）；

	2.定义key的初始化函数（key_init函数），按键电平判断函数（如key0_scan函数）；

	提问：该怎么进行key_init函数体的编程呢？

	步骤：1.定义一个GPIO结构体：GPIO_InitTypeDef gpio_initstruct（GPIO_InitTypeDef是一个结构体类型，包含GPIO配置参数）;

		  2.调用时钟使能宏：__HAL_RCC_GPIOX_CLK_ENABLE() （X根据要调用按键的引脚组别来决定）；

		  3.引脚结构体成员赋值：gpio_initstruct.Pin，确定引脚参数；

						   gpio_initstruct.Mode，确定输入输出模式参数（输入用GPIO_MODE_INPUT）；

						   gpio_initstruct.Pull，确定输入模式类型参数（上拉输入用GPIO_PULLUP，下拉输入用GPIO_PULLDOWN）；

						   gpio_initstruct.Speed，输出速度参数（一般意义不大，写GPIO_SPEED_FREQ_LOW）；

		   4.调用初始化执行函数（HAL库的函数）：

		   函数用法：HAL_GPIO_Init（引脚的宏,  对应GPIO结构体的地址），将配置直接写入硬件寄存器。

	##提问：该怎么进行按键电平判断函数的编程呢？

	步骤：1.调用HAL_GPIO_ReadPin（读取按键电平状态）

		   函数用法：HAL_GPIO_ReadPin(引脚对应组别，引脚对应序号);

		   2.进行判断（条件判断if，根据情况设置低电平返回什么值，高电平返回什么值）。

	##main函数使用：

	调用HAL_GPIO_TogglePin（翻转电平状态）；

	函数用法：HAL_GPIO_TogglePin(引脚组别PORT，引脚序号PIN)；

	根据电平状态来判断是否改变LED灯的状态，实现按键输入功能。

#3.外部中断实验（GPIO输入实验，中断方式）

	得自己创建新的文件（exti.h,，exti.c文件，创建在main.c同级目录下，即User文件（应用层））（期间得调用HAL库的函数，故得引用HAL库头文件）；

	##exti文件内容：

	1.定义外部中断初始化函数exti_init；
	
	##提问：该怎么进行exti_init函数体的编程呢？

	步骤：1.定义一个GPIO结构体：GPIO_InitTypeDef gpio_initstruct（GPIO_InitTypeDef是一个结构体类型，包含GPIO配置参数）;

		  2.调用时钟使能宏：__HAL_RCC_GPIOX_CLK_ENABLE() （X根据要调用按键的引脚组别来决定）；

		  3.调用 SYSCFG 时钟：__HAL_RCC_SYSCFG_CLK_ENABLE()（外部中断的引脚映射需要 SYSCFG 模块来配合，因此必须启用这个时钟）；

		  4.引脚结构体成员赋值：gpio_initstruct.Pin，确定引脚参数；

						   gpio_initstruct.Mode，确定中断模式参数（输入用GPIO_MODE_IT_FALLING）（it代表interrupt，表示中断模式）；

						   gpio_initstruct.Pull，确定输入模式类型参数（上拉输入用GPIO_PULLUP，下拉输入用GPIO_PULLDOWN）；

						   gpio_initstruct.Speed，输出速度参数（一般意义不大，写GPIO_SPEED_FREQ_LOW）；

		  5.调用初始化执行函数（HAL库的函数）：

		   函数用法：HAL_GPIO_Init（引脚的宏,  对应GPIO结构体的地址），将配置直接写入硬件寄存器。

		  6.调用函数HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0)（设置中断优先级）；

		   函数用法：HAL_NVIC_SetPriority（外部中断线中断号，抢占优先级，响应优先级）；

		  7.调用函数HAL_NVIC_EnableIRQ(EXTI2_IRQn)（使CPU执行中断函数，开关使能）；

		   函数用法：HAL_NVIC_EnableIRQ（外部中断线中断号）。

	 说明：1.NVIC是嵌套向量中断控制器，负责告诉CPU中断优先级；

		   2.抢占优先级高的可以打断优先级低的中断，抢占优先级一致的，CPU优先执行响应优先级高的中断（响应优先级为子优先级）；

		   3.数字越小，中断优先级越高；

		   4.EXTI为外部中断线，其它类型的有所不同；

		   5.编号为 X 的中断线，对应所有后缀为 X 的引脚（如PAX，PBX，PCX......）。

	  2.定义中断回调函数HAL_GPIO_EXTI_Callback；

	   ##提问：该怎么实现HAL_GPIO_EXTI_Callback函数体的编程呢？

	   步骤：1.在it文件中写一个EXTIX_IRQHandler函数（X根据硬件引脚命名，CPU会跳转使用），函数体里调用HAL_GPIO_EXTI_IRQHandler（GPIO_PIN_X)(HAL库函数）；

		     2.重写callback函数，写业务逻辑。

	   说明：1.硬件被动响应，CPU会直接跳转到EXTIX_IRQHandler函数（在it文件里）；

		     2.EXTIX_IRQHandler函数中会调用HAL_GPIO_EXTI_IRQHandler（GPIO_PIN_X)；

		     3.HAL_GPIO_EXTI_IRQHandler 检查并清除中断标志位，HAL_GPIO_EXTI_IRQHandler 调用 HAL_GPIO_EXTI_Callback（这个函数在HAL_GPIO文件中）；

		     4.最后执行自己编辑的业务逻辑，完成功能实现。

	##main函数使用：

	由于外部中断函数是硬件被动响应，因此主函数体中无需写多余代码，在初始化阶段（循环外）直接调用exti_init即可。
	