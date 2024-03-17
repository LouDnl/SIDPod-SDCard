#include <stdio.h>
#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"
#include "f_util.h"
#include "ff_util.h"
#include "hardware/rtc.h"
#include "hw_config.h"
#include "platform_config.h"

#ifndef MP_WEAK
#define MP_WEAK __attribute__((weak))
#endif

MP_WEAK DWORD get_fattime(void) {
    datetime_t t;
    rtc_get_datetime(&t);
    return ((t.year - 1980) << 25) | ((t.month) << 21) | ((t.day) << 16) | ((t.hour) << 11) | ((t.min) << 5) |
           (t.sec / 2);
}

void filesystem_init() {
    FATFS fs;
    sd_card_t *pSD = sd_get_by_num(0);
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    TRACE_PRINTF(">>> %s\n", __FUNCTION__);

    // Initialize SD card
    if (!sd_init_driver()) {
        panic("ERROR: Could not initialize SD card\r\n");
        while (true);
    }

    // Mount filesystem
    if (fr != FR_OK) {
        panic("f_mount error: %s (%d)\r\n", FRESULT_str(fr), fr);
        while (true);
    } else {
        f_unmount("");
    }
}