/**
  ******************************************************************************
  * @file    PDS_SocETHERNET.c
  * @author  WuYi
  * @brief   This file initialize ethernet peripheral of Stm32f429. And define the
  * routines of the ethernet interface.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "data_type.h"
#include "PDS_SocETHERNET.h"
#include "PDS_PeripheralKSZ8081.h"
#include "stm32f4x7_eth.h"
#include "stdio.h"
#include "GM_Malloc.h" 
/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
/* Macro -------------------------------------------------------------*/
/* Variables ---------------------------------------------------------*/
ETH_DMADESCTypeDef *DMARxDscrTab;	//以太网DMA接收描述符数据结构体指针
ETH_DMADESCTypeDef *DMATxDscrTab;	//以太网DMA发送描述符数据结构体指针 
U8_t *Rx_Buff; 					//以太网底层驱动接收buffers指针 
U8_t *Tx_Buff; 					//以太网底层驱动发送buffers指针
/* Function prototypes -----------------------------------------------*/
extern void lwip_packet_handler(void);		//在lwip_comm.c里面定义

BOOL_t PDS_soc_ethernet_controller_init(void)
{
	ETH_InitTypeDef eth_cfg; 

	REG_BIT_SET(RCC->AHB1ENR,25);	//ETHMACRXEN: Ethernet Reception clock
	REG_BIT_SET(RCC->AHB1ENR,26);	//ETHMACTXEN: Ethernet Transmission clock
	REG_BIT_SET(RCC->AHB1ENR,27);	//ETHMACEN: Ethernet MAC clock

	ETH_DeInit();  					//Reset the ethernet through AHB bus
	ETH_SoftwareReset();  			//Reset the ethernet through software

	/* Wait reset ready */
	while (ETH_GetSoftwareResetStatus() == SET);

	ETH_StructInit(&eth_cfg); 	 	//Initialize to the default configuration

	//Set MAC parameter
	eth_cfg.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;					//开启网络自适应功能
	eth_cfg.ETH_LoopbackMode = ETH_LoopbackMode_Disable;						//关闭反馈
	eth_cfg.ETH_RetryTransmission = ETH_RetryTransmission_Disable;				//关闭重传功能kp
	eth_cfg.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;		//关闭自动去除PDA/CRC功能 
	eth_cfg.ETH_ReceiveAll = ETH_ReceiveAll_Disable;							//关闭接收所有的帧
	eth_cfg.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;	//允许接收所有广播帧
	eth_cfg.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;					//关闭混合模式的地址过滤  
	eth_cfg.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;		//对于组播地址使用完美地址过滤   
	eth_cfg.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;			//对单播地址使用完美地址过滤 
#ifdef CHECKSUM_BY_HARDWARE
	eth_cfg.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable; 					//开启ipv4和TCP/UDP/ICMP的帧校验和卸载   
#endif
	//当我们使用帧校验和卸载功能的时候，一定要使能存储转发模式,存储转发模式中要保证整个帧存储在FIFO中,
	//这样MAC能插入/识别出帧校验值,当真校验正确的时候DMA就可以处理帧,否则就丢弃掉该帧
	eth_cfg.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; //开启丢弃TCP/IP错误帧
	eth_cfg.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;     //开启接收数据的存储转发模式    
	eth_cfg.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;   //开启发送数据的存储转发模式  

	eth_cfg.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;     	//禁止转发错误帧  
	eth_cfg.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;	//不转发过小的好帧 
	eth_cfg.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;  		//打开处理第二帧功能
	eth_cfg.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;  	//开启DMA传输的地址对齐功能
	eth_cfg.ETH_FixedBurst = ETH_FixedBurst_Enable;            			//开启固定突发功能    
	eth_cfg.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;     		//DMA发送的最大突发长度为32个节拍   
	eth_cfg.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;			//DMA接收的最大突发长度为32个节拍
	eth_cfg.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;
	eth_cfg.Sys_Clock_Freq=192;//系统时钟频率为192Mhz

	/* Configure the ethernet */
	if (ETH_Init(&eth_cfg,KSZ8081_PHY_ADDRESS) == ETH_SUCCESS)
	{
		ETH_DMAITConfig(ETH_DMA_IT_NIS|ETH_DMA_IT_R,ENABLE);//使能以太网接收中断
		return BOOL_TRUE;	
	}

	return BOOL_FALSE;
}



