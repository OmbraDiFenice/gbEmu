#pragma once

unsigned char* loadData(const char* filename);

#define BIT(x, bit) ((((x) & (1 << (bit))) & 0xFF) >> (bit))