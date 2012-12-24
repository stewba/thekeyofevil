#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <alsa/asoundlib.h>

#define C4 60
#define D4 62
#define E4 64
#define F4 65
#define G4 67
#define A5 69
#define B5 71
#define C5 72
#define EXIT 60


/* Declare widgets etc.
-----------------------*/

/* black background window */
GtkWindow *window_bg;
GdkPixbuf *pixbuf_bg;
GtkWidget *picture_bg;

/* image display window */
GtkWindow *window;
GdkPixbuf *pixbuf;
GtkWidget *picture;

//Midi handling
snd_rawmidi_t *midiInHandle;
/* Event handler functions */

static gboolean delete_event( GtkWidget *widget, GdkEvent  *event, gpointer   data )
{
    return FALSE;
}

static void destroy( GtkWidget *widget, gpointer   data )
{
    gtk_main_quit ();
}


static void update_image(char* image)
{
    gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(picture));
    pixbuf = gdk_pixbuf_new_from_file_at_scale(image, 1280, 1024, TRUE, NULL);
    picture=gtk_image_new_from_pixbuf(pixbuf);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(picture));
    gtk_widget_show_all(GTK_WIDGET(window));

}

static void midi_update_image(int midi_note)
{

    switch(midi_note)
    {
    case C4:
        //printf("%c",event->keyval);
        update_image("c4.jpg");
        break;

    case D4:
        //printf("%c",event->keyval);
        update_image("d4.jpg");
        break;

    case E4:
        //printf("%c",event->keyval);
        update_image("e4.jpg");
        break;

    case F4:
        //printf("%c",event->keyval);
        update_image("f4.jpg");
        break;

    case G4:
        //printf("%c",event->keyval);
        update_image("g4.jpg");
        break;

    case A5:
        //printf("%c",event->keyval);
        update_image("a5.jpg");
        break;

    case B5:
        //printf("%c",event->keyval);
        update_image("b5.jpg");
        break;

    case C5:
        //printf("%c",event->keyval);
        update_image("c5.jpg");
        break;

    case 96:
        //printf("%c",event->keyval);
        gtk_exit(0);
        break;


    }

}


static void check_midi()
{

    unsigned char command;
    unsigned char note;
    unsigned char velocity;

    register int err;


    if ((err = snd_rawmidi_read(midiInHandle, &command, 1)) < 0)
    {
        printf("Can't read MIDI input: %s\n", snd_strerror(err));
    }

    snd_rawmidi_read(midiInHandle, &note, 1);
    snd_rawmidi_read(midiInHandle, &velocity, 1);

    printf("%d\n",note);
    midi_update_image(note);


}


static void button_press_callback( GtkWidget *widget, GdkEventKey *event, gpointer data )
{


    switch(event->keyval)
    {
    case 'a':
        //printf("%c",event->keyval);
        update_image("a.jpg");
        break;

    case 'b':
        //printf("%c",event->keyval);
        update_image("b.jpg");
        break;

    case 'c':
        //printf("%c",event->keyval);
        update_image("c.jpg");
        break;

    case 'd':
        //printf("%c",event->keyval);
        update_image("d.jpg");
        break;

    case 'e':
        //printf("%c",event->keyval);
        update_image("e.jpg");
        break;

    case 'f':
        //printf("%c",event->keyval);
        update_image("f.jpg");
        break;

    }

}

