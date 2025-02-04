#include <pico/sleep.h>
#include <hardware/pll.h>
#include <hardware/clocks.h>
#include <cstdio>
#include <pico/multicore.h>
#include <hardware/xosc.h>
#include "System.h"
#include "UI.h"
#include "platform_config.h"
#include "audio/SIDPlayer.h"

struct repeating_timer tudTaskTimer{};

bool connected = false;

void System::configureClocks() {
    TRACE_PRINTF(">>> %s\n", __FUNCTION__);
    clocks_hw->resus.ctrl = 0;
    xosc_init();
    pll_init(pll_sys, 1, 1500 * MHZ, 6, 2);
    pll_init(pll_sys, 1, 1500 * MHZ, 6, 2);
    clock_configure(clk_ref,
                    CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC,
                    0,
                    12 * MHZ,
                    12 * MHZ);
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
                    125 * MHZ,
                    125 * MHZ);
    clock_configure(clk_adc,
                    0,
                    CLOCKS_CLK_ADC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                    48 * MHZ,
                    48 * MHZ);
    clock_configure(clk_rtc,
                    0,
                    CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                    48 * MHZ,
                    46875);
    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    125 * MHZ,
                    125 * MHZ);
    stdio_init_all();
}

void System::softReset() {
    AIRCR_Register = SYSRESETREQ;
}

void System::goDormant() {
    SIDPlayer::ampOff();
    sleepUntilDoubleClick();
    configureClocks();
}

void System::sleepUntilDoubleClick() {
    sleep_run_from_rosc();
    bool sleep = true;
    while (sleep) {
        sleep_goto_dormant_until_pin(ENC_SW_PIN, true, false);
        gpio_pull_up(ENC_SW_PIN);
        bool lastSwitchState = !gpio_get(ENC_SW_PIN);
        for (int i = 0; i < DOUBLE_CLICK_SPEED_MS; i++) {
            bool currentSwitchState = !gpio_get(ENC_SW_PIN);
            if (currentSwitchState && currentSwitchState != lastSwitchState) {
                sleep = false;
                break;
            }
            lastSwitchState = currentSwitchState;
            busy_wait_ms(1);
        }
    }
    gpio_pull_up(ENC_SW_PIN);
}
