/*
 * Use these functions with a custom bootloader.
 * The built-in bootloader comms don't seem to work due
 * to delays present and maybe other things.
 */

void CyBtldrCommStart(void)
{
    SPIS_Start();
}
 
void CyBtldrCommStop(void)
{
    SPIS_Stop();
}
 
void CyBtldrCommReset(void)
{
	SPIS_SpiUartClearRxBuffer();
	SPIS_SpiUartClearTxBuffer();
}
 
cystatus CyBtldrCommWrite(uint8 *buffer, uint16 size, uint16 *count, uint8 timeOut) {
    uint16 c = *count;
    (void)timeOut;
    
    // disable rx fifo
    SPIS_RX_FIFO_CTRL_REG |= SPIS_RX_FIFO_CTRL_FREEZE;
    
    for(c = size; c; c--) {
        SPIS_SpiUartWriteTxData((uint8_t)*buffer++);  
	}
    
    // wait to complete before returning
    while (SPIS_SPI_STATUS_REG & SPIS_SPI_STATUS_BUS_BUSY);
    // clear anything possibly received in rx fifo before disabling, and enable it
    SPIS_RX_FIFO_CTRL_REG &= ~SPIS_RX_FIFO_CTRL_FREEZE;
    
    *count = size;
    return CYRET_SUCCESS;
}
 
cystatus CyBtldrCommRead(uint8 *buffer, uint16 size, uint16 *count, uint8 timeOut) {
	*count = 0;
    int timeoutUs = timeOut * 10000;
	
    cystatus status = CYRET_TIMEOUT;
    
    while(*count < size && timeoutUs >= 0) {
		if(SPIS_SpiUartGetRxBufferSize() > 0) {
			buffer[(*count)++] = SPIS_SpiUartReadRxData();
			// Switch to byte-to-byte timeout and mark as success
			timeoutUs = 1000; // 1mS
			status = CYRET_SUCCESS;
		} else {
			CyDelayUs(10);
			timeoutUs -= 10;
		}
	}
    
	return status;
}