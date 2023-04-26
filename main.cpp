// Color palette generator script by github user Ryder17z

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <math.h>
#include <cstring>
#include <unordered_set>
#include <set>
#include <tuple>
#include "app_exit.hpp"

// filename.ext
#define OUTPUT_FILENAME "color_palette.gpl"
#define PALETTE_NAME "Extensive Palette"
#define HTML_FILENAME "preview.html"
#define JSON_FILENAME "color_palette.json.c3dpal"

// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)

void HSVtoRGB(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b)
{
	int i;
	float f, p, q, t;

	if (s == 0) {
		// achromatic (grey)
		r = g = b = v * 255.0;
		return;
	}

	h /= 60;			// sector 0 to 5
	i = floor(h);
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));

	switch (i) {
	case 0:
		r = v * 255.0;
		g = t * 255.0;
		b = p * 255.0;
		break;
	case 1:
		r = q * 255.0;
		g = v * 255.0;
		b = p * 255.0;
		break;
	case 2:
		r = p * 255.0;
		g = v * 255.0;
		b = t * 255.0;
		break;
	case 3:
		r = p * 255.0;
		g = q * 255.0;
		b = v * 255.0;
		break;
	case 4:
		r = t * 255.0;
		g = p * 255.0;
		b = v * 255.0;
		break;
	default:
		r = v * 255.0;
		g = p * 255.0;
		b = q * 255.0;
		break;
	}
}

int colorval_r = 0;
int colorval_g = 0;
int colorval_b = 0;
int total_colors = 0;

void reset_color()
{
    colorval_r = 0;
    colorval_g = 0;
    colorval_b = 0;
}

void WriteColor(std::ofstream &fp, int color_index)
{
	if(!fp.is_open()){ 	APP_EXIT_IO_FAIL(); }
	fp << colorval_r << " " << colorval_g << " " << colorval_b << " Index " << color_index << "\n";
}

void HTML_WriteColor(std::ofstream &fhtml, int color_index)
{
	if(!fhtml.is_open()){ APP_EXIT_IO_FAIL(); }

	if(color_index % 10 == 0 || color_index == 0) fhtml << "</div><div style=\"width:700px; height:38px;\">\n";
	fhtml << "<span style=\"display:inline; background-color: rgb(" << colorval_r << ", " << colorval_g << ", " << colorval_b << "); padding: 4px; border: 4px solid black;\">&nbsp;&nbsp;&nbsp;</span>\n";
}

void JSON_WriteColor(std::ofstream& fjson)
{
	if (!fjson.is_open())
	{
		APP_EXIT_IO_FAIL();
	}
	if (total_colors != 0) fjson << ",";
	fjson << "{\"r\":" << colorval_r << ",\"g\":" << colorval_g << ",\"b\":" << colorval_b << "}";
}

void PrintColor()
{
	std::cout << colorval_r << "\t" << colorval_g << "\t" << colorval_b << "\n";
}

std::set<std::tuple<int, int, int>> color_set; // Declare a set to keep track of generated colors

struct TupleHash {
    std::size_t operator()(const std::tuple<uint8_t, uint8_t, uint8_t>& t) const {
        auto hash1 = std::hash<uint8_t>{}(std::get<0>(t));
        auto hash2 = std::hash<uint8_t>{}(std::get<1>(t));
        auto hash3 = std::hash<uint8_t>{}(std::get<2>(t));
        return hash1 ^ hash2 ^ hash3;
    }
};

uint8_t RGB_Color[256 * 256 * 256][3];

float lerp(float a, float b, float t) {
	return (1.0 - t) * a + t * b;
}

