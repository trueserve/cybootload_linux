## cybootload_linux
An attempt to use Cypress AN68272 "UART Bootloader Host" on Linux, by modifying
`communication_api.c` and `main.c`.

This version supports SPI bootloading, which as an example is useful for updating
firmware to a device connected to a Raspberry Pi via SPI pins.

### Build:
> make
> make test

### Usage:
> ./cybootload_linux "./Bootloadable Blinking LED.cyacd"
> ./cybootload -d /dev/spidev0.0 CY8CKIT-049-41XX_PWM_Example.cyacd
> ./cybootload -d /dev/spidev0.0 -s 1000000 CY8CKIT-049-41XX_UART_Example.cyacd

### Bootloader Functions:
Configure your bootloader as "Custom interface" and use the comms functions present in `bootloadable/custom_bootloader.c`.

### Examples included:
* * LED demo: LED starts blinking at a slow speed.
* > make test
./cybootload "Bootloadable Blinking LED.cyacd"


* * GPIO demo: once programmed, press SW1 swich on board to lit the LED. When SW1 is released,
light goes off. 
* > make test1
./cybootload CY8CKIT-049-41XX_GPIO_Example.cyacd


* * LED PWM demo: lights from low intensity to high and then to low, and repeats.
* > make test2
./cybootload_linux CY8CKIT-049-41XX_PWM_Example.cyacd


* * UART demo: start minicom -D /dev/ttyACM0, and press SW1 switch on board.
* * minicom displays `Hello World!`.
* > make test2
./cybootload_linux CY8CKIT-049-41XX_UART_Example.cyacd


### Ref:
PSoC® 3, PSoC 4, and PSoC 5LP UART Bootloader AN68272 
http://www.cypress.com/?rID=50230&source=AN68272
AN68272.zip (UART Bootloader appnote)
http://www.cypress.com/file/45166/download
AN84401.zip (SPI Bootloader appnote)
https://www.cypress.com/file/46121/download


https://en.wikibooks.org/wiki/Serial_Programming/Serial_Linux

https://www.eewiki.net/display/microcontroller/Getting+Started+with+PSoC+4+Prototyping+Kits+-+CY8CKIT-049
CY8CKIT-049-41XX GPIO_Example.zip
https://www.eewiki.net/download/attachments/39256220/CY8CKIT-049-41XX_GPIO_Example.zip?version=1&modificationDate=1412268358807&api=v2
CY8CKIT-049-41XX PWM_Example.zip
https://www.eewiki.net/download/attachments/39256220/CY8CKIT-049-41XX_PWM_Example.zip?version=1&modificationDate=1412268383543&api=v2
CY8CKIT-049-41XX UART_Example.zip
https://www.eewiki.net/download/attachments/39256220/CY8CKIT-049-41XX_UART_Example.zip?version=1&modificationDate=1412268420980&api=v2