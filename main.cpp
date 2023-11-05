#include "bmpheader.hpp"


void unit_test() {
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
}


int main() {
	unit_test();
}
