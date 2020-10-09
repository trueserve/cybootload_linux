SRC=main.c comm_api_spi.c cybtldr_api.c cybtldr_command.c cybtldr_parse.c
OBJ=$(subst .c,.o,$(SRC))
all: cybootload

%.o: %.c
#	gcc -DDEBUG -Wall -g -c $< -o $@
	gcc -Wall -g -c $< -o $@

cybootload: $(OBJ)
	gcc -Wall -g -o $@ $+

test1:
	./cybootload "test/Bootloadable Blinking LED.cyacd"
test2:
	./cybootload test/CY8CKIT-049-41XX_GPIO_Example.cyacd
test3:
	./cybootload test/CY8CKIT-049-41XX_PWM_Example.cyacd

test4:
	./cybootload test/CY8CKIT-049-41XX_UART_Example.cyacd

clean:
	-rm *.o cybootload

StringImage.h: "Bootloadable Blinking LED.cyacd"
	perl conv.pl "$<"


gdb: cybootload
	-rm -f gdb.log
	gdb -batch -x run.gdb
