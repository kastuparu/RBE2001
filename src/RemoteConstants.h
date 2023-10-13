// This file contains constants for the address and commands in
// the messages transmitted by the Mini IR Remote Control:
// https://www.pololu.com/product/2777

#pragma once

#include <Arduino.h>

const uint16_t REMOTE_POWER = 0x45;
const uint16_t REMOTE_VOL_PLUS = 0x46;
const uint16_t REMOTE_FUNC_STOP = 0x47;

const uint8_t REMOTE_BACK = 0x44;
const uint8_t REMOTE_PLAY_PAUSE = 0x40;
const uint8_t REMOTE_CONTINUE = 0x43;

const uint8_t REMOTE_DOWN = 0x07;
const uint8_t REMOTE_VOL_MINUS = 0x15;
const uint8_t REMOTE_UP = 0x09;

const uint8_t REMOTE_0 = 0x16;
const uint8_t REMOTE_EQ = 0x19;
const uint8_t REMOTE_ST_REPT = 0x0D;

const uint8_t REMOTE_1 = 0x0C;
const uint8_t REMOTE_2 = 0x18;
const uint8_t REMOTE_3 = 0x5E;

const uint8_t REMOTE_4 = 0x08;
const uint8_t REMOTE_5 = 0x1C;
const uint8_t REMOTE_6 = 0x5A;

const uint8_t REMOTE_7 = 0x42;
const uint8_t REMOTE_8 = 0x52;
const uint8_t REMOTE_9 = 0x4A;
