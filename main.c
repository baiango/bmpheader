#include "bmpheader.h"


int32_t main() {
	{ // Lifetime
		AutoCleanBmp888 bmp = new_bmp(20, 20);
		uint8_t num = 0;
		for (uint32_t i = 0; i < 30; i++) {
			for (uint32_t j = 0; j < 30; j++) {
				Col888 *px = px_ptr(&bmp, i, j);
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
