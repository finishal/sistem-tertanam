#include <asf.h>
#include <stdio.h>

static char strbuf[128];

struct int_pair {
    int fi, se;
};

struct int_pair status[4] = {
    {1, 1},
    {0, 1},
    {1, 0},
    {0, 0},
};

int main(void) {
    /* Insert system clock initialization code here (sysclk_init()). */

    //init board
    board_init();

    //init lcd
    gfx_mono_init();

    //set background lcd on
    gpio_set_pin_high(LCD_BACKLIGHT_ENABLE_PIN);

    //set display - output lcd
    gfx_mono_draw_string("Sistem Tertanam", 0, 0, &sysfont);
    gfx_mono_draw_string("Faisal Hanif", 0, 8, &sysfont);
    gfx_mono_draw_string("Praktikum 01 : I/O", 0, 16, &sysfont);

    //toggle flag 1=low, 0=high
    int flagLed0 = 1;
    int flagLed1 = 1;

    int SW0, SW1, SW2;

    uint64_t counter = 0;
    uint64_t snumber = 0;

    //toggle led. if 0 > 1, if 1 > 0
    gpio_toggle_pin(LED0);
    gpio_toggle_pin(LED1);
    gpio_toggle_pin(LED2);

    //delay 1000ms = 1s
    delay_ms(1000);

    //toggle led. if 0 > 1, if 1 > 0
    gpio_toggle_pin(LED0);
    gpio_toggle_pin(LED1);
    gpio_toggle_pin(LED2);

    //set J1 Pin 0 to output
    ioport_set_pin_dir(J1_PIN0, IOPORT_DIR_OUTPUT);

    //forever loop
    while (1) {
        //check button press
        SW0 = ioport_get_pin_level(GPIO_PUSH_BUTTON_0);
        SW1 = ioport_get_pin_level(GPIO_PUSH_BUTTON_1);
        SW2 = ioport_get_pin_level(GPIO_PUSH_BUTTON_2);

        //toggle status
        if (SW0 == 0) {
            snumber++;
            snumber %= 4;
            flagLed0 = status[snumber].fi;
            flagLed1 = status[snumber].se;
        }

        //set pin based on flag.
        ioport_set_pin_level(LED0_GPIO, flagLed0);
        ioport_set_pin_level(LED1_GPIO, flagLed1);

        //increment or decrement counter
        if (SW1 == 0) {
            counter++;
        }
        if (SW2 == 0) {
            counter--;
        }

        snprintf(strbuf, sizeof(strbuf), "Counter : %3d", counter); // Show counter to lcd
        gfx_mono_draw_string(strbuf, 0, 24, &sysfont);

        if (SW1 == 0 || SW2 == 0) {
            ioport_set_pin_level(J1_PIN0, 1); //set high
            delay_ms(100);
            ioport_set_pin_level(J1_PIN0, 0); //set low
        }

        //delay for every loop
        delay_ms(50);
    }
}
