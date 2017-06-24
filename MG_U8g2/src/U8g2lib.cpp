// These functions have been modified to work with MicroGrader

#include "U8g2lib.h"

void U8G2::begin() {
    /* note: call to u8x8_utf8_init is not required here, this is done in the setup procedures before */
    initDisplay();
    clearDisplay();
    setPowerSave(0);

    if (MicroGrader.mg_mode != INACTIVE) { // Send INIT message for MicroGrader
        uint8_t msg[2]; // will contain height and width in 8x8 "tiles"
        msg[0] = getBufferTileWidth();
        msg[1] = getBufferTileHeight();
        MicroGrader.sendMessage(MG_OLED_INIT, msg, 2);
    }
}

void U8G2::sendBuffer() {
    u8g2_SendBuffer(&u8g2);
    if (MicroGrader.mg_mode != INACTIVE) {  // Send the buffer over serial
        uint8_t *ptr = getBufferPtr(); // Organized in 8x8 tiles
        uint32_t buffer_len = getBufferTileWidth() * getBufferTileHeight() * 8;
        MicroGrader.sendMessage(MG_OLED_FULL, ptr, buffer_len);
    }
}


