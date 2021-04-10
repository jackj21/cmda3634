INCLUDE_DIR := include
SRC_DIR := src

OBJECTS := array_2d.o wave.o

INCLUDES := -fopenmp -lm

all: wave_timing wave_images wave_animation

array_2d.o:
	gcc -o array_2d.o -I$(INCLUDE_DIR) -c $(SRC_DIR)/array_2d.c $(INCLUDES) 

wave.o: array_2d.o
	gcc -o wave.o -I$(INCLUDE_DIR) -c $(SRC_DIR)/wave.c $(INCLUDES) 

wave_timing: $(OBJECTS)
	gcc -o wave_timing wave_timing.c -I$(INCLUDE_DIR) $(OBJECTS) $(INCLUDES) 

wave_images: $(OBJECTS)
	gcc -o wave_images wave_images.c -I$(INCLUDE_DIR) $(OBJECTS) $(INCLUDES)

wave_animation: $(OBJECTS)
	gcc -o wave_animation wave_animation.c -I$(INCLUDE_DIR) $(OBJECTS) $(INCLUDES)

clean:
	rm -f *.o
	rm -f wave_timing
	rm -f wave_images
	rm -f wave_animation