void list_midi_interfaces(char* midi_dev, int bufsize)
{
    register int  err;
    int  cardNum;
    char devices[5][14];
    char user_midi[bufsize];

    // Start with first card
    cardNum = -1;

    for (;;)
    {
        snd_ctl_t *cardHandle;

        // Get next sound card's card number. When "cardNum" == -1, then ALSA
        // fetches the first card
        if ((err = snd_card_next(&cardNum)) < 0)
        {
            printf("Can't get the next card number: %s\n", snd_strerror(err));
            break;
        }

        // No more cards? ALSA sets "cardNum" to -1 if so
        if (cardNum < 0) break;

        // Open this card's control interface. We specify only the card number -- not
        // any device nor sub-device too
        {
            char   str[64];

            sprintf(str, "hw:%i", cardNum);
            if ((err = snd_ctl_open(&cardHandle, str, 0)) < 0)
            {
                printf("Can't open card %i: %s\n", cardNum, snd_strerror(err));
                continue;
            }
        }

        {
            int      devNum;

            // Start with the first MIDI device on this card
            devNum = -1;

            for (;;)
            {
                snd_rawmidi_info_t  *rawMidiInfo;
                register int        subDevCount, i;

                // Get the number of the next MIDI device on this card
                if ((err = snd_ctl_rawmidi_next_device(cardHandle, &devNum)) < 0)
                {
                    printf("Can't get next MIDI device number: %s\n", snd_strerror(err));
                    break;
                }

                // No more MIDI devices on this card? ALSA sets "devNum" to -1 if so.
                // NOTE: It's possible that this sound card may have no MIDI devices on it
                // at all, for example if it's only a digital audio card
                if (devNum < 0) break;

                // To get some info about the subdevices of this MIDI device (on the card), we need a
                // snd_rawmidi_info_t, so let's allocate one on the stack
                snd_rawmidi_info_alloca(&rawMidiInfo);
                memset(rawMidiInfo, 0, snd_rawmidi_info_sizeof());

                // Tell ALSA which device (number) we want info about
                snd_rawmidi_info_set_device(rawMidiInfo, devNum);

                // Get info on the MIDI outs of this device
                snd_rawmidi_info_set_stream(rawMidiInfo, SND_RAWMIDI_STREAM_INPUT);

                i = -1;
                subDevCount = 1;

                // More subdevices?
                while (++i < subDevCount)
                {
                    // Tell ALSA to fill in our snd_rawmidi_info_t with info on this subdevice
                    snd_rawmidi_info_set_subdevice(rawMidiInfo, i);
                    if ((err = snd_ctl_rawmidi_info(cardHandle, rawMidiInfo)) < 0)
                    {
                        printf("Can't get info for MIDI input subdevice hw:%i,%i,%i: %s\n", cardNum, devNum, i, snd_strerror(err));
                        continue;
                    }

                    // Print out how many subdevices (once only)
                    if (!i)
                    {
                        subDevCount = snd_rawmidi_info_get_subdevices_count(rawMidiInfo);
                        printf("\nFound %i MIDI input subdevices on card %i\n", subDevCount, cardNum);
                    }

                    // NOTE: If there's only one subdevice, then the subdevice number is immaterial,
                    // and can be omitted when you specify the hardware name
                    printf((subDevCount > 1 ? "[%i] -    hw:%i,%i,%i\n" : "[%i] -    hw:%i,%i\n"), i, cardNum, devNum, i);
                    sprintf(devices[i], (subDevCount > 1 ? "hw:%i,%i,%i\n":"hw:%i,%i\n"),cardNum, devNum, i);
                }
            }
        }

        // Close the card's control interface after we're done with it
        snd_ctl_close(cardHandle);


    }

    snd_config_update_free_global();

    printf("\nSelect the MIDI input to use\n");
    scanf("%s",user_midi);

    strncpy(midi_dev,user_midi,bufsize);
}

int main( int argc, char *argv[] )
{
    int buf_size = 7;
    char* midi_device = (char*) malloc(sizeof(char) * buf_size);

    list_midi_interfaces(midi_device,buf_size);

    printf("Users selected %s\n",midi_device);

    register int err;
    //midi_device = "hw:2,0";

    // Open input MIDI device hw:0,0,0
    if ((err = snd_rawmidi_open(&midiInHandle, 0, midi_device, 0)) < 0)
    {
        printf("Can't open MIDI input: %s\n", snd_strerror(err));
    }

    /* Initialise GTK+ */
    gtk_init(&argc, &argv);


    /* ***** Create photo display ******/
    window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    pixbuf = gdk_pixbuf_new_from_file_at_scale("e4.jpg", 1280, 1024, TRUE, NULL);
    picture=gtk_image_new_from_pixbuf(pixbuf);

    /* signal handlers would be here */
    g_signal_connect (G_OBJECT (window), "delete_event",G_CALLBACK (delete_event), NULL);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
    //g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (button_press_callback), NULL);

    /* pack the image into the window & display */
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(picture));
    gtk_widget_show_all(GTK_WIDGET(window));
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    /* Set window to have no border etc. */
    gtk_window_set_decorated(window, FALSE);
    gtk_window_fullscreen(window);

    g_idle_add ((GSourceFunc)check_midi,NULL);
    /* Start main event loop */
    gtk_main();

    return 0;
}



