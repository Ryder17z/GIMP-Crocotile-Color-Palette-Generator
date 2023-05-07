/// <summary>
/// Creates a JSON file and writes header data. File will be compatible with Crocotile3d and any other software supporting rgb in json format, i.e: [{"r":0,"g":128,"b":255},{"r":200,"g":20,"b":220}]
/// </summary>
/// <param name="fname">Name of file, including file extension</param>
/// <returns>The output file stream</returns>
std::ofstream JSON_WriteBOF(std::string& fname)
{
	std::ofstream jsonFile;
	jsonFile.open(JSON_FILENAME, std::ofstream::out | std::ofstream::trunc);
	
	if (!jsonFile.is_open()) { APP_EXIT_IO_FAIL(jsonFname); }
	jsonFile << "["; // BOF

	return jsonFile;
}

/// <summary>
/// Writes a color to a JSON file
/// </summary>
/// <param name="fjson">The output file stream</param>
/// <param name="color_index">The index of the color to be written</param>
/// <param name="color_array">An array containing the RGB values [0-255] values of the color</param>
void JSON_WriteColor(std::ofstream& fjson, unsigned int color_index, std::array<uint8_t, 3> color_array)
{
	if (!fjson.is_open())
	{
		APP_EXIT_IO_FAIL(jsonFname);
	}

	uint8_t r = color_array[0];
	uint8_t g = color_array[1];
	uint8_t b = color_array[2];
	fjson << "{\"r\":" << static_cast<int>(r) << ",\"g\":" << static_cast<int>(g) << ",\"b\":" << static_cast<int>(b) << "}";
	if (color_index < total_colors-1) fjson << ","; // do not write a comma at the end.
}

/// <summary>
/// Writes an end-of-file marker to the JSON file
/// </summary>
/// <param name="fjson">The output file stream</param>
void JSON_WriteEOF(std::ofstream& fjson)
{
	fjson << "]"; // EOF
	fjson.close();
}
