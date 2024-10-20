CC = gcc
CFLAGS = -Wall -Wextra -g `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf
OBJ = main.o snake.o skins.o


TARGET = snake


all: $(TARGET)


$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)


%.o: %.c snake.h config.h
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c snake.h config.h
	$(CC) $(CFLAGS) -c $< -o $@

snake.o: snake.c snake.h config.h
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean