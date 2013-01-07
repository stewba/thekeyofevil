all:
	gcc main.c -o key_of_evil `pkg-config --cflags --libs gtk+-2.0 alsa`
