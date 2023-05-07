/// <summary>
/// Creates a HTML file and writes header data
/// </summary>
/// <param name="fname">Name of file, including file extension</param>
/// <param name="start_color">The start color of the gradient</param>
/// <param name="end_color">The end color of the gradient</param>
/// <returns>The output file stream</returns>
std::ofstream HTML_WriteBOF(std::string& fname, std::tuple<uint8_t, uint8_t, uint8_t> start_color, std::tuple<uint8_t, uint8_t, uint8_t> end_color)
{
	std::ofstream htmlFile;
	htmlFile.open(HTML_FILENAME, std::ofstream::out | std::ofstream::trunc);

	if (!htmlFile.is_open()) { APP_EXIT_IO_FAIL(fname); }

	auto [c1r, c1g, c1b] = start_color;
	auto [c2r, c2g, c2b] = end_color;

	auto [ min_h, max_h, min_s, max_s, min_v, max_v ] = extract_min_max_hsv(start_color, end_color);

	auto midHue = midpoint(min_h, max_h);

	auto [c3r, c3g, c3b] = HSV_to_RGB(std::make_tuple(min_h, 1.0, 1.0));
	auto [c4r, c4g, c4b] = HSV_to_RGB(std::make_tuple(max_h, 1.0, 1.0));

	auto [c5r, c5g, c5b] = HSV_to_RGB(std::make_tuple(midHue, min_s, 1.0));
	auto [c6r, c6g, c6b] = HSV_to_RGB(std::make_tuple(midHue, max_s, 1.0));

	auto [c7r, c7g, c7b] = HSV_to_RGB(std::make_tuple(midHue, 1.0, min_v));
	auto [c8r, c8g, c8b] = HSV_to_RGB(std::make_tuple(midHue, 1.0, max_v));

	htmlFile << "<!DOCTYPE html><html lang=\"en\"><head><meta charset = \"utf-8\">";
	htmlFile << "<title>Color Palette Preview</title>";
	htmlFile << "<style> body{ background: -webkit-gradient(linear, left top, left bottom, from(#dadada), to(#555)) fixed;";
	htmlFile << "background: -moz-linear-gradient(top, #dadada, #555) fixed; background: -ms-linear-gradient(top, #dadada, #555) fixed;";
	htmlFile << "background: -o-linear-gradient(top, #dadada, #555) fixed; background: linear-gradient(to bottom, #dadada, #555) fixed;";
	htmlFile << "background-color: #ccc; } #title{ position: absolute; left: 20px; top: 20px; } </style> </head> <body>";
	htmlFile << "<div style=\"position:absolute; left:20px; top:20px; \">GIMP Palette<br>" << "Name: " << fname << "<br>" << "Colors: " << total_colors << "<br>";
	htmlFile << "# Generated Using <a href=\"https://github.com/Ryder17z/GIMP-Crocotile-Color-Palette-Generator\">https://github.com/Ryder17z/GIMP-Crocotile-Color-Palette-Generator</a>\n</div>";
	htmlFile << "<br><br><div style=\"position:relative; left:0px; top:20px;\">";

	htmlFile << "<style>table{ border - collapse: collapse; margin - bottom: 20px; }";
	htmlFile << "td{padding: 10px;} tr:not(:last - child) { margin - bottom: 10px; }</style>";

	htmlFile << "<div style=\"position:relative; left:20px; top: 40px;\"><table><tr><td align=\"right\">Gradient between&nbsp;&nbsp;&nbsp;\n";

	htmlFile << "<span style=\"display:inline; background-color: rgb(" << std::to_string(static_cast<uint8_t>(c1r)) << ", "
		<< std::to_string(static_cast<uint8_t>(c1g)) << ", "
		<< std::to_string(static_cast<uint8_t>(c1b))
		<< "); padding: 4px; border: 4px solid black;\">&nbsp;&nbsp;&nbsp;</span>\n";

	htmlFile << "&nbsp;&nbsp;&nbsp;and&nbsp;&nbsp;&nbsp;\n";

	htmlFile << "<span style=\"display:inline; background-color: rgb(" << std::to_string(static_cast<uint8_t>(c2r)) << ", "
		<< std::to_string(static_cast<uint8_t>(c2g)) << ", "
		<< std::to_string(static_cast<uint8_t>(c2b))
		<< "); padding: 4px; border: 4px solid black;\">&nbsp;&nbsp;&nbsp;</span></td></tr>\n";

	htmlFile << "<tr><td align=\"right\">Hue between&nbsp;&nbsp;&nbsp;\n";

	htmlFile << "<span style=\"display:inline; background-color: rgb(" << std::to_string(static_cast<uint8_t>(c3r)) << ", "
		<< std::to_string(static_cast<uint8_t>(c3g)) << ", "
		<< std::to_string(static_cast<uint8_t>(c3b))
		<< "); padding: 4px; border: 4px solid black;\">&nbsp;&nbsp;&nbsp;</span>\n";

	htmlFile << "&nbsp;&nbsp;&nbsp;and&nbsp;&nbsp;&nbsp;\n";

	htmlFile << "<span style=\"display:inline; background-color: rgb(" << std::to_string(static_cast<uint8_t>(c4r)) << ", "
		<< std::to_string(static_cast<uint8_t>(c4g)) << ", "
		<< std::to_string(static_cast<uint8_t>(c4b))
		<< "); padding: 4px; border: 4px solid black;\">&nbsp;&nbsp;&nbsp;</span></td></tr>\n";

	htmlFile << "<tr><td align=\"right\">Saturation between&nbsp;&nbsp;&nbsp;\n";

	htmlFile << "<span style=\"display:inline; background-color: rgb(" << std::to_string(static_cast<uint8_t>(c5r)) << ", "
		<< std::to_string(static_cast<uint8_t>(c5g)) << ", "
		<< std::to_string(static_cast<uint8_t>(c5b))
		<< "); padding: 4px; border: 4px solid black;\">&nbsp;&nbsp;&nbsp;</span>\n";

	htmlFile << "&nbsp;&nbsp;&nbsp;and&nbsp;&nbsp;&nbsp;\n";

	htmlFile << "<span style=\"display:inline; background-color: rgb(" << std::to_string(static_cast<uint8_t>(c6r)) << ", "
		<< std::to_string(static_cast<uint8_t>(c6g)) << ", "
		<< std::to_string(static_cast<uint8_t>(c6b))
		<< "); padding: 4px; border: 4px solid black;\">&nbsp;&nbsp;&nbsp;</span></td></tr>\n";

	htmlFile << "<tr><td align=\"right\">Value between&nbsp;&nbsp;&nbsp;\n";

	htmlFile << "<span style=\"display:inline; background-color: rgb(" << std::to_string(static_cast<uint8_t>(c7r)) << ", "
		<< std::to_string(static_cast<uint8_t>(c7g)) << ", "
		<< std::to_string(static_cast<uint8_t>(c7b))
		<< "); padding: 4px; border: 4px solid black;\">&nbsp;&nbsp;&nbsp;</span>\n";

	htmlFile << "&nbsp;&nbsp;&nbsp;and&nbsp;&nbsp;&nbsp;\n";

	htmlFile << "<span style=\"display:inline; background-color: rgb(" << std::to_string(static_cast<uint8_t>(c8r)) << ", "
		<< std::to_string(static_cast<uint8_t>(c8g)) << ", "
		<< std::to_string(static_cast<uint8_t>(c8b))
		<< "); padding: 4px; border: 4px solid black;\">&nbsp;&nbsp;&nbsp;</span></td></tr>\n";

	htmlFile << "<tr><td align=\"right\">HSV min: " << doubleToString(min_h) << ", " << doubleToString(min_s) << ", " << doubleToString(min_v) << "<br>";
	htmlFile << "HSV max: " << doubleToString(max_h) << ", " << doubleToString(max_s) << ", " << doubleToString(max_v) << "</td></tr>";
	htmlFile << "</table></div>";

	htmlFile << "<div style=\"position:relative; left:20px; top: 20px;\">\n";
	return htmlFile;
}

