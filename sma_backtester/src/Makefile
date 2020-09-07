IDIR =../include
CC=g++
CFLAGS=-I$(IDIR)

all: $(OBJ)
	$(CC) -c *.cpp $(CFLAGS) 
	$(CC) -g -o sma_backtest *.o $(CFLAGS)
	rm -f *.o
	
clean:
	rm -f *.o *~ core