void Component(std::ofstream& fp, std::ofstream& fhtml, std::ofstream& fjson, int num_hues, int num_saturations, int num_velocities, std::tuple<float, float, float> start_color, std::tuple<float, float, float> end_color) {
	int i, j, k;
	std::unordered_set<std::tuple<uint8_t, uint8_t, uint8_t>, TupleHash> used_colors;

	for (i = 0; i < num_hues; i++) {
		float hue = std::get<0>(start_color) + i * (std::get<0>(end_color) - std::get<0>(start_color)) / (num_hues - 1);
		for (j = 0; j < num_saturations; j++) {
			float saturation = std::get<1>(start_color) + j * (std::get<1>(end_color) - std::get<1>(start_color)) / (num_saturations - 1);
			for (k = 0; k < num_velocities; k++) {
				float value = std::get<2>(start_color) + k * (std::get<2>(end_color) - std::get<2>(start_color)) / (num_velocities - 1);

				// Convert HSV to RGB
				uint8_t r, g, b;
				HSVtoRGB(hue, saturation, value, r, g, b);

				// Check if the color is unique
				std::tuple<uint8_t, uint8_t, uint8_t> color_tuple(r, g, b);
				if (used_colors.count(color_tuple) == 0) {
					// Add the color to the set of used colors
					used_colors.insert(color_tuple);

					// Store the RGB values in the RGB_Color array
					RGB_Color[total_colors][0] = r;
					RGB_Color[total_colors][1] = g;
					RGB_Color[total_colors][2] = b;

					// Update the colorval variables for printing and JSON output
					colorval_r = r;
					colorval_g = g;
					colorval_b = b;

					// Write the color files
					WriteColor(fp, total_colors);
					HTML_WriteColor(fhtml, total_colors);
					JSON_WriteColor(fjson);

					// Print the color values to the console
					//PrintColor();

					// Increment the total number of colors generated
					total_colors++;
				}
			}
		}
	}

	// Print the generated colors
	for (i = 0; i < num_hues * num_saturations * num_velocities; i++) {
		printf("Color %d: RGB(%d, %d, %d)\n", i + 1, RGB_Color[i][0], RGB_Color[i][1], RGB_Color[i][2]);
	}
}

int main() {
	std::ofstream paletteFile;
	paletteFile.open(OUTPUT_FILENAME, std::ofstream::out | std::ofstream::trunc);

	std::ofstream htmlFile;
	htmlFile.open(HTML_FILENAME, std::ofstream::out | std::ofstream::trunc);

	std::ofstream jsonFile;
	jsonFile.open(JSON_FILENAME, std::ofstream::out | std::ofstream::trunc);
	
	if(!paletteFile.is_open()){ APP_EXIT_IO_FAIL(); }
	if(!htmlFile.is_open()){ APP_EXIT_IO_FAIL(); }
	if(!jsonFile.is_open()){ APP_EXIT_IO_FAIL(); }
	// continue

    // write file headers
    paletteFile << "GIMP Palette\n" << "Name: " << PALETTE_NAME << "\n" << "Columns: 4\n# Generated File\n";
	
	htmlFile << "<html><head><meta charset=\"UTF-8\"><title>Color Palette Preview</title></head>\n<body>\n<div style=\"position:absolute; left:20px; top:20px;\">";
	htmlFile << "GIMP Palette<br>" << "Name: " << PALETTE_NAME << "<br>" << "Columns: 4<br># Generated Using <a href=\"https://github.com/Ryder17z/GIMP-Crocotile-Color-Palette-Generator\">https://github.com/Ryder17z/GIMP-Crocotile-Color-Palette-Generator</a>\n</div><br><br><div style=\"position:absolute; left:20px; top:120px;\"><div>&nbsp;";

    jsonFile << "["; // BOF

	memset(RGB_Color, 0, sizeof(RGB_Color));

	// Example start color: dark red
	std::tuple<float, float, float> start_color = std::make_tuple(0.0, 1.0, 0.5);
	// Example end color: light blue
	std::tuple<float, float, float> end_color = std::make_tuple(210.0, 0.3, 0.8);

    Component(paletteFile, htmlFile, jsonFile, 3, 3, 3, start_color, end_color);

    jsonFile << "]"; // EOF
    jsonFile.close();

	std::cout << "Colors: " << total_colors << "\n";
    return 0;
}
