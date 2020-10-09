/*******************************************************************************
 * File Name: communication_api.c  
 * Version 1.0
 *
 * Description:
 * This file is created by the author . This contains definitions of APIs 
 * used in structure 'CyBtldr_CommunicationsData' defined in cybtldr_api.h ,
 * using SPI commuincations component 
 ********************************************************************************/

#include "comm_api_spi.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>


static int spi_fd;
extern int errno;

char    *comm_dev   = COMM_DEV_DEFAULT;
uint32_t comm_speed = COMM_SPEED_DEFAULT;
uint8_t  comm_mode  = COMM_MODE_DEFAULT;
uint8_t  comm_bits  = 0;
uint16_t comm_delay = 0;


/*******************************************************************************
 * Function Name: OpenConnection
 ********************************************************************************
 *
 * Summary:
 *  Initializes the communications component : In this case UART
 *
 * Parameters:  
 *  void
 *
 * Return: 
 *  Returns a flag to indicate whether the operation was successful or not
 *
 *
 *******************************************************************************/
int OpenConnection(void)
{
	/*
	UART_Start();
	*/
	if (comm_dev == NULL) {
		if ((comm_dev = strdup(COMM_DEV_DEFAULT)) == NULL) {
			printf("[ERROR] malloc failed\n");
			return(CYRET_ERR_FILE);
		}
	}

	spi_fd = open(comm_dev, O_RDWR | O_NOCTTY | O_SYNC);
	if (spi_fd < 0) {
		printf("[ERR.] opening %s : %s\n", comm_dev, strerror(errno));
		exit(1);
		// return (CYRET_ERR_FILE);
	}

	if (ioctl(spi_fd, SPI_IOC_WR_MODE, &comm_mode) == -1) {
		printf("[ERR.] device %s : can't set mode\n", comm_dev);
	}
	if (ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &comm_bits) == -1) {
		printf("[ERR.] device %s : can't set bits\n", comm_dev);
	}
	if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &comm_speed) == -1) {
		printf("[ERR.] device %s : can't set speed\n", comm_dev);
	}

	return(CYRET_SUCCESS);
}


/*******************************************************************************
 * Function Name: CloseConnection
 ********************************************************************************
 *
 * Summary:
 *  Clears the status and stops the communications component (UART).
 *
 * Parameters:  
 *  void
 *
 * Return: 
 *  Returns a flag to indicate whether the operation was successful or not
 *
 *
 *******************************************************************************/
int CloseConnection(void)
{
	/*
	UART_Stop();
	*/
	printf("[INFO] Closing communications device\n");
	close(spi_fd);
	return(CYRET_SUCCESS);
}

/*******************************************************************************
 * Function Name: WriteData
 ********************************************************************************
 *
 * Summary:
 *  Writes the specified number of bytes using the communications component (UART)
 *
 * Parameters:  
 *  wrData - Pointer to write data buffer
 *  byteCnt - No. of bytes to be written 
 *
 * Return: 
 *  Returns a flag to indicate whether the operation was successful or not
 *
 *
 *******************************************************************************/
int WriteData(uint8* wrData, int byteCnt)
{
	if (write(spi_fd, wrData, byteCnt) == -1) {
		printf("[ERR.]: writing data: %s\n", strerror(errno));
		return(CYRET_ERR_COMM_MASK);
	}

	return(CYRET_SUCCESS);
}


/*******************************************************************************
 * Function Name: ReadData
 ********************************************************************************
 *
 * Summary:
 *  Reads the specified number of bytes usign the communications component (UART)
 *
 * Parameters:  
 *  rdData - Pointer to read data buffer
 *  byteCnt - No. of bytes to be read 
 *
 * Return: 
 *  Returns a flag to indicate whether the operation was successful or not
 *
 *
 *******************************************************************************/
int ReadData(uint8* rdData, int byteCnt)
{
	// need to wait before reading, just in case we've just
	// sent and started a flash page write operation...
	// per datasheet, the longest time should be 20ms
	usleep(20000);

	struct spi_ioc_transfer rx = {
		.rx_buf = (unsigned long)rdData,
		.tx_buf = 0,
		.len = byteCnt,
		.speed_hz = comm_speed,
		.bits_per_word = comm_bits,
		.delay_usecs = comm_delay,
	};

	if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &rx) == -1) {
		printf("[ERR.] Read from target fail\n");
		return(CYRET_ERR_COMM_MASK);
	}

	// sometimes byte comms don't work correctly?
	// we've made sure the bootloader is set to 1ms, so just make sure
	// we're longer than that
	usleep(1200);

	return(CYRET_SUCCESS);
}

/* [] END OF FILE */
