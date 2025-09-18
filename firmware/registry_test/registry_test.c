#include <stdio.h>
#include "pico/stdlib.h"


#define Q_PIN 13
#define CLK_PIN 14
#define SH_LD_PIN 15


int main()
{
    stdio_init_all();

    gpio_init(Q_PIN);
    gpio_set_dir(Q_PIN, GPIO_IN);

    gpio_init(CLK_PIN);
    gpio_set_dir(CLK_PIN, GPIO_OUT);
    gpio_put(CLK_PIN, 0);

    gpio_init(SH_LD_PIN);
    gpio_set_dir(SH_LD_PIN, GPIO_OUT);
    gpio_put(SH_LD_PIN, 1);


    while (true)
    {
        gpio_put(SH_LD_PIN, 0);
        sleep_ms(1);
        gpio_put(SH_LD_PIN, 1);
        sleep_ms(10);

        for (uint8_t i = 0; i < 8; i++)
        {
            uint8_t q = gpio_get(Q_PIN);

            printf("%d", q);

            gpio_put(CLK_PIN, 1);
            sleep_ms(1);
            gpio_put(CLK_PIN, 0);
            sleep_ms(1);
        }

        printf("\n");

        sleep_ms(1000);
    }
}
