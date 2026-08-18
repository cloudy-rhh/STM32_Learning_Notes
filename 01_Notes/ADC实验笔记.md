ADC实验：

#1.单通道ADC采集实验：

	得自己添加新的文件（adc.h，adc.c文件，创建在main.c同级目录下，即User文件（应用层）)（期间得调用HAL库的函数，故得引用HAL库头文件）；
------------------------------------------
	adc文件内容：

	1.定义ADC初始化函数（adc_init函数），读取ADC值函数（adc_read函数）：

	提问：该怎么进行adc_init函数体的编程呢？

	步骤：

	1.定义一个adc结构体（全局变量）：ADC_HandleTypeDef g_adc_handle（结构体命名和之前实验规律一致）;

	2.定义一个GPIO结构体（局部变量）：GPIO_InitTypeDef gpio_initstruct;

  	3.调用时钟使能宏：__HAL_RCC_GPIOX_CLK_ENABLE() （X根据要调用按键的引脚组别来决定）；

	4.调用一个ADC使能宏：__HAL_RCC_ADCX_CLK_ENABLE()（X要根据使用的ADC模块号来决定，这里用1号模块）;

	5.配置引脚（配置步骤之前GPIO实验里有，模式改为GPIO_MODE_ANALOG（模拟输入），引脚号根据原理图中ADC通道判断）；

	6.配置adc结构体：g_adc_handle.Instance（确定ADC模块，一共3个模块，这里用1号模块，就写ADC1）;

	g_adc_handle.Init.ScanConvMode（确定是否扫描多通道，这里不扫描，使用DISABLE）；

	g_adc_handle.Init.ContinuousConvMode（确定是否连续转换，这里不转换，使用DISABLE）；

	g_adc_handle.Init.DiscontinuousConvMode（确定是否启用不连续模式，这里不启用，使用DISABLE）；

	g_adc_handle.Init.ExternalTrigConv（选择触发模式，这里选择软件触发，ADC_SOFTWARE_START）；

	g_adc_handle.Init.DataAlign（选择数据对齐模式，这里右对齐，ADC_DATAALIGN_RIGHT）；

	g_adc_handle.Init.NbrOfConversion（选择转换通道数，该实验是单通道，选择1）；

	7.将代码写入硬件：

	调用HAL_ADC_Init（详细用法编译器会提示，不做具体说明）；

	8.定义一个ADC通道结构体：ADC_ChannelConfTypeDef sConfig;

	9.配置通道结构体：sConfig.Channel（选择通道号，这里用通道1，即ADC_CHANNEL_1）；

	sConfig.Rank （选择序列顺序，单通道默认为1）；

	sConfig.SamplingTime（确定采样周期，采样周期为 55.5 个 ADC 时钟周期（采样时间越长，结果越稳定，但转换速度越慢），
	ADC_SAMPLETIME_55CYCLES_5）；

	10.将通道配置写入硬件：调用HAL_ADC_ConfigChannel（详细用法编译器会提示，不做具体说明）；

	提问：怎么进行adc_read函数体编程？

	uint16_t adc_read(void)

	{

  		  uint32_t adc_value = 0;	//定义一个变量，用来记录模拟值

   		 HAL_ADC_Start(&g_adc_handle); 	//使用软件触发，得调用这个函数

   		 if (HAL_ADC_PollForConversion(&g_adc_handle, 100) == HAL_OK)	//使用该函数等待转换完成，设置超时时间为100ms

  	 	 {

      	 		 adc_value = HAL_ADC_GetValue(&g_adc_handle);

   		 }

  		  HAL_ADC_Stop(&g_adc_handle);	//使用软件触发，调用该函数停止采集

  	 	 return (uint16_t)adc_value;

	}	
-------------------------------------------------
	main函数使用：

	1.初始化串口外设：调用usart_init函数；

	2.初始化ADC函数：调用adc_init函数；

	3.主循环逻辑修改：

	while(1)

   	 { 

		uint16_t adc_val = adc_read();	//记录模拟值

		float voltage = adc_val * 3.3f / 4095.0f;	//计算电压值
				
		printf("ADC: %d, Voltage: %.2f\r\n", adc_val, voltage);	//输出信息

		HAL_Delay(500);

   	 }

#2.单通道ADC采集实验（DMA读取实验）:

	得自己添加新的文件（adc.h，adc.c文件，创建在main.c同级目录下，即User文件（应用层）)（期间得调用HAL库的函数，故得引用HAL库头文件）；
---------------------------------------------------
	adc文件内容：

	1.定义ADC字符串长度宏ADC_BUFFER_SIZE（设置为1000较为合适）；
