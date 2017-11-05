#ifndef PATTERN_H
#define PATTERN_H


static const uint8_t strokePatterns[][8] PROGMEM = {
    // Space
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

    // Dot
    {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },

    // Left bracket
    {0x18, 0x3c, 0x42, 0x81, 0x00, 0x00, 0x00, 0x00 },

    // Right bracket
    {0x81, 0x42, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x00 },

    // 0
    {0x7e, 0x81, 0x81, 0x81, 0x81, 0x7e, 0x00, 0x00 },

    // 2
    {0x82, 0xc1, 0xe1, 0x91, 0x89, 0x86, 0x00, 0x00 },

    // 3
    {0x41, 0x81, 0x81, 0x8f, 0x93, 0x61, 0x00, 0x00 },

    // Part of 4
    {0xe0, 0xd0, 0x8c, 0x82, 0x81, 0x80, 0x00, 0x00 },

    // Part of 5
    {0x47, 0x84, 0x84, 0x84, 0x80, 0x78, 0x00, 0x00 },

    // 6
    {0x7c, 0x9a, 0x99, 0x99, 0x91, 0x60, 0x00, 0x00 },

    // Part of 7
    {0x01, 0xe1, 0x11, 0x09, 0x05, 0x03, 0x00, 0x00 },

    // Part of 8
    {0x66, 0x99, 0x99, 0x99, 0x99, 0x66, 0x00, 0x00 },

    // Part of 9
    {0x06, 0x89, 0x99, 0x59, 0x59, 0x3e, 0x00, 0x00 },

    // Vertical line
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },

    // Horizontal line
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },

    // Slash
    {0x80, 0x60, 0x10, 0x08, 0x06, 0x01, 0x00, 0x00 },

    // Back slash
    {0x01, 0x06, 0x08, 0x10, 0x60, 0x80, 0x00, 0x00 }
};


#endif