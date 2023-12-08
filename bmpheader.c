#include "bmpheader.h"
#define _CRT_SECURE_NO_WARNINGS


VecCol888 new_veccol888(size_t size) {
	VecCol888 v;
	v.data = malloc(size * sizeof(VecCol888));
	v.size = size;
	// The reason it's gray is cuz easier to debug your own software
	fill_vecol888(&v.data, (Col888){0x20, 0x20, 0x20});
	return v;
}

void fill_vecol888(VecCol888 *v, Col888 color) {
	for (size_t i = 0; i < v->size; i++) {
		v->data[i] = color;
	}
}

Bmp888 new_base_bmp() {
	return (Bmp888) {.black_hole = (Col888){0, 0, 0}, .ok = true};
}

Bmp888 new_bmp(int32_t width, int32_t height) {
	uint32_t image_size = width * height * 3;
	Bmp888 bmp = new_base_bmp();

	// Initialize header
	bmp.header.signature[0] = 'B';
	bmp.header.signature[1] = 'M';
	bmp.header.size = 54 + image_size; // 54 bytes for header + size of image data
	bmp.header.reserved0 = 0;
	bmp.header.reserved1 = 0;
	bmp.header.pixel_data_offset = 54;

	// Initialize info
	bmp.info.byte_size = 40; // The size of the Info header
	bmp.info.width = width;
	bmp.info.height = height;
	bmp.info.color_plane_count = 1; // Typically set to 1
	bmp.info.color_depth = 8 * 3; // 24 bits per pixel
	bmp.info.compress_method = 0; // No compression
	bmp.info.raw_bitmap_size = image_size;
	bmp.info.horizonal_resolution = 0;
	bmp.info.vertical_resolution = 0;
	bmp.info.table_entries = 0; // No color table for 24-bit BMP
	bmp.info.important_colors = 0; // All colors are important

	bmp.pixel_data = new_veccol888(image_size);

	return bmp;
}

void free_bmp(Bmp888 *bmp) {
	free(bmp->pixel_data.data);
}

Col888 *px_ptr(Bmp888 *bmp, int32_t x, int32_t y) {
	int32_t index = x * bmp->info.width + y;
	if (x >= 0 && x < bmp->info.width && y >= 0 && y < bmp->info.height) {
		return &bmp->pixel_data.data[index];
	} else {
		fprintf(stderr, "Error: Pixel coordinates (%d, %d) are out of bounds.\n", x, y);
		return &bmp->black_hole;
	}
}

bool save_bmp(Bmp888 *bmp, char *filename) {
	if (bmp == NULL || filename == NULL) { return false; }

	FILE *file = fopen(filename, "wb");
	if (file == NULL) { return false; }

	// Write the header
	fwrite(&bmp->header, sizeof(Header), 1, file);

	// Write the info
	fwrite(&bmp->info, sizeof(Info), 1, file);

	// Write the pixel data
	size_t pixel_data_size = bmp->info.raw_bitmap_size;
	fwrite(bmp->pixel_data.data, 3, pixel_data_size, file);

	fclose(file);
	return true;
}

Bmp888 load_bmp(const char *filename) {
	if (filename == NULL) { goto null_error; }

	FILE *file = fopen(filename, "rb");
	if (file == NULL) { goto null_error; }

	// Read the header
	Bmp888 bmp = new_base_bmp();
	if (fread(&bmp.header, sizeof(Header), 1, file) != 1) { goto file_error; }

	// Read the info
	if (fread(&bmp.info, sizeof(Info), 1, file) != 1) { goto file_error; }

	// Allocate memory for pixel data
	bmp.pixel_data.data = malloc(bmp.info.raw_bitmap_size * sizeof(Col888));
	if (bmp.pixel_data.data == NULL) { goto file_error; }

	// Go to the start of the pixel data
	fseek(file, bmp.header.pixel_data_offset, SEEK_SET);

	// Read the pixel data
	if (fread(bmp.pixel_data.data, 1, bmp.info.raw_bitmap_size, file)
		!= bmp.info.raw_bitmap_size) {
		goto file_error;
	}

	fclose(file);
	return bmp;
file_error:
	fclose(file);
null_error:
	return (Bmp888) { .ok = false };
}
