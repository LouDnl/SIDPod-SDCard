#include "hardware/flash.h"

#ifdef SOLDERPARTY_RP2040_STAMP
#define FLASH_STORAGE_BYTES (7552 * 1024)
#endif
#ifdef RASPBERRYPI_PICO
#define FLASH_STORAGE_BYTES                 (1408 * 1024)
#endif
#define SECTOR_COUNT                        (FLASH_STORAGE_BYTES / FLASH_SECTOR_SIZE)
#define FLASH_BASE_ADDR                     (PICO_FLASH_SIZE_BYTES - FLASH_STORAGE_BYTES)
#define FLASH_MMAP_ADDR                     (XIP_BASE + FLASH_BASE_ADDR)

#define FS_LABEL                            "SIDPod"

#define BOARD_TUD_RHPORT                    0

#ifndef HW_USB_VID
#define HW_USB_VID                          (0x2E8A)
#endif
#ifndef HW_USB_PID
#define HW_USB_PID                          (0x0006)
#endif

#define I2C_BAUDRATE                        400000
#define DISPLAY_WIDTH                       128
#define DISPLAY_HEIGHT                      32
#define DISPLAY_I2C_ADDRESS                 0x3C

#define AUDIO_RENDERING_STARTED_FIFO_FLAG   124

#define SAMPLE_RATE                         ((uint32_t)44100)
#define SAMPLES_PER_BUFFER                  (6 * 1024)

#define AMP_CONTROL_PIN                     15

#define ENC_A                               10
#define ENC_SW                              12