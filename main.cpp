#include "bmpheader.hpp"


void unit_test() {
	using namespace BmpHpp;
	{
		Bmp bmp(512, 512);

		bmp.save_sample();

		bmp.clear_pixel();
		if (not bmp.read("sample.bmp"))
			std::cerr << "Reading sample.bmp failed!" << std::endl;
		bmp.save("read.bmp");
	}

	for (int32_t count = 20; count <= 45; count += 1) {
		Bmp bmp(count, count / 2);

		for (int32_t x = 10; x < std::min(30, bmp.get_width()); x++)
			for (int32_t y = 10; y < std::min(30, bmp.get_height()); y++)
				bmp.xy(x, y)
				.r(x)
				.g(y * 10)
				.b(x * 30);

		bmp.save(std::to_string(count) + ".bmp");
	}

	Bmp bmp(256, 256);

	bmp.read("sample.bmp");
	bmp.save("aaa.bmp");
}

/*
clang++ -g -O1 -D LIBFUZZER -fsanitize=fuzzer,address main.cpp
set ASAN_OPTIONS=detect_container_overflow=0
a
*/
extern "C" int LLVMFuzzerTestOneInput(const uint8_t Data[], size_t Size) {
	using namespace BmpHpp;

	if (Size < 4 * 4) {
		return 0;
	}
	int32_t x, y;
	memcpy(&x, Data, 4);
	memcpy(&y, &Data[4], 4);

	uint8_t r, g, b;
	memcpy(&r, &Data[8], 1);
	memcpy(&g, &Data[9], 1);
	memcpy(&b, &Data[10], 1);

	std::string file_name((char *)&Data[12], 4);

	Bmp bmp(512, 512);

	Result<Bmp::Col8 *, bool> pixel = bmp.px_ptr(x, y);
	if (pixel.is_ok())
		*(pixel.get_value()) = {r, g, b};

	bmp.save(file_name + ".bmp");
	return 0;
}


#ifndef LIBFUZZER
int main() {
	unit_test();
}
#endif
