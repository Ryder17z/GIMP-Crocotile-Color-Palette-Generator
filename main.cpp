// Color palette generator script by github user Ryder17z

// Input / output:
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

// Containers :
#include <vector>
#include <array>
#include <tuple>
#include <unordered_set>

// Algorithms :
#include <algorithm>
#include <functional>
#include <regex>

// Math :
#include <cmath>
#include <math.h>
#include <iomanip>
#include <limits>

// Miscellaneous :
#include <cstring>
#include <sstream>

/*
constexpr double pi = 3.14159265358979323846;
constexpr double two_pi = 2.0 * pi;
constexpr double half_pi = 0.5 * pi;
constexpr double quarter_pi = 0.25 * pi;
constexpr double inv_pi = 1.0 / pi;
constexpr double inv_two_pi = 1.0 / two_pi;
constexpr double inv_half_pi = 1.0 / half_pi;
constexpr double inv_quarter_pi = 1.0 / quarter_pi;
constexpr double pi_squared = pi * pi;
constexpr double two_pi_squared = two_pi * two_pi;
constexpr double half_pi_squared = half_pi * half_pi;
constexpr double quarter_pi_squared = quarter_pi * quarter_pi;

#define M_PI pi
*/

bool file_exists(const std::string& filename)
{
	std::ifstream infile(filename);
	return infile.good();
}

std::string boolToString(bool value) {
	return value ? "true" : "false";
}

double midpoint(double a, double b) {
	return a + (b - a) / 2.0;
}

float fmin3(float a, float b, float c) {
	return std::min(a, std::min(b, c));
}
float fmax3(float a, float b, float c) {
	return std::max(a, std::max(b, c));
}
std::string doubleToString(double input) {
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2) << input; // set precision to 2 decimal places
	std::string output = oss.str();
	std::replace(output.begin(), output.end(), ',', '.'); // replace comma with dot
	return output;
}

// filename.ext
#define PALETTE_NAME "Extensive Palette"
#define HTML_FILENAME "preview.html"
#define JSON_FILENAME "color_palette.json.c3dpal"
#define GIMP_FILENAME "color_palette.gpl"

// global stuff
unsigned int total_colors = 0;
std::string htmlFname;
std::string jsonFname;
std::string gimpFname;

#include "app_exit.hpp"
#include "color_funcs.hpp"

#include "gimp.hpp"
#include "html.hpp"
#include "json.hpp"


std::tuple<double, double, double> WeightedPriority(const std::string& priorities) {
	double w_hue = 1.0, w_saturation = 1.0, w_value = 1.0;
	double w_dec = 0.3;
	for (auto c : priorities) {
		if (c == 'h')
		{
			w_saturation = std::max(0.0, w_saturation - w_dec);
			w_value = std::max(0.0, w_value - w_dec);
		}
		else if (c == 's')
		{
			w_hue = std::max(0.0, w_hue - w_dec);
			w_value = std::max(0.0, w_value - w_dec);
		}
		else if (c == 'v')
		{
			w_hue = std::max(0.0, w_hue - w_dec);
			w_saturation = std::max(0.0, w_saturation - w_dec);
		}
	}
	double total_weight = w_hue + w_saturation + w_value;
	return std::make_tuple(w_hue / total_weight, w_saturation / total_weight, w_value / total_weight);
}

