/*******************************************************************************
 * File Name: communication_api.h  
 * Version 1.0
 *
 * Description:
 * This is the header file for the communication module created by the author.  
 * It contains function prototypes and constants for the users' convenience. 
 ********************************************************************************/

#include <stdint.h>
#include "cybtldr_utils.h"


#define COMM_DEV_DEFAULT	"/dev/spidev0.0"
#define COMM_SPEED_DEFAULT	500000
#define COMM_MODE_DEFAULT	SPI_CPHA | SPI_CPOL;	// mode 3


typedef     unsigned char uint8;
typedef     unsigned short uint16;

extern char    *comm_dev;
extern uint32_t comm_speed;


/* Function declarations */
int OpenConnection(void);
int CloseConnection(void);
int ReadData(uint8* rdData, int byteCnt);
int WriteData(uint8* wrData, int byteCnt);

//[] END OF FILE

