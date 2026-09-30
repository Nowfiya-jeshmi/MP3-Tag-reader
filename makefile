CC = gcc
CFLAGS = -Wall

OBJ = main.o mp3_reader.o mp3_editor.o

mp3_tag_reader: $(OBJ)
	$(CC) $(OBJ) -o mp3_tag_reader

%.o: %.c header.h enum.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o mp3_tag_reader