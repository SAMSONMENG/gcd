/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "xparameters.h"
#include "platform.h"
#include "xil_printf.h"
#include "xaxidma.h"
#include "xgcd.h"
#include "XTime_l.h"

#define MEM_BASE_ADDRESS 0x0A000000
#define TX_BUFFER_BASE (MEM_BASE_ADDRESS + 0X00100000)
#define RX_BUFFER_BASE (MEM_BASE_ADDRESS + 0X00300000)

XGcd gcd;
XGcd_Config *XGcd_config;

XAxiDma axidma;
XAxiDma_Config *axi_dma_config;

u32 instreamdata[2];

void initperipherls()
{
//	 printf("initizlizing natrix mul.... \n");
//   matrix_mul_config= XMatrixmul_LookupConfig(XPAR_MATRIXMUL_0_DEVICE_ID);
//	 if (matrix_mul_config)
	printf("initizlizing natrix mul.... \n");
	XGcd_config= XGcd_LookupConfig(XPAR_GCD_0_DEVICE_ID);
	if (XGcd_config)
	{
		int status= XGcd_CfgInitialize(&gcd, XGcd_config);

		if(status!= XST_SUCCESS)
		{
			printf("ERROR IN MATRIX MUL CORE \n");
		}
	}

	printf("initializing AXI DMA  core\n");
	axi_dma_config=XAxiDma_LookupConfig(XPAR_AXIDMA_0_DEVICE_ID);
	if (axi_dma_config)
	{
		int status= XAxiDma_CfgInitialize(&axidma, axi_dma_config);

		if (status!= XST_SUCCESS)
		{
			printf("AXI DMA error \n");
		}
	}

	XAxiDma_IntrDisable(&axidma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&axidma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	//XAxiDma_IntrEnable(&axidma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	//XAxiDma_IntrEnable(&axidma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
}


int main()
{
	  for(int i=0;i<2;i++)
	    {
    init_platform();

    int *m_dma_buffer_Rx= (int*) RX_BUFFER_BASE;

    u32 x,y;
    XTime ST,SP;
    initperipherls();


    	if(i==0)
    	{
          printf("value1\n");
          scanf("%ld",&x);
          printf("value2");
          scanf("%ld",&y);
          instreamdata[0]=x;
    	  instreamdata[1]=y;
    	}
    	else{
    		instreamdata[0]=x;
    		instreamdata[1]=y;
    	}
    	if(i==1)
          	{
          		printf("value 1 = %d\n",instreamdata[0]);
    		    printf("value 2 = %d\n",instreamdata[1]);
          		printf("GCD = %d\n", m_dma_buffer_Rx[1]);
          		printf("%.7f\n",1.0*(SP-ST)/(COUNTS_PER_SECOND/1));
          		break;
          	}
   // while (1)
   // {
        XTime_GetTime(&ST);
    	XGcd_Set_gain(&gcd, 1);

    	XGcd_Start(&gcd);

    	Xil_DCacheFlushRange((u32)instreamdata, 2*sizeof(int));
    	Xil_DCacheFlushRange((u32)m_dma_buffer_Rx, 1*sizeof(int));

    	printf("sending data to axi_dma... \n");
    	XAxiDma_SimpleTransfer(&axidma, (u32)instreamdata, 2*sizeof(int), XAXIDMA_DMA_TO_DEVICE) ;

    	printf("read data .... \n");
    	XAxiDma_SimpleTransfer(&axidma, (u32)m_dma_buffer_Rx, 1*sizeof(int), XAXIDMA_DEVICE_TO_DMA) ;

    	printf("hererrrrr \n");
    	printf("Xuyang \n");
 //   	while(XAxiDma_Busy(&axidma, XAXIDMA_DEVICE_TO_DMA));
    	printf("therere....\n");

    	Xil_DCacheInvalidateRange((u32)m_dma_buffer_Rx, 1*sizeof(int));

    	printf("before completion \n");

    	while(!XGcd_IsDone(&gcd));
    	XTime_GetTime(&SP);
    	printf("calculation complete..\n");
//    	if(i==1)
//          	{
//          		printf("value 1 = %d\n",instreamdata[0]);
//    		    printf("value 2 = %d\n",instreamdata[1]);
//          		printf("GCD = %d\n", m_dma_buffer_Rx[1]);
//          		printf("%.7f\n",1.0*(SP-ST)/(COUNTS_PER_SECOND/1000000));
//          	}
      }
   	//	printf("value [%d]= %d\n", m_dma_buffer_Rx);
    cleanup_platform();
    return 0;
}

