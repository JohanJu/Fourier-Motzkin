# for power.ludat.lth.se
#CFLAGS	= -m64 -O3 -g -Wall -Wextra -Werror -std=c99 -mcpu=970 -mtune=970 -maltivec

# the following works on any machine
#CFLAGS	= -O3 -g -Wall -Wextra -Werror -std=c99

CFLAGS	= -O3 -std=c99
CC	= gcc 
OUT	= fm
OBJS	= main.o jjucbr.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)
	./fm
	size jjucbr.o

clean:
	rm -f $(OUT) $(OBJS)
