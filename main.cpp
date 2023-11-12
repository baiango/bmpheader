#include "bmpheader.hpp"
#include <array>


/* Code coverage
clang++ -fprofile-instr-generate -fcoverage-mapping main.cpp
a
llvm-profdata merge -sparse default.profraw -o default.profdata

// Detailed
llvm-cov show a.exe -instr-profile=default.profdata
// Summary
llvm-cov report a.exe -instr-profile=default.profdata
// HTML
mkdir coverage_report && llvm-cov show -format=html -o coverage_report -instr-profile=default.profdata a.exe
*/
void unit_test() {
	using namespace BmpHpp;
	{
		Img24 bmp(512, 512);

		bmp.save_sample();
		bmp.clear_pixel();
		bmp.read_sample();

		bmp.save("read.bmp");
	}
	{
		Img24 bmp;
		bmp.resize(32, 32);
		bmp.px_ptr(-1, -1).is_err();
		bmp.xy(-1, -1).get_value()->gray(255);
		bmp.r(255)->b(9)->g(1);
		bmp.gray(200);

		bmp.read("i_am_nullptr.bmp");
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


int main() {
	unit_test();
	md_example();
}
