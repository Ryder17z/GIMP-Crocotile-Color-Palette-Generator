// Omitting the <summary> tag is an intentinal hack to get linebreaks working.
// As the <remarks> feature appears to be broken and the content cannot be shown
// without messing with a visual-studio specific project, this will have to do for the time being
//--------------------------------------------------------------------------------------------------


/// Creates a color palette file and writes header data. File will be compatible with Gimp and any other software supporting rgb in gpl format.
/// Example.gpl file contents:
/// GIMP Palette
/// Name: Nature
/// Columns : 4
/// # your comment here
/// 34 139 34 Index 0
/// 135 206 235 Index 1
/// 245, 215, 0 Index 2
/// 139, 0, 0 Index 3
/// </summary>
/// <param name="fname">Name of file, including file extension</param>
/// <returns>The output file stream</returns>
/// 
std::ofstream GIMP_WriteBOF(std::string& fname)
{
	std::ofstream fgpl;
	fgpl.open(fname, std::ofstream::out | std::ofstream::trunc);

	if (!fgpl.is_open())
	{
		APP_EXIT_IO_FAIL(gimpFname);
	}
	
	fgpl << "GIMP Palette\n" << "Name: " << fname << "\n" << "Columns: 4\n# Generated File\n";
	return fgpl;
}

/// <summary>
/// Writes a color to a Gimp style .gpl file
/// </summary>
/// <param name="fgpl">The output file stream</param>
/// <param name="color_index">The index of the color to be written</param>
/// <param name="color_array">An array containing the RGB values [0-255] values of the color</param>
void GIMP_WriteColor(std::ofstream& fgpl, unsigned int color_index, std::array<uint8_t, 3> color_array)
{
	if (!fgpl.is_open())
	{
		APP_EXIT_IO_FAIL(gimpFname);
	}

	uint8_t r = color_array[0];
	uint8_t g = color_array[1];
	uint8_t b = color_array[2];

	fgpl << static_cast<int>(r) << " " << static_cast<int>(g) << " " << static_cast<int>(b) << " Index " << color_index << "\n";
}

/// <summary>
/// Writes an end-of-file marker to the Gimp style .gpl file
/// </summary>
/// <param name="fgpl">The output file stream</param>
void GIMP_WriteEOF(std::ofstream& fgpl)
{
	// Exists only for sake of consistency - compare to json.hpp or html.hpp
	
	if (!fgpl.is_open()) { APP_EXIT_IO_FAIL(gimpFname); }

	fgpl.close();
}