FrameTypeDef PDS_soc_receive_ethernet_packet(void)
{ 
	U32_t framelength=0;
	FrameTypeDef frame={0,0};   
	//检查当前描述符,是否属于ETHERNET DMA(设置的时候)/CPU(复位的时候)
	if((DMARxDescToGet->Status&ETH_DMARxDesc_OWN)!=(U32_t)RESET)
	{	
		frame.length=ETH_ERROR; 
		if ((ETH->DMASR&ETH_DMASR_RBUS)!=(U32_t)RESET)  
		{ 
			ETH->DMASR = ETH_DMASR_RBUS;//清除ETH DMA的RBUS位 
			ETH->DMARPDR=0;//恢复DMA接收
		}
		return frame;//错误,OWN位被设置了
	}  
	if(((DMARxDescToGet->Status&ETH_DMARxDesc_ES)==(U32_t)RESET)&& 
	((DMARxDescToGet->Status & ETH_DMARxDesc_LS)!=(U32_t)RESET)&&  
	((DMARxDescToGet->Status & ETH_DMARxDesc_FS)!=(U32_t)RESET))  
	{       
		framelength=((DMARxDescToGet->Status&ETH_DMARxDesc_FL)>>ETH_DMARxDesc_FrameLengthShift)-4;//得到接收包帧长度(不包含4字节CRC)
 		frame.buffer = DMARxDescToGet->Buffer1Addr;//得到包数据所在的位置
	}else framelength=ETH_ERROR;//错误  
	frame.length=framelength; 
	frame.descriptor=DMARxDescToGet;  
	//更新ETH DMA全局Rx描述符为下一个Rx描述符
	//为下一次buffer读取设置下一个DMA Rx描述符
	DMARxDescToGet=(ETH_DMADESCTypeDef*)(DMARxDescToGet->Buffer2NextDescAddr);   
	return frame;  
}

BOOL_t PDS_soc_send_ethernet_packet(U16_t len)
{   
	//检查当前描述符,是否属于ETHERNET DMA(设置的时候)/CPU(复位的时候)

	if((DMATxDescToSet->Status&ETH_DMATxDesc_OWN)!=(U32_t)RESET)
	{
		return BOOL_FALSE;//ETH_ERROR;//错误,OWN位被设置了
	}

 	DMATxDescToSet->ControlBufferSize=(len&ETH_DMATxDesc_TBS1);//设置帧长度,bits[12:0]
	DMATxDescToSet->Status|=ETH_DMATxDesc_LS|ETH_DMATxDesc_FS;//设置最后一个和第一个位段置位(1个描述符传输一帧)
  	DMATxDescToSet->Status|=ETH_DMATxDesc_OWN;//设置Tx描述符的OWN位,buffer重归ETH DMA

	if((ETH->DMASR&ETH_DMASR_TBUS)!=(U32_t)RESET)//当Tx Buffer不可用位(TBUS)被设置的时候,重置它.恢复传输
	{ 
		ETH->DMASR=ETH_DMASR_TBUS;//重置ETH DMA TBUS位 
		ETH->DMATPDR=0;//恢复DMA发送
	}

	//更新ETH DMA全局Tx描述符为下一个Tx描述符
	//为下一次buffer发送设置下一个DMA Tx描述符 
	DMATxDescToSet=(ETH_DMADESCTypeDef*)(DMATxDescToSet->Buffer2NextDescAddr);    
	return BOOL_TRUE;//ETH_SUCCESS;
}

U32_t PDS_soc_get_current_tx_buf_addr(void)
{  
	return DMATxDescToSet->Buffer1Addr;
}

/* Alloc the ethernet memory */
BOOL_t PDS_soc_malloc_eth_memory(void)
{
	DMARxDscrTab=GM_malloc(INTERAL_GEN_MEM,ETH_RXBUFNB*sizeof(ETH_DMADESCTypeDef));//申请内存
	DMATxDscrTab=GM_malloc(INTERAL_GEN_MEM,ETH_TXBUFNB*sizeof(ETH_DMADESCTypeDef));//申请内存  
	Rx_Buff=GM_malloc(INTERAL_GEN_MEM,ETH_RX_BUF_SIZE*ETH_RXBUFNB);	//申请内存
	Tx_Buff=GM_malloc(INTERAL_GEN_MEM,ETH_TX_BUF_SIZE*ETH_TXBUFNB);	//申请内存
	if(!DMARxDscrTab||!DMATxDscrTab||!Rx_Buff||!Tx_Buff)
	{
		PDS_soc_free_eth_memory();

		return BOOL_FALSE;	//申请失败
	}

	return BOOL_TRUE;		//申请成功
}

/* Free the ethernet memory */
void PDS_soc_free_eth_memory(void)
{ 
	GM_free(INTERAL_GEN_MEM,DMARxDscrTab);//释放内存
	GM_free(INTERAL_GEN_MEM,DMATxDscrTab);//释放内存
	GM_free(INTERAL_GEN_MEM,Rx_Buff);		//释放内存
	GM_free(INTERAL_GEN_MEM,Tx_Buff);		//释放内存  
}

/*
void eth_init(void)
{
	PDS_peripheral_ksz8081_init();

	REG_BIT_SET(RCC->AHB1ENR,25);	//ETHMACRXEN: Ethernet Reception clock
	REG_BIT_SET(RCC->AHB1ENR,26);	//ETHMACTXEN: Ethernet Transmission clock
	REG_BIT_SET(RCC->AHB1ENR,27);	//ETHMACEN: Ethernet MAC clock
}
*/

//ETH_WritePHYRegister(PHYAddress, PHY_BCR, PHY_Reset);
