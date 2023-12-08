#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// GCC-based only
#define AutoCleanBmp888 Bmp888 __attribute__((cleanup(free_bmp)))

// Tell the compiler do not pad the struct with "#pragma pack(push, 1)"!
#pragma pack(push, 1)
typedef struct {
	uint8_t signature[2];
	uint32_t size;
	uint16_t reserved0;
	uint16_t reserved1;
	uint32_t pixel_data_offset;
} Header;
#pragma pack(pop)

typedef struct {
	uint32_t byte_size;
	int32_t width;
	int32_t height;
	uint16_t color_plane_count;
	uint16_t color_depth;
	uint32_t compress_method;
	uint32_t raw_bitmap_size;
	int32_t horizonal_resolution;
	int32_t vertical_resolution;
	uint32_t table_entries;
	uint32_t important_colors;
} Info;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Col888;

typedef struct {
	Col888 *data;
	size_t size;
} VecCol888;

typedef struct {
	Header header;
	Info info;
	VecCol888 pixel_data;
	Col888 black_hole;
	bool ok;
} Bmp888;

VecCol888 new_veccol888(size_t size);
Bmp888 new_base_bmp();
Bmp888 new_bmp(int32_t width, int32_t height);
void free_bmp(Bmp888 *bmp);
Col888 *px_ptr(Bmp888 *bmp, int32_t x, int32_t y);
bool save_bmp(Bmp888 *bmp, char *filename);
Bmp888 load_bmp(const char *filename);
