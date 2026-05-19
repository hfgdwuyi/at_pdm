#include "tcp_server_demo.h" 
#include "delay.h"
#include "usart.h"
#include "led.h"
//#include "key.h"
#include "GM_Malloc.h"
#include "stdio.h"
#include "string.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32������
//TCP Server ���Դ���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/25
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 
//TCP Server�������ݻ�����
u8 tcp_server_recvbuf[TCP_SERVER_RX_BUFSIZE];	
//TCP������������������
const u8 *tcp_server_sendbuf="Apollo STM32F4/F7 TCP Server send data\r\n";

//TCP Server ����ȫ��״̬��Ǳ���
//bit7:0,û������Ҫ����;1,������Ҫ����
//bit6:0,û���յ�����;1,�յ�������.
//bit5:0,û�пͻ���������;1,�пͻ�����������.
//bit4~0:����
u8 tcp_server_flag;	 
 
 
//TCP Server ����
void tcp_server_test(void)
{
	err_t err;  
	struct tcp_pcb *tcppcbnew;  	//����һ��TCP���������ƿ�
	struct tcp_pcb *tcppcbconn;  	//����һ��TCP���������ƿ�
	
	u8 *tbuf;
 	//u8 key;
	u8 res=0;		
	u8 t=0; 
	u8 connflag=0;		//���ӱ��

	tbuf=GM_malloc(INTERAL_GEN_MEM,200);	//�����ڴ�
	if(tbuf==NULL)return ;		//�ڴ�����ʧ����,ֱ���˳�
	// sprintf((char*)tbuf,"Server IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);//������IP

	// sprintf((char*)tbuf,"Server Port:%d",TCP_SERVER_PORT);//�������˿ں�

	tcppcbnew=tcp_new();	//����һ���µ�pcb
	if(tcppcbnew)			//�����ɹ�
	{ 
		err=tcp_bind(tcppcbnew,IP_ADDR_ANY,TCP_SERVER_PORT);	//������IP��ָ���Ķ˿ںŰ���һ��,IP_ADDR_ANYΪ�󶨱������е�IP��ַ
		if(err==ERR_OK)	//�����
		{
			tcppcbconn=tcp_listen(tcppcbnew); 			//����tcppcb�������״̬
			tcp_accept(tcppcbconn,tcp_server_accept); 	//��ʼ��LWIP��tcp_accept�Ļص�����
		}else res=1;  
	}else res=1;

	while(res==0)
	{
		//key=KEY_Scan(0);
		/*if(key==WKUP_PRES)break;
		if(key==KEY0_PRES)//KEY0������,��������
		{
			tcp_server_flag|=1<<7;//���Ҫ��������
		}*/
		if(tcp_server_flag&1<<6)//�Ƿ��յ�����?
		{
			tcp_server_flag&=~(1<<6);//��������Ѿ���������.
		}
		if(tcp_server_flag&1<<5)//�Ƿ�������?
		{
			if(connflag==0)
			{ 
				// sprintf((char*)tbuf,"Client IP:%d.%d.%d.%d",lwipdev.remoteip[0],lwipdev.remoteip[1],lwipdev.remoteip[2],lwipdev.remoteip[3]);//�ͻ���IP
				connflag=1;//���������
			} 
		}else if(connflag)
		{
			connflag=0;	//������ӶϿ���
		}
		lwip_periodic_handle();
		delay_ms(2);
		t++;
		if(t==200)
		{
			t=0;
			LED0=!LED0;
		} 
	}   
	tcp_server_connection_close(tcppcbnew,0);//�ر�TCP Server����
	tcp_server_connection_close(tcppcbconn,0);//�ر�TCP Server���� 
	tcp_server_remove_timewait(); 
	memset(tcppcbnew,0,sizeof(struct tcp_pcb));
	memset(tcppcbconn,0,sizeof(struct tcp_pcb)); 
	GM_free(INTERAL_GEN_MEM,tbuf);
} 
//lwIP tcp_accept()�Ļص�����
err_t tcp_server_accept(void *arg,struct tcp_pcb *newpcb,err_t err)
{
	err_t ret_err;
	struct tcp_server_struct *es; 
 	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);
	tcp_setprio(newpcb,TCP_PRIO_MIN);//�����´�����pcb���ȼ�
	es=(struct tcp_server_struct*)mem_malloc(sizeof(struct tcp_server_struct)); //�����ڴ�
 	if(es!=NULL) //�ڴ����ɹ�
	{
		es->state=ES_TCPSERVER_ACCEPTED;  	//��������
		es->pcb=newpcb;
		es->p=NULL;
		
		tcp_arg(newpcb,es);
		tcp_recv(newpcb,tcp_server_recv);	//��ʼ��tcp_recv()�Ļص�����
		tcp_err(newpcb,tcp_server_error); 	//��ʼ��tcp_err()�ص�����
		tcp_poll(newpcb,tcp_server_poll,1);	//��ʼ��tcp_poll�ص�����
		tcp_sent(newpcb,tcp_server_sent);  	//��ʼ�����ͻص�����
		  
		tcp_server_flag|=1<<5;				//����пͻ���������
		lwipdev.remoteip[0]=newpcb->remote_ip.addr&0xff; 		//IADDR4
		lwipdev.remoteip[1]=(newpcb->remote_ip.addr>>8)&0xff;  	//IADDR3
		lwipdev.remoteip[2]=(newpcb->remote_ip.addr>>16)&0xff; 	//IADDR2
		lwipdev.remoteip[3]=(newpcb->remote_ip.addr>>24)&0xff; 	//IADDR1 
		ret_err=ERR_OK;
	}else ret_err=ERR_MEM;
	return ret_err;
}
//lwIP tcp_recv()�����Ļص�����
err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	err_t ret_err;
	u32 data_len = 0;
	struct pbuf *q;
  	struct tcp_server_struct *es;
	LWIP_ASSERT("arg != NULL",arg != NULL);
	es=(struct tcp_server_struct *)arg;
	if(p==NULL) //�ӿͻ��˽��յ�������
	{
		es->state=ES_TCPSERVER_CLOSING;//��Ҫ�ر�TCP ������
		es->p=p; 
		ret_err=ERR_OK;
	}else if(err!=ERR_OK)	//�ӿͻ��˽��յ�һ���ǿ�����,��������ĳ��ԭ��err!=ERR_OK
	{
		if(p)pbuf_free(p);	//�ͷŽ���pbuf
		ret_err=err;
	}else if(es->state==ES_TCPSERVER_ACCEPTED) 	//��������״̬
	{
		if(p!=NULL)  //����������״̬���ҽ��յ������ݲ�Ϊ��ʱ�����ӡ����
		{
			memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //���ݽ��ջ���������
			for(q=p;q!=NULL;q=q->next)  //����������pbuf����
			{
				//�ж�Ҫ������TCP_SERVER_RX_BUFSIZE�е������Ƿ����TCP_SERVER_RX_BUFSIZE��ʣ��ռ䣬�������
				//�Ļ���ֻ����TCP_SERVER_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
				if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) memcpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//��������
				else memcpy(tcp_server_recvbuf+data_len,q->payload,q->len);
				data_len += q->len;  	
				if(data_len > TCP_SERVER_RX_BUFSIZE) break; //����TCP�ͻ��˽�������,����	
			}
			tcp_server_flag|=1<<6;	//��ǽ��յ�������
			lwipdev.remoteip[0]=tpcb->remote_ip.addr&0xff; 		//IADDR4
			lwipdev.remoteip[1]=(tpcb->remote_ip.addr>>8)&0xff; //IADDR3
			lwipdev.remoteip[2]=(tpcb->remote_ip.addr>>16)&0xff;//IADDR2
			lwipdev.remoteip[3]=(tpcb->remote_ip.addr>>24)&0xff;//IADDR1 
 			tcp_recved(tpcb,p->tot_len);//���ڻ�ȡ��������,֪ͨLWIP���Ի�ȡ��������
			pbuf_free(p);  	//�ͷ��ڴ�
			ret_err=ERR_OK;
		}
	}else//�������ر���
	{
		tcp_recved(tpcb,p->tot_len);//���ڻ�ȡ��������,֪ͨLWIP���Ի�ȡ��������
		es->p=NULL;
		pbuf_free(p); //�ͷ��ڴ�
		ret_err=ERR_OK;
	}
	return ret_err;
}
//lwIP tcp_err�����Ļص�����
void tcp_server_error(void *arg,err_t err)
{  
	LWIP_UNUSED_ARG(err);  
	printf("tcp error:%x\r\n",(u32)arg);
	if(arg!=NULL)mem_free(arg);//�ͷ��ڴ�
} 
//lwIP tcp_poll�Ļص�����
err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	struct tcp_server_struct *es; 
	es=(struct tcp_server_struct *)arg; 
	if(es!=NULL)
	{
		if(tcp_server_flag&(1<<7))	//�ж��Ƿ�������Ҫ����
		{
			es->p=pbuf_alloc(PBUF_TRANSPORT,strlen((char*)tcp_server_sendbuf),PBUF_POOL);//�����ڴ�
			pbuf_take(es->p,(char*)tcp_server_sendbuf,strlen((char*)tcp_server_sendbuf));
			tcp_server_senddata(tpcb,es); 		//��ѯ��ʱ����Ҫ���͵�����
			tcp_server_flag&=~(1<<7);  			//������ݷ��ͱ�־λ
			if(es->p!=NULL)pbuf_free(es->p); 	//�ͷ��ڴ�	
		}else if(es->state==ES_TCPSERVER_CLOSING)//��Ҫ�ر�����?ִ�йرղ���
		{
			tcp_server_connection_close(tpcb,es);//�ر�����
		}
		ret_err=ERR_OK;
	}else
	{
		tcp_abort(tpcb);//��ֹ����,ɾ��pcb���ƿ�
		ret_err=ERR_ABRT; 
	}
	return ret_err;
} 
//lwIP tcp_sent�Ļص�����(����Զ���������յ�ACK�źź�������)
err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct tcp_server_struct *es;
	LWIP_UNUSED_ARG(len); 
	es = (struct tcp_server_struct *) arg;
	if(es->p)tcp_server_senddata(tpcb,es);//��������
	return ERR_OK;
} 
//�˺���������������
void tcp_server_senddata(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
	struct pbuf *ptr;
	u16 plen;
	err_t wr_err=ERR_OK;
	 while((wr_err==ERR_OK)&&es->p&&(es->p->len<=tcp_sndbuf(tpcb)))
	 {
		ptr=es->p;
		wr_err=tcp_write(tpcb,ptr->payload,ptr->len,1); //��Ҫ���͵����ݼ��뷢�ͻ��������
		if(wr_err==ERR_OK)
		{ 
			plen=ptr->len;
			es->p=ptr->next;			//ָ����һ��pbuf
			if(es->p)pbuf_ref(es->p);	//pbuf��ref��һ
			pbuf_free(ptr);
			tcp_recved(tpcb,plen); 		//����tcp���ڴ�С
		}else if(wr_err==ERR_MEM)es->p=ptr;
		tcp_output(tpcb);   //�����ͻ�������е����ݷ��ͳ�ȥ
	 }
} 
//�ر�tcp����
void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
	tcp_close(tpcb);
	tcp_arg(tpcb,NULL);
	tcp_sent(tpcb,NULL);
	tcp_recv(tpcb,NULL);
	tcp_err(tpcb,NULL);
	tcp_poll(tpcb,NULL,0);
	if(es)mem_free(es); 
	tcp_server_flag&=~(1<<5);//������ӶϿ���
}
extern void tcp_pcb_purge(struct tcp_pcb *pcb);	//�� tcp.c���� 
extern struct tcp_pcb *tcp_active_pcbs;			//�� tcp.c���� 
extern struct tcp_pcb *tcp_tw_pcbs;				//�� tcp.c����  
//ǿ��ɾ��TCP Server�����Ͽ�ʱ��time wait
void tcp_server_remove_timewait(void)
{
	struct tcp_pcb *pcb,*pcb2; 
	u8 t=0;
	while(tcp_active_pcbs!=NULL&&t<200)
	{
		lwip_periodic_handle();	//������ѯ
		t++;
		delay_ms(10);			//�ȴ�tcp_active_pcbsΪ��  
	}
	pcb=tcp_tw_pcbs;
	while(pcb!=NULL)//����еȴ�״̬��pcbs
	{
		tcp_pcb_purge(pcb); 
		tcp_tw_pcbs=pcb->next;
		pcb2=pcb;
		pcb=pcb->next;
		memp_free(MEMP_TCP_PCB,pcb2);	
	}
}



































