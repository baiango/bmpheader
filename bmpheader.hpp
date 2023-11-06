#include <iostream>
#include <string>
#include <fstream>
#include <vector>


// You can set the pixel by gray, Col8, or r, g, b, and a uint8_t with set_pixel(),
// or set the color cursor position by set_data_cursor_position() or xy() and chain it with r(), g(), b(), or a(),
// or you can set it by reference from "*px_ptr(x, y)".
//
// The Col8 is a union. To set the color with .rgba, use this format "0xrrggbbaa".
// You can do it with .r, .g, .b, and .a as well.


class Bmp {
public:
	// Tell the compiler do not pad the struct with "#pragma pack(push, 1)"!
#pragma pack(push, 1)
	struct Header {
		// The reason not to use std::string is,
		// it add extra stuff to the header, so it will corrupt the header
		char signature[2] = {'B', 'M'};
		uint32_t size = 512 * 512 + 54;
		uint16_t reserved0 = 0;
		uint16_t reserved1 = 0;
		uint32_t pixel_data_offset = 54;

		struct Info {
			uint32_t byte_size = 40;
			int32_t width = 512;
			int32_t height = 512;
			uint16_t color_plane_count = 1;
			// Most decoder can't read 3 channels, and it's buggy
			// So I made it 4 channels instead
			uint16_t color_depth = 8 * 3;
			uint32_t compress_method = 0;
			uint32_t raw_bitmap_size = 0;
			int32_t horizonal_resolution = 0;
			int32_t vertical_resolution = 0;
			uint32_t table_entries = 0;
			uint32_t important_colors = 0;
		};
	};
#pragma pack(pop)

	// The reason it's gray is cos easier to debug your own software
	// You must use clear_pixel() to set it black
	union Col8 {
		struct {
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
		struct {
			uint8_t rgb[3];
			uint8_t a;
		};
		uint32_t rgba = 0x202020ff;
	};

	const char BM_signature[2] = {'B', 'M'};

	Header header;
	Header::Info info;
	std::vector<std::vector<Col8>> pixels;

	int32_t data_cursor_x = 0;
	int32_t data_cursor_y = 0;

	Bmp(int32_t new_width, int32_t new_height) {
		resize(new_width, new_height);
	}

	void resize(int32_t new_width, int32_t new_height) {
		info.width = new_width;
		info.height = new_height;
		header.size = get_width() * get_height() + 54;

		pixels.resize(get_width());
		for (int32_t x = 0; x < get_width(); x++) {
			pixels[x].resize(get_height());
			for (int32_t y = 0; y < get_height(); y++) {
				set_pixel(x, y, 0x20);
			}
		}
	}

	int32_t get_width() { return info.width; }
	int32_t get_height() { return info.height; }
	Col8 get_pixel(int32_t x, int32_t y) { return pixels[x][y]; }
	Col8 *get_pixel_reference(int32_t x, int32_t y) { return &pixels[x][y]; }
	Col8 *px_ptr(int32_t x, int32_t y) { return get_pixel_reference(x, y); }

	void set_pixel(int32_t x, int32_t y, uint8_t gray) {
		pixels[x][y] = Col8{gray, gray, gray};
	}

	void set_pixel(int32_t x, int32_t y, Col8 col) {
		pixels[x][y] = col;
	}

	void set_pixel(
		int32_t x, int32_t y,
		uint8_t red, uint8_t green, uint8_t blue
	) {
		pixels[x][y] = {red, green, blue, pixels[x][y].a};
	}

	void set_pixel(
		int32_t x, int32_t y,
		uint8_t red, uint8_t green, uint8_t blue,
		uint8_t alpha
	) {
		pixels[x][y] = {red, green, blue, alpha};
	}

	Bmp &set_data_cursor_position(int32_t new_x, int32_t new_y) {
		data_cursor_x = new_x;
		data_cursor_y = new_y;
		return *this;
	}

	Bmp &xy(int32_t x, int32_t y) {
		return set_data_cursor_position(x, y);
	}

	Bmp &r(uint8_t new_red) {
		pixels[data_cursor_x][data_cursor_y].r = new_red;
		return *this;
	}

	Bmp &g(uint8_t new_green) {
		pixels[data_cursor_x][data_cursor_y].g = new_green;
		return *this;
	}

	Bmp &b(uint8_t new_blue) {
		pixels[data_cursor_x][data_cursor_y].b = new_blue;
		return *this;
	}

	Bmp &a(uint8_t new_alpha) {
		pixels[data_cursor_x][data_cursor_y].a = new_alpha;
		return *this;
	}

	void clear_pixel() {
		for (int32_t x = 0; x < get_width(); x++)
			for (int32_t y = 0; y < get_height(); y++)
				pixels[x][y] = {0, 0, 0};
	}

	bool read(std::string file_name) {
		// IFStream: Input File Stream
		std::ifstream file(file_name, std::ios::binary);

		if (not file) {
			std::cerr << "Failed to open the file." << std::endl;
			return false;
		}

		file.read((char *)&header, 14);
		file.read((char *)&info, 40);

		if (header.signature[0] != BM_signature[0]
			&& header.signature[1] != BM_signature[1]) {
			std::cerr << "Not a BMP file." << std::endl;
			return false;
		}

		if (info.color_depth != 8 * 3) {
			std::cerr << "Not a 24-bit or RGB BMP file." << std::endl;
			return false;
		}

		if (info.compress_method) {
			std::cerr << "Not an uncompressed BMP file." << std::endl;
			return false;
		}

		resize(get_width(), get_height());

		file.seekg(header.pixel_data_offset);
		for (int32_t x = 0; x < get_width(); x++)
			for (int32_t y = 0; y < get_height(); y++)
				file.read((char *)px_ptr(x, y), info.color_depth / 8);

		file.close();
		return true;
	}

	void save_sample(std::string file_name = "sample.bmp") {
		for (int32_t x = 0; x < get_width(); x++)
			for (int32_t y = 0; y < get_height(); y++)
				*px_ptr(x, y) = {uint8_t(x + y), 50, uint8_t(y * 0.3)};

		save(file_name);
	}

	void save(std::string file_name) {
		// OFStream: Output File Stream
		std::ofstream file(file_name, std::ios::binary);

		file.write((char *)&header, 14);
		file.write((char *)&info, 40);

		int32_t row_size = get_width() * info.color_depth / 8;
		int32_t row_padding_size = (4 - (row_size % 4)) % 4;
		for (int32_t x = 0; x < get_width(); x++) {
			for (int32_t y = 0; y < get_height(); y++) {
				file.write((char *)px_ptr(x, y), 3);
			}
			file.write("\0\0\0\0", row_padding_size);
		}

		file.close();
	}
};
