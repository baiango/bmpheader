# Bmpheader
The first library known that will display your algorithm(e.g. Perlin) in 8-bit RGBA channels, and it doesn't require a Ph.D. in mathematics to understand it.  
Made in C++14. It might have bugs, so it's best to use 1:1 ratio to draw your image.  

## Usage
You can set the pixel by gray, Col8, or r, g, b, and a uint8_t with set_pixel(),  
or set the color cursor position by set_data_cursor_position() or xy() and chain it with r(), g(), b(), or a(),  
or you can set it by reference from "*px_ptr(x, y)".  

The Col8 is a union. To set the color with .rgba, use this format "0xrrggbbaa".  
You can do it with .r, .g, .b, and .a as well.
```C++
class Bmp {
public:
	union Col8 {
		struct {
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
		uint32_t rgba = 0x202020ff;
	};

}
```


```C++
#include "bmpheader.hpp"

void md_example() {
	using namespace BmpHpp;

	for (int32_t count = 20; count <= 45; count += 1) {
		Bmp bmp(count, count / 2);

		for (int32_t x = 10; x < std::min(30, bmp.get_width()); x++)
			for (int32_t y = 10; y < std::min(30, bmp.get_height()); y++)
				bmp.xy(x, y).get_value()
				->r(x)
				->g(y * 10)
				->b(x * 30);

		bmp.save(std::to_string(count) + ".bmp");
	}
}
```