---------------------------------------------------
	2.定义ADC初始化函数（adc_init函数），读取ADC值函数（adc_read函数）(步骤和上一个实验基本一致，但得支持连续转换（ENABLE））：
---------------------------------------------------
	3.配置ADC通道（在adc_init函数中）（步骤和上一个实验基本一致，周期得改为239.5个时钟周期为采样周期（否则易导致CPU长时间处于中断状态，无法独立完成任务））；
---------------------------------------------------
	4.定义DMA初始化函数（adc_dma_init函数），ADC中断回调函数；
	
	提问：该怎么进行adc_dma_init函数体的编程呢？

	步骤：

	1.定义一个DMA结构体（全局变量）（形式和之前实验外设结构体差不多，不多解释）；

	2.调用一个DMA使能宏：__HAL_RCC_DMAX_CLK_ENABLE()（X要根据使用的DMA模块号来决定，这里用1号模块）;

	3.配置DMA结构体参数：g_dma_handle.Instance（选择DMA通道，这里用通道1（DMA1_Channel1）;

	g_dma_handle.Init.Direction（设置图片传输方向，这里选择从外设到内存，DMA_PERIPH_TO_MEMORY，另一种方向是 DMA_MEMORY_TO_PERIPH（内存到外	设），用于串口发送等场景），还有 DMA_MEMORY_TO_MEMORY（内存到内存），用于数组拷贝；

	g_dma_handle.Init.PeriphInc（设置外设地址，这里不递增，DMA_PINC_DISABLE，递增则用DMA_PINC_ENABLE）；

	g_dma_handle.Init.MemInc（设置内存地址，这里选择，即每次传输完成后内存地址向后移一个单位，DMA_MINC_ENABLE）；

	g_dma_handle.Init.PeriphDataAlignment（设置外设端数据的数据宽度，这里是DMA_PDATAALIGN_HALFWORD，HALFWORD 表示 16 位，WORD 表示 32 位，	BYTE 表示 8 位（1 个字节））；

	g_dma_handle.Init.MemDataAlignment（设置内存端数据的数据宽度，这里是DMA_MDATAALIGN_HALFWORD;，和上面一个参数同理，和外设端保持一致）；

	g_dma_handle.Init.Mode（设置DMA工作模式，这里设置为循环模式，DMA_CIRCULAR）；

	g_dma_handle.Init.Priority（设置DMA通道优先级，DMA_PRIORITY_HIGH）；

	4.将配置写入硬件：HAL_DMA_Init(&g_dma_handle)；

	5.设置中断优先级（NVIC，之前实验都有，改一下参数即可）；

	6.连接DMA与ADC：__HAL_LINKDMA(&g_adc_handle, DMA_Handle, g_dma_handle)；
	
	提问：该怎么进行ADC中断回调函数体的编程呢？

	void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)

	{

		if(hadc->Instance == ADC1)

		{

			uint32_t sum = 0; 	//用于记录采样值
				
			for(int i = 0; i < ADC_BUFFER_SIZE; i++)

			{

				sum += adc_buffer[i];  //循环累加所有采样值

			}

			uint16_t avg = sum / ADC_BUFFER_SIZE;  //记录平均采样值
				
			printf("First: %d, Avg: %d\r\n", adc_buffer[0], avg);	//打印采样值数据

		}

	}

	7.it文件中添加中断操作函数（之前的实验基本一致，改一下函数名称即可）；
------------------------------------------------------
	main函数使用：

	1.定义记录采样值的数组：uint16_t adc_buffer[ADC_BUFFER_SIZE]；

	2.初始化串口usart_init(115200);

	3.初始化ADC和DMA（调用函数，2个都得调用）；

	4.启动ADC和DMA：HAL_ADC_Start_DMA(&g_adc_handle, (uint32_t*)adc_buffer, ADC_BUFFER_SIZE)；

	（说明：中断是由DMA产生的，而非ADC，但是回调函数仍然写的是ADC，千万小心回调函数名不要写错）

#3.多通道ADC采集实验（DMA读取实验）：

	得自己添加新的文件（adc.h，adc.c文件，创建在main.c同级目录下，即User文件（应用层）)（期间得调用HAL库的函数，故得引用HAL库头文件）；
------------------------------------------------------
	该实验和单通道ADC采集实验区别极小，主要有以下注意点：

	1.要配置两个及以上引脚和通道，形成多通道（引脚配置，通道配置和之前一致）；

	2.ScanConvMode要改为ENABLE，支持多通道扫描；

	3.NbrOfConversion要改为检测通道数，改为多通道；

	4.通道的Rank决定DMA数据搬运顺序，不能一样，要有先后；

	5.中断回调函数要重写（交替搬运多个通道的数据）：

	void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)

	{

		if(hadc->Instance == ADC1)

		{

			uint32_t sum_ch0 = 0;

        	uint32_t sum_ch1 = 0;   //分别计算两个通道的总模拟值

        	for (int i = 0; i < ADC_BUFFER_SIZE; i += 2)  

       		{

           		 sum_ch0 += adc_buffer[i];     

           		 sum_ch1 += adc_buffer[i + 1];   //交替读取两个通道的数据

      		}

      		uint16_t avg_ch0 = sum_ch0 / (ADC_BUFFER_SIZE / 2);

       		uint16_t avg_ch1 = sum_ch1 / (ADC_BUFFER_SIZE / 2);  //分别计算两个通道的模拟值平均值

       		printf("CH0: %d, CH1: %d\r\n", avg_ch0, avg_ch1);  //打印采集数据信息

		}

	}

	其他步骤和单通道ADC采集实验（DMA读取实验）一致。

#4.单通道ADC过采样实验（16位分辨率）实验：

	该实验与单通道ADC采集实验区别也极小，主要有一下注意点：

	1.将ADC_BUFFER_SIZE改为2560；

	2.将采样周期改为1.5个ADC时钟周期；

	3.修改中断回调函数：

	void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)

	{

    	if (hadc->Instance == ADC1)

   		{

       		 uint32_t sum = 0;

       		for (int i = 0; i < ADC_BUFFER_SIZE; i++) 

			{

            	sum += adc_buffer[i];

        	}

            uint32_t avg = sum / ADC_BUFFER_SIZE;

       	    uint16_t result_16bit = avg << 4;  //采样值右移4位，得到16位采样值

	      	float voltage = result_16bit * 3.3f / 65535.0f;	//计算电压值

       		printf("16-bit: %d, Voltage: %.4fV\r\n", result_16bit, voltage);  //打印信息

   		 }

	}

	其他步骤和单通道ADC采集实验（DMA读取实验）一致。