/// <summary>
/// Writes a color to a HTML file
/// </summary>
/// <param name="fhtml">The output file stream</param>
/// <param name="color_index">The index of the color to be written</param>
/// <param name="color_array">An array containing the RGB values [0-255] values of the color</param>
void HTML_WriteColor(std::ofstream& fhtml, unsigned int color_index, std::array<uint8_t, 3> color_array)
{
	if (!fhtml.is_open()) { APP_EXIT_IO_FAIL(htmlFname); }
	//printf(" %i<>", color_index);
	if (color_index == 0) fhtml << "<div style=\"width:700px; height:38px;\">\n";
	if ((((color_index) % 10) == 0) && (color_index < total_colors - 1)) fhtml << "</div>\n" << "<div style=\"width:700px; height:38px;\">\n";
	//if ((color_index % 10) == 0) printf(" %i<mod>", color_index);, printf(" %i<mov>", color_index)

	fhtml << "<span style=\"display:inline; background-color: rgb(" << std::to_string(static_cast<uint8_t>(color_array[0])) << ", "
		<< std::to_string(static_cast<uint8_t>(color_array[1])) << ", "
		<< std::to_string(static_cast<uint8_t>(color_array[2]))
		<< "); padding: 4px; border: 4px solid black;\">&nbsp;&nbsp;&nbsp;</span>\n";

	if (color_index >= total_colors - 1) fhtml << "</div>\n";
}

/// <summary>
/// Writes an end-of-file marker to the HTML file
/// </summary>
/// <param name="fhtml">The output file stream</param>
void HTML_WriteEOF(std::ofstream& fhtml)
{
	if (!fhtml.is_open()) { APP_EXIT_IO_FAIL(htmlFname); }

	fhtml << "</div></div></div><br></body></html>\n";
	fhtml.close();
}