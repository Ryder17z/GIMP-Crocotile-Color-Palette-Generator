// Color palette generator script by github user Ryder17z

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>

// filename.ext
#define OUTPUT_FILENAME "color_palette.gpl"
#define PALETTE_NAME "Extensive Palette"
#define HTML_FILENAME "preview.html"
#define JSON_FILENAME "color_palette.json.c3dpal"

// Pause methods
#ifdef _WIN32
// For windows
void waitForAnyKey() {
    system("pause");
}
#elif __linux__
// For linux
void waitForAnyKey() {
    std::cout << "Press any key to continue...";
    system("read -s -N 1"); // Continues when pressed a key like windows
}
#endif

void APP_EXIT_IO_FAIL()
{
	printf ("Error Writing to output.file\nPlease verify file/folder R/W permissions\nThe application will now exit.\n\n");
	waitForAnyKey();
	exit(0); // RW failure
}

float fmin3(float a, float b, float c) {
    return std::min(a, std::min(b, c));
}
float fmax3(float a, float b, float c) {
    return std::max(a, std::max(b, c));
}

// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)

void RGBtoHSV( float r, float g, float b, float &h, float &s, float &v )
{
	float min, max, delta;

	min = fmin3( r, g, b );
	max = fmax3( r, g, b );
	v = max;				// v

	delta = max - min;

	if( max != 0 )
		s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		s = 0;
		h = -1;
		return;
	}

	if( r == max )
		h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		h = 4 + ( r - g ) / delta;	// between magenta & cyan

	h *= 60;				// degrees
	if( h < 0 )
		h += 360;

}

void HSVtoRGB( float &r, float &g, float &b, float &h, float &s, float &v )
{
	int i;
	float f, p, q, t;

	if( s == 0 ) {
		// achromatic (grey)
		r = g = b = v;
		return;
	}

	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	switch( i ) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		default:		// case 5:
			r = v;
			g = p;
			b = q;
			break;
	}

}

int colorval_r = 3;
int colorval_g = 3;
int colorval_b = 3;
int total_colors = 0;

void reset_color()
{
    colorval_r = 3;
    colorval_g = 3;
    colorval_b = 3;
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

void JSON_WriteColor(std::ofstream &fjson, bool lastcolor)
{
	if(!fjson.is_open())
	{
		APP_EXIT_IO_FAIL();
	}
	fjson << "{\"r\":" << colorval_r << ",\"g\":" << colorval_g << ",\"b\":" << colorval_b << "}";
	if(!lastcolor) fjson << ",";
}

void PrintColor()
{
    std::cout << colorval_r << "\t" << colorval_g << "\t" << colorval_b << "\n";
}

void CreateColor(int _RED_COLOR, int _GREEN_COLOR, int _BLUE_COLOR, float __SHIFTHUE, float __SHIFTSAT, float __SHIFTVEL, std::ofstream &fp, int color_index, std::ofstream &fhtml, std::ofstream &fjson, bool lastcolor)
{
	float HUE, SAT, VEL, reds, greens, blues;

    RGBtoHSV( (float) _RED_COLOR/255.0, (float) _GREEN_COLOR/255.0, (float) _BLUE_COLOR/255.0, HUE, SAT, VEL );

    HUE += __SHIFTHUE;
    SAT *= __SHIFTSAT;
    VEL *= __SHIFTVEL;

    HSVtoRGB( reds, greens, blues, HUE, SAT, VEL );

    colorval_r = round(reds * 255.0); //!< Extract red
    colorval_g = round(greens * 255.0); //!< Extract green
    colorval_b = round(blues * 255.0); //!< Extract blue
    
    //std::cout << "[r]" << reds << "\t"<< "[g]" << greens << "\t"<< "[b]" << blues << "\n";
    //PrintColor();
    WriteColor(fp, color_index);
	HTML_WriteColor(fhtml, color_index);
	JSON_WriteColor(fjson, lastcolor);

	total_colors++;
}

int main()
{
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

    // ALL GRAY
	CreateColor( 3, 3, 3,  1.0, 1.0, 0.0, paletteFile, total_colors, htmlFile, jsonFile, false); // add black
	CreateColor( 25, 25, 25,  1.0, 1.0, 0.65, paletteFile, total_colors, htmlFile, jsonFile, false); // add darkest grey
    int Feed_r = 253;
    int Feed_g = 253;
    int Feed_b = 253;
	for(int VEL=1; VEL <= 10; VEL++)
	{
		int color_tmp[3] = { Feed_r, Feed_g, Feed_b };
		
		// skip certain known colors
		if(VEL == 9) continue;
		if(VEL == 8) continue;
		if(VEL == 5) continue;
		CreateColor( color_tmp[0], color_tmp[1], color_tmp[2],  1.0, 1.0, (double)(VEL/10.0), paletteFile, total_colors, htmlFile, jsonFile, false);
		
		// insert 2 extra colors
		if(VEL == 1)
		{
			double tmpvel = ( (25 * 1.2) + 3 ) / 255.0 ;
			CreateColor( color_tmp[0], color_tmp[1], color_tmp[2], 1.0, 1.0, tmpvel, paletteFile, total_colors, htmlFile, jsonFile, false);
			tmpvel = ( (25 * 1.5) + 3 ) / 255.0 ;
			CreateColor( color_tmp[0], color_tmp[1], color_tmp[2], 1.0, 1.0, tmpvel, paletteFile, total_colors, htmlFile, jsonFile, false);
		}
	}
	reset_color();
	
    // Any color (red-yellown-green-blue)
    Feed_r = 253;
    Feed_g = 3;
    Feed_b = 3;

	int limit_c = 15; // hue
	int limit_w = 10; // sat
	int limit_v = 10; // vel

    for(int c = 0; c <= limit_c; c++)
    {
        for(int w = 1; w <= limit_w; w++)
        {
			// skip certain known colors
            if(w % 2) continue;
			if(w == 3) continue;
			if(w == 9) continue;
			
            // Determine Hue
            double HU = c * 22.5;
            for(int VEL=1; VEL <= limit_v; VEL++)
            {
				bool lastcol=false;
				if(c == limit_c && w == limit_w && VEL == limit_v)
				{
					lastcol=true;
				}

                int color_tmp[3] = { Feed_r, Feed_g, Feed_b };
                
                // skip certain known colors
                if(VEL == 8) continue;
                CreateColor( color_tmp[0], color_tmp[1], color_tmp[2], HU, (w*1.0)/10, (double)(VEL/10.0), paletteFile, total_colors, htmlFile, jsonFile, lastcol);
                
                // insert 2 extra colors
                if(VEL == 1)
                {
                    double tmpvel = ( (25 * 1.2) + 3 ) / 255.0 ;
                    CreateColor( color_tmp[0], color_tmp[1], color_tmp[2], HU, (w*1.0)/10, tmpvel, paletteFile, total_colors, htmlFile, jsonFile, false);
                    tmpvel = ( (25 * 1.5) + 3 ) / 255.0 ;
                    CreateColor( color_tmp[0], color_tmp[1], color_tmp[2], HU, (w*1.0)/10, tmpvel, paletteFile, total_colors, htmlFile, jsonFile, false);
                }
            }
            reset_color();
        }
    }

	std::cout << "GIMP Palette\n" << "Name: Extensive Palette\n" << "Colors: " << total_colors << "\n";

	htmlFile << "</div></div><div style=\"position:absolute; left:20px; top:105px;\">Colors: " << total_colors << "<div><br></body></html>\n";
	jsonFile << "]"; // EOF
	
	paletteFile.close();
	htmlFile.close();
	jsonFile.close();

    return 0;
}