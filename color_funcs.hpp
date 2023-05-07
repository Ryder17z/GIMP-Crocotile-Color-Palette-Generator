// r,g,b values are from 0 to 255
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)

std::tuple<uint8_t, uint8_t, uint8_t> HSV_to_RGB(std::tuple<double, double, double> HSV_color);
std::tuple<double, double, double> RGB_to_HSV(std::tuple<uint8_t, uint8_t, uint8_t> RGB_color);

/**
 * Calculates the difference in the hue, saturation, and value components between two HSV colors.
 * Range is (0.0 - 360.0) for Hue and (0.0 - 1.0) for Saturation and Value.
 *
 * @param color1 A tuple representing the HSV values of the first color.
 * @param color2 A tuple representing the HSV values of the second color.
 * @return A tuple representing the absolute differences between the hue, saturation, and value components of the two colors.
 */
std::tuple<double, double, double> GetdeltaHSV(std::tuple<double, double, double> color1, std::tuple<double, double, double> color2) {
    double deltaHue = std::abs(std::get<0>(color1) - std::get<0>(color2));
    double deltaSaturation = std::abs(std::get<1>(color1) - std::get<1>(color2));
    double deltaValue = std::abs(std::get<2>(color1) - std::get<2>(color2));
    return std::make_tuple(deltaHue, deltaSaturation, deltaValue);
}

/**
 * Calculates the difference in the red, green, and blue components between two RGB colors. (Range is 0-255)
 *
 * @param color1 A tuple representing the RGB values of the first color.
 * @param color2 A tuple representing the RGB values of the second color.
 * @return A tuple representing the absolute differences between the red, green, and blue components of the two colors.
 */
std::tuple<uint8_t, uint8_t, uint8_t> GetdeltaRGB(std::tuple<uint8_t, uint8_t, uint8_t> color1, std::tuple<uint8_t, uint8_t, uint8_t> color2) {
    uint8_t deltaRed = std::abs(std::get<0>(color1) - std::get<0>(color2));
    uint8_t deltaGreen = std::abs(std::get<1>(color1) - std::get<1>(color2));
    uint8_t deltaBlue = std::abs(std::get<2>(color1) - std::get<2>(color2));
    return std::make_tuple(deltaRed, deltaGreen, deltaBlue);
}

/**
 * @brief Creates a new color with the given RGB values by shifting the hue, saturation, and value.
 *
 * @param red, green, blue The red, green, blue components of the RGB color. (Range is 0-255)
 * @param shiftHue The amount to shift the hue value of the color (in degrees)
 * @param shiftSat The amount to shift the saturation value of the color (a value between -1.0 and 1.0)
 * @param shiftVal The amount to shift the value of the color (a value between -1.0 and 1.0)
 *
 * @return A tuple representing the resulting color in RGB format, with values between 0 and 255.
 */
std::tuple<uint8_t, uint8_t, uint8_t> CreateColor(uint8_t red, uint8_t green, uint8_t blue, double shiftHue, double shiftSat, double shiftVal)
{
    //printf("Red:%u, Green:%u, Blue:%u, shiftHue:%f, shiftSat:%f, shiftVal:%f \n", red, green, blue, shiftHue, shiftSat, shiftVal);

    double hue = 0.0f, sat = 0.0f, val = 0.0f;
    //std::tuple<double, double, double> hsv = RGB_to_HSV(std::make_tuple(red, green, blue));
    //printf("Previous Color: Hue: %.2f, Saturation: %.2f, Value: %.2f\n", std::get<0>(hsv), std::get<1>(hsv), std::get<2>(hsv));

    hue += shiftHue;
    sat += shiftSat;
    val += shiftVal;

    // ensure that the HUE, SAT, and VAL values are within the expected ranges
    hue = std::clamp(hue, 0.0, 360.0);
    sat = std::clamp(sat, 0.0, 1.0);
    val = std::clamp(val, 0.0, 1.0);

    printf("Color: Hue: %.2f, Saturation: %.2f, Value: %.2f\n", hue, sat, val);

    return HSV_to_RGB(std::make_tuple(hue, sat, val));
}

