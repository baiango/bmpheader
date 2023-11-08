#include "bmpheader.hpp"
#include <array>


/*
// It trigger on std::string or std::to_string(), so don't use the ASan
//clang++ -g -O1 -D LIBFUZZER -fsanitize=fuzzer,address -fno-omit-frame-pointer main.cpp
//set ASAN_OPTIONS=detect_container_overflow=0
clang++ -g -O1 -D LIBFUZZER -fsanitize=fuzzer -fno-omit-frame-pointer main.cpp
a
*/
uint64_t iteration_counter = 0;
extern "C" int LLVMFuzzerTestOneInput(const uint8_t Data[], size_t Size) {
	if (Size < 4 * 4) {
		return 0;
	}
	using namespace BmpHpp;

	Img24 bmp(512, 512);

	// Check that fuzzer is working
	for (int32_t x = 10; x < std::min(30, bmp.get_width()); x++) {
		for (int32_t y = 10; y < std::min(30, bmp.get_height()); y++) {
			Result<Img24 *, bool> pixel = bmp.xy(x, y);
			if (pixel.is_ok())
				pixel.get_value()
				->r(x)
				->g(y * 10)
				->b(x * 30);
		}
	}

	int32_t x, y;
	uint8_t r, g, b;

	for (uint32_t i = 0; i < 1000; i++) {
		size_t data_index = i * 1;
		if (data_index + 16 > Size)
			continue;

		memcpy(&x, &Data[0 + data_index], 4);
		memcpy(&y, &Data[4 + data_index], 4);

		memcpy(&r, &Data[8 + data_index], 1);
		memcpy(&g, &Data[9 + data_index], 1);
		memcpy(&b, &Data[10 + data_index], 1);

		x = (uint32_t)x % bmp.get_width();
		y	= (uint32_t)y % bmp.get_height();
		Result<Col888 *, bool> pixel = bmp.px_ptr(x, y);
		if (pixel.is_ok())
			*(pixel.get_value()) = {r, g, b};
	}
	bmp.save(std::to_string(iteration_counter) + ".bmp");
	iteration_counter++;
	return 0;
}

void unit_test() {
	using namespace BmpHpp;
	{
		Img24 bmp(512, 512);

		bmp.save_sample();

		bmp.clear_pixel();
		if (not bmp.read("sample.bmp"))
			std::cerr << "Reading sample.bmp failed!" << std::endl;
		bmp.save("read.bmp");
	}
	{
		Img24 bmp;

		bmp.read("sample.bmp");
		bmp.save("aaa.bmp");
	}
}

void md_example() {
	using namespace BmpHpp;

	for (int32_t count = 20; count <= 45; count += 1) {
		Img24 bmp(count, count / 2);

		for (int32_t x = 10; x < std::min(30, bmp.get_width()); x++)
			for (int32_t y = 10; y < std::min(30, bmp.get_height()); y++)
				bmp.xy(x, y).get_value()
				->r(x)
				->g(y * 10)
				->b(x * 30);

		bmp.save(std::to_string(count) + ".bmp");
	}
}

uint64_t hash(const char *key) {
	uint64_t hash = 2;
	uint64_t length = strlen(key);

	while (*key) {
		hash = hash * 0x72ed + *key + length;
		key++;
	}
	return hash;
}


void unit_test2() {
	using namespace BmpHpp;
	using std::array;

	array<array<uint64_t, 16>, 16> bucket;
	Heatmap heatmap;
	uint64_t num = 1000;

	// Hashmap here
	// Example: hash() % bucket_size
}

#ifndef LIBFUZZER
int main() {
	unit_test();
	md_example();
	//unit_test2();
}
#endif
