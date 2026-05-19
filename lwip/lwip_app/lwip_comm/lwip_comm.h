#ifndef _LWIP_COMM_H
#define _LWIP_COMM_H
#include "data_type.h" 

#define LWIP_MAX_DHCP_TRIES		4   //DHCP������������Դ���
   
#define LWIP_TCP_SERVERDEMO 0X80  	//TCP����������
#define LWIP_TCP_CLIENTDEMO 0X40  	//TCP�ͻ��˹���
#define LWIP_UDP_DEMO 		0X20  	//UDP����������

//lwip���ƽṹ��
typedef struct  
{
	U8_t mac[6];      //MAC��ַ
	U8_t remoteip[4];	//Զ������IP��ַ 
	U8_t ip[4];       //����IP��ַ
	U8_t netmask[4]; 	//��������
	U8_t gateway[4]; 	//Ĭ�����ص�IP��ַ
	
	VU8_t dhcpstatus;	//dhcp״̬ 
					//0,δ��ȡDHCP��ַ;
					//1,����DHCP��ȡ״̬
					//2,�ɹ���ȡDHCP��ַ
					//0XFF,��ȡʧ��.
}__lwip_dev;
extern __lwip_dev lwipdev;	//lwip���ƽṹ��
 
void lwip_packet_handler(void);
void lwip_periodic_handle(void); 
void lwip_comm_default_ip_set(__lwip_dev *lwipx);
BOOL_t lwip_comm_mem_malloc(void);
void lwip_comm_mem_free(void);
BOOL_t lwip_comm_init(void);
void lwip_dhcp_process_handle(void);

#endif