// Example colors to set the gradient constraints: (0.0, 1.0, 1.0), (359.9, 0.0, 0.0);
std::vector<std::array<uint8_t, 3>> CreateGradient(std::tuple<double, double, double> startingHSV, std::tuple<double, double, double> endingHSV, std::tuple<uint8_t, uint8_t, uint8_t> numsteps)
{
	std::vector<std::array<uint8_t, 3>> ColorVecArrays;
	
	auto deltaHSV = GetdeltaHSV(startingHSV, endingHSV);
	
	auto incr_hue = std::get<0>(deltaHSV) / static_cast<double>(std::get<0>(numsteps)-1);
	auto incr_sat = std::get<1>(deltaHSV) / static_cast<double>(std::get<1>(numsteps)-1);
	auto incr_val = std::get<2>(deltaHSV) / static_cast<double>(std::get<2>(numsteps)-1);

	double hsvshiftby[] = { incr_hue, incr_sat, incr_val };
	
	uint8_t current_red = 0, current_green = 0, current_blue = 0;
	double iter_hue = 0.0f, iter_sat = 0.0f, iter_val = 0.0f;
	double tolerance = 0.01f;
	
 	printf("startingHSV: (%f, %f, %f)\n", std::get<0>(startingHSV), std::get<1>(startingHSV), std::get<2>(startingHSV));
	printf("endingHSV: (%f, %f, %f)\n", std::get<0>(endingHSV), std::get<1>(endingHSV), std::get<2>(endingHSV));
	printf("deltaHSV: (%f, %f, %f)\n", std::get<0>(deltaHSV), std::get<1>(deltaHSV), std::get<2>(deltaHSV));
	
	double startHSV[] = { std::get<0>(startingHSV), std::get<1>(startingHSV), std::get<2>(startingHSV) };
	double endHSV[] = { std::get<0>(endingHSV), std::get<1>(endingHSV), std::get<2>(endingHSV) };

	// Swap values if necessary
	for (int i = 0; i < 3; i++) {
		if (endHSV[i] < startHSV[i]) {
			std::swap(endHSV[i], startHSV[i]);
		}
	}

	printf("startHSV: (%f, %f, %f)\n", startHSV[0], startHSV[1], startHSV[2]);
	printf("endHSV: (%f, %f, %f)\n",  endHSV[0], endHSV[1], endHSV[2]);

	auto inputHSV = std::make_tuple(startHSV[0], startHSV[1], startHSV[2]);
	
	iter_hue = startHSV[0];
	while (true) {
		iter_sat = startHSV[1];
		while (true) {
			iter_val = startHSV[2];
			while (true) {
				if (total_colors == 0) {
					auto inputRGB = HSV_to_RGB(inputHSV);
					current_red = std::get<0>(inputRGB);
					current_green = std::get<1>(inputRGB);
					current_blue = std::get<2>(inputRGB);
				}

				auto [color_red, color_green, color_blue] = CreateColor(current_red, current_green, current_blue, iter_hue, iter_sat, iter_val);

				printf("Current color: %d, %d, %d\n", current_red, current_green, current_blue);
				printf("New color: %d, %d, %d\n", color_red, color_green, color_blue);

				current_red = color_red;
				current_green = color_green;
				current_blue = color_blue;

				total_colors++;

				ColorVecArrays.push_back({ current_red, current_green, current_blue });

				iter_val += hsvshiftby[2];
				if (iter_val > endHSV[2] + tolerance)
					break;
			}
			iter_sat += hsvshiftby[1];
			if (iter_sat > endHSV[1] + tolerance)
				break;
		}
		iter_hue += hsvshiftby[0];
		if (iter_hue > endHSV[0] + tolerance)
			break;
	}

	return ColorVecArrays;
}

void remove_duplicates(std::vector<std::array<uint8_t, 3>>* vec_ptr)
{
	// Check for null pointer
	if (vec_ptr == nullptr) {
		return;
	}

	std::vector<std::array<uint8_t, 3>> result;

	// Iterate over the input vector and insert each unique element into the result vector.
	for (const auto& arr : *vec_ptr)
	{
		if (std::find(result.begin(), result.end(), arr) == result.end())
		{
			result.push_back(arr);
		}
	}

	// Copy the unique elements back into the input vector
	*vec_ptr = result;
}

int main() {
	double startHSV[] = { 40.0f, 0.2f, 0.3f };
	double endHSV[] = { 300.0f, 0.7f, 0.8f };

	std::tuple<double, double, double> startingHSV = std::make_tuple(startHSV[0], startHSV[1], startHSV[2]);
	std::tuple<double, double, double> endingHSV = std::make_tuple(endHSV[0], endHSV[1], endHSV[2]);

	std::tuple<uint8_t, uint8_t, uint8_t> numsteps = std::make_tuple(6, 3, 4);

	auto colors = CreateGradient(startingHSV, endingHSV, numsteps);

	std::tuple<uint8_t, uint8_t, uint8_t> start_color = HSV_to_RGB(startingHSV);
	std::tuple<uint8_t, uint8_t, uint8_t> end_color = HSV_to_RGB(endingHSV);
	
	// Remove duplicates from the vector
	//remove_duplicates(&colors);

	htmlFname = HTML_FILENAME;
	jsonFname = JSON_FILENAME;
	gimpFname = GIMP_FILENAME;

	auto fhtml = HTML_WriteBOF(htmlFname, start_color, end_color);
	auto fjson = JSON_WriteBOF(jsonFname);
	auto fgimp = GIMP_WriteBOF(gimpFname);

	int coloridx=0;

	// Loop through the vector and print each std::array
	for (const auto& color : colors) {

		// Get the values of the array
		uint8_t red = static_cast<uint8_t>(std::round(color[0]));
		uint8_t green = static_cast<uint8_t>(std::round(color[1]));
		uint8_t blue = static_cast<uint8_t>(std::round(color[2]));

		HTML_WriteColor(fhtml, coloridx, color);
		JSON_WriteColor(fjson, coloridx, color);
		GIMP_WriteColor(fgimp, coloridx, color);
		coloridx++;

		// Print the values
		std::cout << "Red: " << static_cast<int>(red) << ", " << "Green: " << static_cast<int>(green) << ", " << "Blue: " << static_cast<int>(blue) << std::endl;
	}

	HTML_WriteEOF(fhtml);
	JSON_WriteEOF(fjson);
	GIMP_WriteEOF(fgimp);
	
	printf("\n\tSucess!\n\n");

	return 0;
}

int main2() {
	
	// Define the sorting priority
	std::regex valid_regex("^[hsv]+$"); // regex pattern to match "h", "s", or "v"

	std::string priority;

	std::cout << "Enter a priority string (h for hue, s for saturation, v for value): ";
	std::getline(std::cin, priority);

	while (!std::regex_match(priority, valid_regex)) {
		std::cout << "Invalid input. Please enter a string containing only 'h', 's', or 'v': ";
		std::getline(std::cin, priority);
	}

    return 0;
}
