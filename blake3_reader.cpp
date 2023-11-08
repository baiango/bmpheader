#include "blake3_reader.hpp"


// This is the alternate of the libfuzzer,
// which is way better than libfuzzer because it generates completely random data
// rather than targeted data
std::array<uint8_t, 4096> generate_random_numbers() {
	std::array<uint8_t, 4096> data{};

	size_t seed = rand();
	std::string command = "echo " + std::to_string(seed) + " | b3sum_windows_x64_bin.exe --no-names --raw > b3sum_result.txt";
	system(command.c_str());
	for (size_t i = 0; i < 128 - 1; i++) {
		seed = rand();
		std::string command = "echo " + std::to_string(seed) + " | b3sum_windows_x64_bin.exe --no-names --raw >> b3sum_result.txt";
		system(command.c_str());
	}

	std::ifstream file("b3sum_result.txt", std::ios::binary);
	if (!file) {
		std::cerr << "Unable to open b3sum_result.txt for reading." << std::endl;
		goto return_empty;
	}

	file.read((char *)data.data(), data.size());
	data[4095] = (uint8_t)rand();

return_empty:
	return data;
}