#include "PSIDCatalog.h"
#include "audio/SIDPlayer.h"
#include "UI.h"
#include "System.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "platform_config.h"

using namespace std;

extern "C" void filesystem_init();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main() {
    TRACE_PRINTF("\r\n***SIDPod init***\r\n");
    TRACE_PRINTF("Start configureClocks\r\n");
    System::configureClocks();
    TRACE_PRINTF("Start initUI\r\n");
    UI::initUI();
    TRACE_PRINTF("Start screenOn\r\n");
    UI::screenOn();
    TRACE_PRINTF("Start filesystem_init\r\n");
    filesystem_init();
    TRACE_PRINTF("Start PSIDCatalog::refresh\r\n");
    PSIDCatalog::refresh();
    TRACE_PRINTF("Start UI::start\r\n");
    UI::start();
    TRACE_PRINTF("Start SIDPlayer::initAudio\r\n");
    SIDPlayer::initAudio();
    TRACE_PRINTF("Start UI::updatedUI loop\r\n");
    while (true) {
        UI::updateUI();
    }
}
#pragma clang diagnostic pop
