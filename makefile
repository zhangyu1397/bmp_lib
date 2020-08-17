CC := gcc

target := jpeg_test

source := main.o \
				bmp.o 
CFLAGS +=


$(target): $(source)
	$(CC) -o $(target) $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o