/* 
 * SVG Data generator library (C++)
 * 
 * Copyright (c) Project Bajran. (MIT License)
 *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */

#pragma once

#include <vector>
#include <string>
#include <sstream>

#include "../lib_qrcodegen/qrcodegen.hpp"


namespace svggen {

/* 
 * Based on provided data in constructor, class:
 *  1) Verifying input data
 *  2) Creating vector<> of tile ttype
 *  3) Making .SVG text based on tile ttype
 */
class SvgData final {


	/*---- Public helper enumeration ----*/

	/* 
	 * Can be used to describe QR code body style
	 */
    public: enum class body {
        SQUARE,
        ROUNDED,
        CIRCLES
    };

	/* 
	 * Can be used to describe QR code eyeball style
	 */
    public: enum class eyeball {
        SQUARE,
        ROUNDED,
        CIRCLE,
        CIRCLES
    };

	/* 
	 * Can be used to describe QR code eyeframe style
	 */
    public: enum class eyeframe {
        SQUARE,
        ROUNDED,
        CIRCLE,
        CIRCLES
    };


	/*---- Public helper structures ----*/

    /* 
     * Describes the shape style of a QR code.
     * Default values are applied if not explicitly specified by the user.
     */
    public: struct StyleShape final{
        body bodystyle;
        eyeball eyeballstyle;
        eyeframe eyeframestyle;
        StyleShape  
        (
            body     _bd = body::CIRCLES, 
            eyeball  _eb = eyeball::CIRCLES, 
            eyeframe _ef = eyeframe::CIRCLES
        ):
            bodystyle(_bd), 
            eyeballstyle(_eb), 
            eyeframestyle(_ef) 
        {}
    };

    /* 
     * Describes the color style of a QR code.
     * Default values are applied if not explicitly specified by the user.
     */
    public: struct StyleColor final{

        std::string background;
        std::string body;
        std::string eyeball;
        std::string eyeframe;

        StyleColor  
        (
            const std::string& backgroundVal = "FF0000",
            const std::string& bodyVal       = "00FFFF00",
            const std::string& eyeballVal    = "00FFFF00",
            const std::string& eyeframeVal   = "00FFFF00"
        ) : 
            background(backgroundVal), 
            body(bodyVal), 
            eyeball(eyeballVal), 
            eyeframe(eyeframeVal) 
        {}
    };


	/*---- Private helper enumeration ----*/

    /* 
     * Represents a single pixel of a QR code as a 'tile'.
     * It is used to categorize each pixel into specific types:
     * BODY, EYEBALL, or EYEFRAME.
     */
    private: enum class tile {
        BODY,
        EYEBALL,
        EYEFRAME
    };

    /* 
     * Represents information about neighboring pixels of a QR code tile.
     * Used to categorize each tile into specific shapes based on the arrangement
     * of surrounding pixels. The '_trivial_' value indicates that the tile's shape
     * does not depend on its neighboring pixels.
     */
    private: enum class neighbours {
        _0_,
        _1_Up,
        _1_Right,
        _1_Down,
        _1_Left,
        _2_UpRight,
        _2_RightDown,
        _2_DownLeft,
        _2_LeftUp,
        _trivial_
    };

    /* 
     * Represents detailed information about an individual tile of a QR code, including:
     *      - Coordinates (x, y) of the tile.
     *      - The type of the tile (tile ttype).
     *      - The categorization of neighboring tiles (neighbours ntype).
     */
    private: struct TileDetail final{
        int x;
        int y;
        tile ttype;
        neighbours ntype;
    };


	/*---- Instance fields ----*/

	// Immutable composite parameters:
    private: StyleShape ss;
    private: StyleColor sc;

	// Immutable scalar parameters:
    private: int    border;
    private: int    qrsize;
    private: int    viewbox;
    private: float  cornerradius;

    // Composite parameters immutable after construction:
    private: std::vector<TileDetail> TileDetailvec;
    private: std::vector<std::vector<bool>> wb;
    private: std::ostringstream svgtext;


    /*---- Constructor ----*/

    public: SvgData (   
                        const qrcodegen::QrCode &qr, 
                        StyleShape _ss, 
                        StyleColor _sc,
                        int _border, 
                        int _cornerradius
                    );


	/*---- Public instance methods ----*/

    /* 
     * Create a .SVG file using the provided filename.
     * The content of the file is based on the current state of the instance.
     */
    public:  void exportSvgDataToFile(std::string filename) const;


    /*---- Private helper methods for constructor: Generating data from QrCode ----*/

    private:  int getBorder                 () const;
    private: void dataValidation            ();
    private: void generateSvgDataFromQrCode (const qrcodegen::QrCode &qr);


    /*---- Private helper methods for constructor: Generating .SVG data file ----*/

    private: void generateSvgDataText ();

    private: void makeSvgHeader       ();
    private: void makeQrBackground    ();
    private: void makeQrBody          ();
    private: void makeSvgEnd          ();

    private: void makeBackgroundForEyeFrameCircleText();
    private: void makeBackgroundForEyeFrameRestText  ();
    private: void makeEyeFrameCircleText             ();
    private: void makeEyeBallCircleText              ();

    private: void makeItSquareText  (SvgData::tile part, std::string color);
    private: void makeItRoundText   (SvgData::tile part, std::string color);
    private: void makeItCirclesText (SvgData::tile part, std::string color);

/*---- End of Class SvgData ----*/
};
/*---- End of namespace svggen ----*/
}

