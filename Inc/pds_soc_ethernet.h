#ifndef _PDS_SOC_ETHERNET_H_
#define _PDS_SOC_ETHERNET_H_
#include "stm32f4x7_eth.h"

#define KSZ8081_PHY_ADDRESS  	0x00				//KSZ8081 PHY芯片地址.

extern ETH_DMADESCTypeDef *DMARxDscrTab;			//以太网DMA接收描述符数据结构体指针
extern ETH_DMADESCTypeDef *DMATxDscrTab;			//以太网DMA发送描述符数据结构体指针 
extern U8_t *Rx_Buff; 							//以太网底层驱动接收buffers指针 
extern U8_t *Tx_Buff; 							//以太网底层驱动发送buffers指针
extern ETH_DMADESCTypeDef  *DMATxDescToSet;		//DMA发送描述符追踪指针
extern ETH_DMADESCTypeDef  *DMARxDescToGet; 		//DMA接收描述符追踪指针 
extern ETH_DMA_Rx_Frame_infos *DMA_RX_FRAME_infos;//DMA最后接收到的帧信息指针

BOOL_t PDS_soc_ethernet_controller_init(void);
FrameTypeDef PDS_soc_receive_ethernet_packet(void);
BOOL_t PDS_soc_send_ethernet_packet(U16_t len);
U32_t PDS_soc_get_current_tx_buf_addr(void);

BOOL_t PDS_soc_malloc_eth_memory(void);
void PDS_soc_free_eth_memory(void);

void eth_init(void);

#endif
