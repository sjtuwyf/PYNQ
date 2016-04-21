/*
 * pmod.h
 * Common header file for all PMOD IOP applications
 * API and drivers for IIC, SPI
 * Includes address mappings, and API for GPIO, SPI and IIC,
 * IOP Switch pin mappings and configuration
 * 
 * 
 * Author: cmccabe
 * Version 1.0 19 Nov 2015
 *
 *
 */

#ifndef PMOD_H_
#define PMOD_H_

#include "xparameters.h"
#include "xspi.h"      /* SPI device driver */
#include "xspi_l.h"
#include "xtmrctr.h"
#include "xiic.h"

// Memory map
#define IIC_BASEADDR      XPAR_IIC_0_BASEADDR
#define SPI_BASEADDR      XPAR_SPI_0_BASEADDR // base address of QSPI[0]
#define SWITCH_BASEADDR    XPAR_MB_JB_MB1_PMOD_IO_SWITCH_IP_S00_AXI_BASEADDR // Base address of switch
#define GPIO             XPAR_GPIO_0_BASEADDR

#define MAILBOX_CMD_ADDR       (*(volatile u32 *)(0x00007FFC)) // command from A9 to MB

#define MAILBOX_DATA(x)        (*(volatile u32 *)(0x00007000 +((x)*4))) 
#define MAILBOX_DATA_PTR(x)    ( (volatile u32 *)(0x00007000 +((x)*4)))
 
#define MAILBOX_DATA_FLOAT(x)     (*(volatile float *)(0x00007000 +((x)*4))) 
#define MAILBOX_DATA_FLOAT_PTR(x) ( (volatile float *)(0x00007000 +((x)*4))) 


// Switch mappings used for IOP Switch configuration
#define GPIO_0 0x0
#define GPIO_1 0x1
#define GPIO_2 0x2
#define GPIO_3 0x3
#define GPIO_4 0x4
#define GPIO_5 0x5
#define GPIO_6 0x6
#define GPIO_7 0x7
#define SCL    0x8
#define SDA    0x9
#define SPICLK 0xa
#define MISO   0xb
#define MOSI   0xc
#define SS     0xd
#define PWM	   0xe
#define TIMER  0xf

// SPI API
// Predefined Control signals macros found in xspi_l.h
#define SPI_INHIBIT (XSP_CR_TRANS_INHIBIT_MASK|XSP_CR_MANUAL_SS_MASK|XSP_CR_ENABLE_MASK|XSP_CR_MASTER_MODE_MASK)
#define SPI_RELEASE (XSP_CR_MANUAL_SS_MASK|XSP_CR_MASTER_MODE_MASK|XSP_CR_ENABLE_MASK)

void spi_init(void);
void spi_transfer(u32 BaseAddress, int bytecount, u8* readBuffer, u8* writeBuffer);

// IIC API
int iic_read(u32 addr, u8* buffer, u8 numbytes); 
int iic_write(u32 addr, u8* buffer, u8 numbytes);

void delay_us(int usdelay);
void delay_ms(u32 ms_count);

// Logging API for sensor PMODs - using mailbox as a circular buffer.  
// Borrow existing circular buffer implementation, modify to match mailbox API
// http://stackoverflow.com/questions/827691/how-do-you-implement-a-circular-buffer-in-c
typedef struct circular_buffer
{
  volatile void *buffer;     // data buffer
  void *buffer_end;          // end of data buffer
  size_t capacity;           // maximum number of items in the buffer
  size_t count;              // number of items in the buffer
  size_t sz;                 // size of each item in the buffer
  volatile void *head;       // pointer to head
  volatile void *tail;       // pointer to tail
} circular_buffer;

circular_buffer pmod_log;

int cb_init(circular_buffer *cb, volatile u32* log_start_addr, size_t capacity, size_t sz);
void cb_push_back(circular_buffer *cb, const void *item);
void cb_push_back_float(circular_buffer *cb, const float *item);
void cb_push_incr_ptrs(circular_buffer *cb);


// Switch Configuration
void configureSwitch(char pin1, char pin2, char pin3, char pin4, char pin5, char pin6, char pin7, char pin8);

// Initialize all PMOD IO Switch connected devices
void pmod_init(void);

#endif /* PMOD_H_ */