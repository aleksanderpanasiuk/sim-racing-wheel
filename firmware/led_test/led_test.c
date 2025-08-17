// Basic program for testing ws2811 RGB leds
// pio assembly code is from
// https://github.com/sladorsoft/RaspberryPiPico/tree/main/tutorials/PIO%20explained



#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "hardware/dma.h"

#include "ws2811.pio.h"
// #include "ws2812b.pio.h"
// #include "ws2812c.pio.h"
// #include "ws2812d.pio.h"


// uncomment one of these to use its implementation
#define USE_WS2811_PIO
//#define USE_WS2812B_PIO
//#define USE_WS2812C_PIO
// #define USE_WS2812D_PIO

#define WS2811_PIN 15
#define LED_COUNT 2
#define COLOURS_COUNT 5

uint32_t colours[COLOURS_COUNT] =
{
    0x110539,
    0x850913,
    0xEDFA58,
    0xCD4948,
    0x58EA70
};

uint32_t led_buffer[LED_COUNT];


int main()
{
    PIO pio = pio0;
    const uint sm = 0;
    const int led_dma_chan = 0;

    uint offset = pio_add_program(pio, &ws2811_program);
    ws2811_program_init(pio, sm, offset, WS2811_PIN);

    // initialise the used GPIO pin to LOW
    gpio_put(WS2811_PIN, false);

    // wait for the WS2812s to reset
    sleep_us(50);

    // configure DMA to copy the LED buffer to the PIO state machine's FIFO
    dma_channel_config dma_ch0 = dma_channel_get_default_config(led_dma_chan);
    channel_config_set_transfer_data_size(&dma_ch0, DMA_SIZE_32);
    channel_config_set_read_increment(&dma_ch0, true);
    channel_config_set_write_increment(&dma_ch0, false);
    channel_config_set_dreq(&dma_ch0, DREQ_PIO0_TX0);

    // run the state machine
    pio_sm_set_enabled(pio, sm, true);


    uint32_t first_colour = 0;
    while (true)
    {
        for (uint32_t i = 0; i < LED_COUNT; i++)
        {
            led_buffer[i] = colours[(first_colour + i) % COLOURS_COUNT];
        }

        // initiate DMA transfer
        dma_channel_configure(led_dma_chan, &dma_ch0, &pio->txf[sm], led_buffer, LED_COUNT, true);

        sleep_ms(500);
        first_colour = (first_colour + 1) % COLOURS_COUNT;
        // initialise the used GPIO pin to LOW
        gpio_put(WS2811_PIN, false);

        // wait for the WS2812s to reset
        sleep_us(50);
    }

    return 0;
}