/**
 * @brief Extracts the minimum and maximum hue, saturation, and value (HSV) values from two RGB colors.
 *
 * Given two RGB colors represented as std::tuple<uint8_t, uint8_t, uint8_t>, this function extracts
 * the minimum and maximum hue, saturation, and value (HSV) values using the RGB_to_HSV function.
 *
 * @param start_color The first RGB color to extract HSV values from.
 * @param end_color The second RGB color to extract HSV values from.
 * @return A tuple of six doubles representing the min/max hue, saturation, and value (HSV) values.
 *         The values are ordered as follows: min_h, max_h, min_s, max_s, min_v, max_v.
 */
std::tuple<double, double, double, double, double, double> extract_min_max_hsv( std::tuple<uint8_t, uint8_t, uint8_t> start_color, std::tuple<uint8_t, uint8_t, uint8_t> end_color)
{
    double min_h = 360.0, max_h = 0.0;
    double min_s = 1.0, max_s = 0.0;
    double min_v = 1.0, max_v = 0.0;

    // Convert start and end colors to HSV and update min/max values
    for (const auto& color : { start_color, end_color }) {
        auto hsv_color = RGB_to_HSV(color);
        double h = std::get<0>(hsv_color);
        double s = std::get<1>(hsv_color);
        double v = std::get<2>(hsv_color);
        min_h = std::min(min_h, h);
        max_h = std::max(max_h, h);
        min_s = std::min(min_s, s);
        max_s = std::max(max_s, s);
        min_v = std::min(min_v, v);
        max_v = std::max(max_v, v);
    }

    return std::make_tuple(min_h, max_h, min_s, max_s, min_v, max_v);
}

std::tuple<uint8_t, uint8_t, uint8_t> HSV_to_RGB(std::tuple<double, double, double> HSV_color) {
    double h = std::get<0>(HSV_color);
    double s = std::get<1>(HSV_color);
    double v = std::get<2>(HSV_color);

    double c = v * s;
    double x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    double m = v - c;

    double r, g, b;
    if (h < 60.0) {
        r = c;
        g = x;
        b = 0;
    }
    else if (h < 120.0) {
        r = x;
        g = c;
        b = 0;
    }
    else if (h < 180.0) {
        r = 0;
        g = c;
        b = x;
    }
    else if (h < 240.0) {
        r = 0;
        g = x;
        b = c;
    }
    else if (h < 300.0) {
        r = x;
        g = 0;
        b = c;
    }
    else {
        r = c;
        g = 0;
        b = x;
    }

    uint8_t r_int = static_cast<uint8_t>((r + m) * 255);
    uint8_t g_int = static_cast<uint8_t>((g + m) * 255);
    uint8_t b_int = static_cast<uint8_t>((b + m) * 255);

    return std::make_tuple(r_int, g_int, b_int);
}

std::tuple<double, double, double> RGB_to_HSV(std::tuple<uint8_t, uint8_t, uint8_t> RGB_color)
{
    // Extract the red, green, and blue components from the RGB tuple.
    uint8_t r = std::get<0>(RGB_color);
    uint8_t g = std::get<1>(RGB_color);
    uint8_t b = std::get<2>(RGB_color);

    // Convert the RGB values to the [0, 1] range.
    double r_norm = static_cast<double>(r) / 255.0;
    double g_norm = static_cast<double>(g) / 255.0;
    double b_norm = static_cast<double>(b) / 255.0;

    // Find the minimum and maximum values among the RGB components.
    double cmax = std::max(std::max(r_norm, g_norm), b_norm);
    double cmin = std::min(std::min(r_norm, g_norm), b_norm);

    // Calculate the value (V) of the HSV color space.
    double V = cmax;

    // Calculate the saturation (S) of the HSV color space.
    double S = 0.0;
    if (cmax != 0.0) {
        S = (cmax - cmin) / cmax;
    }

    // Calculate the hue (H) of the HSV color space.
    double H = 0.0;
    if (cmax != cmin) {
        if (cmax == r_norm) {
            H = (g_norm - b_norm) / (cmax - cmin);
        }
        else if (cmax == g_norm) {
            H = 2.0 + (b_norm - r_norm) / (cmax - cmin);
        }
        else {
            H = 4.0 + (r_norm - g_norm) / (cmax - cmin);
        }
        H *= 60.0;
        if (H < 0.0) {
            H += 360.0;
        }
    }

    // Return the HSV color tuple.
    return std::make_tuple(H, S, V);
}
