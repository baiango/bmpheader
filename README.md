# Bmpheader
The first library known that will display your algorithm(e.g. Perlin) in 24-bit RGB channels, and it doesn't require a Ph.D. in mathematics to understand it.  
Made in C11.

## Usage
You can set the color by reference from "*px_ptr(&bmp, x, y)".  

To set the color in Col888, use .r, .g, and .b or  
use this format "(Col888){r, g, b}".  
```C++
typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Col888;
```


```C
#include "bmpheader.h"


int32_t main() {
	{ // Lifetime
		AutoCleanBmp888 bmp = new_bmp(20, 20);
		uint8_t num = 0;
		for (uint32_t i = 0; i < 30; i++) {
			for (uint32_t j = 0; j < 30; j++) {
				Col888 *px = get_pixel(&bmp, i, j);
				*px = (Col888){num, i * 8, j * 16};
				printf("(%d, %d, %d) ", px->r, px->g, px->b);
				num++;
			}
		}

		save_bmp(&bmp, "test.bmp");
	} // Lifetime end
	{
		AutoCleanBmp888 bmp2 = load_bmp("test.bmp");
		save_bmp(&bmp2, "test2.bmp");
		AutoCleanBmp888 bmp3 = load_bmp("test3.bmp");
		printf("Is bmp2 vaild? %d\n", bmp2.ok);
		printf("Is bmp3 vaild? %d\n", bmp3.ok);
	}
	return 0;
}
```
