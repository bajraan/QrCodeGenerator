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


#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "svggen.hpp"
#include "../lib_qrcodegen/qrcodegen.hpp"


namespace svggen {

/*---- Class SvgData ----*/

SvgData::SvgData (
                    const qrcodegen::QrCode &qr,
                    const StyleShape _ss, 
                    const StyleColor _sc, 
                    const int _border = 1, 
                    const int _cornerradius= 0
                 ):   
                    ss(_ss), 
                    sc(_sc), 
                    border(_border), 
                    cornerradius(_cornerradius)
{
    dataValidation();
    generateSvgDataFromQrCode(qr);
    generateSvgDataText();
}

void SvgData::dataValidation(){
    //corner radious
    //colors
}

void SvgData::exportSvgDataToFile(std::string filename) const {

    std::string outputPath = "./11_UserSvgOutput/";

    // check if extension is already given or not.
    if(filename.find(".svg") == std::string::npos)
        filename += ".svg";
    
    std::string fullFilePath = outputPath + filename;
    std::ofstream outputFile(fullFilePath);

    if (!std::filesystem::exists(outputPath)) {
        std::filesystem::create_directory(outputPath);
    }

	if (outputFile.is_open()) 
    {
        outputFile << svgtext.str();
        outputFile.close();
        std::cout << "File " << filename << " created" << std::endl;
    } 
    else 
    {
        std::cout << "File " << filename << " creation fail" << std::endl;
    }

}

int SvgData::getBorder() const {
    return this->border;
}

void SvgData::generateSvgDataFromQrCode(const qrcodegen::QrCode &qr){

    /*
     *  Get size of QrCode from QrCode class 
     */
    qrsize = qr.getSize();

    /*
     *  Deeermine viewBox 
     */
    viewbox = qrsize + border * 2;

    /*
     *  Create a whiteboard with 1 pixel frame to easily perform neighbourn finder algorithm.
     */
    const int onepixelframe = 1;
    int wbsize = qrsize + onepixelframe * 2;
	wb = std::vector<std::vector<bool>>(wbsize, std::vector<bool>(wbsize, false));

    /*
     *  Store information from qr in to whiteboard.
     *  1px frame can not be overwrited.
     */
	for (int y = 0; y < qrsize; y++)
		for (int x = 0; x < qrsize; x++)
			if (qr.getModule(x, y))
				wb[x+onepixelframe][y+onepixelframe] = true;

   /*
    *  Determine the number of neighbors for each "black pixel".
    *  The data will be stored in TileDetailvec.
    *  "White pixels" will be omitted.
    */
    SvgData::TileDetail TileDetail;
    for (int y = 1; y < wbsize-1; y++) {
		for (int x = 1; x < wbsize-1; x++) {

			TileDetail.x = x-1;   // remove temporary border 
            TileDetail.y = y-1;	// from wb[wbsize][wbsize] (whiteboard)

			if		(!wb[x][y]) continue; // skip white elements

			else if ( !wb[x][y-1] and !wb[x+1][y] and !wb[x][y+1] and !wb[x-1][y] ){
                TileDetail.ntype = SvgData::neighbours::_0_;
			}
			else if	(  wb[x][y-1] and !wb[x+1][y] and !wb[x][y+1] and !wb[x-1][y] ){
                TileDetail.ntype = SvgData::neighbours::_1_Up;
			}
			else if	( !wb[x][y-1] and  wb[x+1][y] and !wb[x][y+1] and !wb[x-1][y] ){
                TileDetail.ntype = SvgData::neighbours::_1_Right;
			}
			else if	( !wb[x][y-1] and !wb[x+1][y] and  wb[x][y+1] and !wb[x-1][y] ){
                TileDetail.ntype = SvgData::neighbours::_1_Down;
			}
			else if	( !wb[x][y-1] and !wb[x+1][y] and !wb[x][y+1] and  wb[x-1][y] ){
                TileDetail.ntype = SvgData::neighbours::_1_Left;
			}
			else if	(  wb[x][y-1] and  wb[x+1][y] and !wb[x][y+1] and !wb[x-1][y] ){
                TileDetail.ntype = SvgData::neighbours::_2_UpRight;
			}
			else if	( !wb[x][y-1] and  wb[x+1][y] and  wb[x][y+1] and !wb[x-1][y] ){
                TileDetail.ntype = SvgData::neighbours::_2_RightDown;
			}
			else if	( !wb[x][y-1] and !wb[x+1][y] and  wb[x][y+1] and  wb[x-1][y] ){
                TileDetail.ntype = SvgData::neighbours::_2_DownLeft;
			}
			else if	(  wb[x][y-1] and !wb[x+1][y] and !wb[x][y+1] and  wb[x-1][y] ){
                TileDetail.ntype = SvgData::neighbours::_2_LeftUp;
			}
			else{
				TileDetail.ntype = SvgData::neighbours::_trivial_;
			}
			TileDetailvec.push_back(TileDetail);
		}
	}

   /*
    *  Divide the TileDetail.TileDetailvec into three categories:
    *   -> tile::BODY
    *   -> tile::EYEBODY
    *   -> tile::EYEFRAME
    *  Information about category will be stored in TileDetail.ttype
    */

   /*
    * Set all tiles as tile::BODY.
    */
    for(auto &el : TileDetailvec)
        el.ttype = tile::BODY;
   
   /*
    *  examine tile::BODY to determine if they are tile::EYEFRAME
    */
    for(auto &el : TileDetailvec ){

        if      (el.x <= 7        and el.y <= 7        ) el.ttype = tile::EYEFRAME;
        else if (el.x >= qrsize-7 and el.y <= 7        ) el.ttype = tile::EYEFRAME;
        else if (el.x <= 7        and el.y >= qrsize-7 ) el.ttype = tile::EYEFRAME;
        
    }

   /*
    *  examine tile::EYEFRAME to determine if they are tile::EYEBALL
    */
    for(auto &el : TileDetailvec ){
        if(el.ttype == tile::EYEFRAME){

        if     (        el.x >= 2 and el.x <= 4         and  el.y >= 2        and el.y <= 4        ) el.ttype = tile::EYEBALL;
        else if( el.x >= qrsize-5 and el.x <= qrsize-2  and  el.y >= 2        and el.y <= 4        ) el.ttype = tile::EYEBALL;
        else if(        el.x >= 2 and el.x <= 4         and  el.y >= qrsize-5 and el.y <= qrsize-2 ) el.ttype = tile::EYEBALL;

        }
    }
}

void SvgData::generateSvgDataText(){

   /*
    *  header
    */
	svgtext << "<svg\n"
            << "\txmlns=\"http://www.w3.org/2000/svg\"\n"
            << "\tversion=\"1.1\"\n"
            << "\tviewBox=\"0 0 " << viewbox << " " << viewbox << " \"\n"
            << "\tstroke=\"none\">\n\n";

   /*
    *  QR Background
    */
    svgtext << "\t<!-- QrBackground  -->\n"
            << "\t<g"
            << " id=\"QrBackground\""
            << " fill=\"#"<<sc.background<<"\">\n";
    makeQrBackground();
    svgtext << "\t</g>\n\n";

   /*
    *  QR Body
    */
    svgtext << "\t<!-- QrBody  -->\n"
            << "\t<g"
            << " id=\"QrBody\">\n\n";
    makeQrBody();
    svgtext << "\t</g>\n";

   /*
    *  End
    */
 	svgtext << "</svg>\n";
}

void SvgData::makeQrBackground(){

    // adjusting maxroudn to the viewbox;
    float maxround = border + border * 2 ;
    if(maxround > viewbox/2.0)
        maxround = viewbox/2.0;

    int overleap = cornerradius - maxround;
    if(overleap > 3) overleap = 3;
    if(overleap < 0) overleap = 0;

    if(ss.eyeframestyle == eyeframe::CIRCLE ){
        // adjusting overleap to the boreder for circle purpose;
        if(overleap){
            cornerradius = maxround+overleap;
            makeBackgroundForEyeFrameCircleText();
        }
    } else {
        //adjusting cornersize the the border
        if(cornerradius > maxround)
            cornerradius = maxround;
    }

    makeBackgroundForEyeFrameRestText();

}

void SvgData::makeBackgroundForEyeFrameCircleText(){

    svgtext <<"\t\t<rect width=\"" << 7 + border
            <<"\" height=\""     << 7 + border
            <<"\" x=\"" << qrsize + border - 7
            <<"\" y=\"" << qrsize + border - 7
            <<"\" rx=\""<< 1
            <<"\" ry=\""<< 1
            <<"\" />\n";
}

void SvgData::makeBackgroundForEyeFrameRestText(){

    svgtext <<"\t\t<rect width=\""<<(qrsize + border * 2)
            <<"\" height=\""<<(qrsize + border * 2)
            <<"\" rx=\""<< cornerradius
            <<"\" ry=\""<< cornerradius
            <<"\" />\n";
}

void SvgData::makeQrBody(){

   switch (ss.bodystyle) {

        case body::SQUARE :
            makeItSquareText(tile::BODY, sc.body);
            break;
        case body::ROUNDED :
            makeItRoundText(tile::BODY, sc.body);
            break;
        case body::CIRCLES :
            makeItCirclesText(tile::BODY, sc.body);
            break;
        default:
            throw std::invalid_argument("Unknown body style");
            break;
    }

    switch (ss.eyeframestyle) {

        case eyeframe::SQUARE :
            makeItSquareText(tile::EYEFRAME, sc.eyeframe);
            break;
        case eyeframe::ROUNDED :
            makeItRoundText(tile::EYEFRAME, sc.eyeframe);
            break;
        case eyeframe::CIRCLES :
            makeItCirclesText(tile::EYEFRAME, sc.eyeframe);
            break;
        case eyeframe::CIRCLE :
            makeEyeFrameCircleText();
            break;
        default:
            throw std::invalid_argument("Unknown eyeframe style");
            break;
    }

    switch (ss.eyeballstyle) {

        case eyeball::SQUARE :
            makeItSquareText(tile::EYEBALL, sc.eyeball);
            break;
        case eyeball::ROUNDED :
            makeItRoundText(tile::EYEBALL, sc.eyeball);
            break;
        case eyeball::CIRCLES :
            makeItCirclesText(tile::EYEBALL, sc.eyeball);
            break;
        case eyeball::CIRCLE :
            makeEyeBallCircleText();
            break;
        default:
            throw std::invalid_argument("Unknown eyeball style");
            break;
    }
}

void SvgData::makeEyeFrameCircleText(){

    svgtext << "\t\t<!-- EYEFRAME -->\n\t\t<g id=\"EYEFRAME\" "

            << " stroke=\"#"<< sc.eyeframe 
            << "\" fill=\"none\" stroke-width=\"1\" >\n"

            << "\t\t\t<circle cx=\"" << 3.5 + border 
            <<        "\" cy=\"" << 3.5 + border
            << "\" r=\"3\" />\n"

            << "\t\t\t<circle cx=\"" << qrsize - 3.5 + border 
            <<        "\" cy=\"" << 3.5 + border
            << "\" r=\"3\" />\n"

            << "\t\t\t<circle cx=\"" << 3.5 + border 
            <<        "\" cy=\"" << qrsize - 3.5 + border
            << "\" r=\"3\" />\n"

            << "\t\t</g>\n";
}

void SvgData::makeEyeBallCircleText(){

    svgtext << "\t\t<!-- EYEBALL -->\n\t\t<g id=\"EYEBALL\" "
            << "fill=\"#"<< sc.eyeball <<"\">\n"

            << "\t\t\t<circle cx=\"" << 3.5 + border 
            <<        "\" cy=\"" << 3.5 + border
            << "\" r=\"1.75\"/>\n"
        
            << "\t\t\t<circle cx=\"" << qrsize - 3.5 + border 
            <<        "\" cy=\"" << 3.5 + border
            << "\" r=\"1.75\"/>\n"

            << "\t\t\t<circle cx=\"" << 3.5 + border 
            <<        "\" cy=\"" << qrsize - 3.5 + border
            << "\" r=\"1.75\"/>\n"

            << "\t\t</g>\n";
}

void SvgData::makeItSquareText(SvgData::tile part, std::string color){

    if( part==tile::BODY     )  svgtext << "\t\t<!-- BODY     -->\n\t\t<g id=\"BODY\" ";
    if( part==tile::EYEFRAME )  svgtext << "\t\t<!-- EYEFRAME -->\n\t\t<g id=\"EYEFRAME\" ";
    if( part==tile::EYEBALL  )  svgtext << "\t\t<!-- EYEBALL  -->\n\t\t<g id=\"EYEBALL\" ";

	svgtext << "fill=\"#"<< color <<"\">\n";
    svgtext << "\t\t\t<path d=\"\n";

    for(auto el : TileDetailvec){
        if(el.ttype == part){
            svgtext << "\t\t\t\tM" << (el.x + border) << "," << (el.y + border) << " h1v1h-1z\n";
        }
    }

    svgtext << "\t\t\t\"/>\n";
    svgtext << "\t\t</g>\n";
}

void SvgData::makeItRoundText(SvgData::tile part, std::string color){

    if( part==tile::BODY     )  svgtext << "\t\t<!-- BODY     -->\n\t\t<g id=\"BODY\" ";
    if( part==tile::EYEFRAME )  svgtext << "\t\t<!-- EYEFRAME -->\n\t\t<g id=\"EYEFRAME\" ";
    if( part==tile::EYEBALL  )  svgtext << "\t\t<!-- EYEBALL  -->\n\t\t<g id=\"EYEBALL\" ";

    svgtext << "fill=\"#"<< color <<"\">\n";

	for(auto &el : TileDetailvec){
        if(el.ttype != part) continue;

		switch(el.ntype){

			case SvgData::neighbours::_0_ :
			  svgtext << "\t\t\t<circle cx=\"" << el.x+0.5 + border << "\" cy=\"" << el.y+0.5 + border<< "\" r=\"0.5\"/>\n";
			  break;

			case SvgData::neighbours::_1_Up :
			  svgtext << "\t\t\t<path d=\"M" 	  << el.x + 0 + border << " " << el.y + 0 +border
					  << " L" 	      	  << el.x + 0 + border << " " << el.y +0.5+ border 
					  << " A0.5 0.5 0 0 0 " << el.x + 1 + border << " " << el.y +0.5+	border
					  << " L" 	          << el.x + 1 + border << " " << el.y - 0 + border 
					  << " Z\"/>\n";
			  break;
			case SvgData::neighbours::_1_Down :
			  svgtext << "\t\t\t<path d=\"M" 	  << el.x + 0 + border << " " << el.y + 1 +	border
					  << " L" 	      	  << el.x + 0 + border << " " << el.y +0.5+ border 
					  << " A0.5 0.5 0 0 1 " << el.x + 1 + border << " " << el.y +0.5+	border
					  << " L" 	          << el.x + 1 + border << " " << el.y + 1 + border 
					  << " Z\"/>\n";
			  break;
			case SvgData::neighbours::_1_Right :
			  svgtext << "\t\t\t<path d=\"M" 	  << el.x + 1 + border << " " << el.y + 0 +	border
					  << " L" 	      	  << el.x +0.5+ border << " " << el.y + 0 + border 
					  << " A0.5 0.5 0 0 0 " << el.x +0.5+ border << " " << el.y + 1 +	border
					  << " L" 	          << el.x + 1 + border << " " << el.y + 1 + border 
					  << " Z\"/>\n";
			  break;
			case SvgData::neighbours::_1_Left :
			  svgtext << "\t\t\t<path d=\"M" 	  << el.x + 0 + border << " " << el.y + 0 +	border
					  << " L" 	      	  << el.x +0.5+ border << " " << el.y + 0 + border 
					  << " A0.5 0.5 0 0 1 " << el.x +0.5+ border << " " << el.y + 1 +	border
					  << " L" 	          << el.x + 0 + border << " " << el.y + 1 + border 
					  << " Z\"/>\n";
			  break;
			case SvgData::neighbours::_2_UpRight:
			  svgtext << "\t\t\t<path d=\"M"  << el.x + 0 + border << " " << el.y + 0 + border
					  << " A1 1 0 0 0 " << el.x + 1 + border << " " << el.y + 1 + border
					  << " L" 	      << el.x + 1 + border << " " << el.y + 0 + border 
					  << " Z\"/>\n";
			  break;
			case SvgData::neighbours::_2_RightDown:
			  svgtext << "\t\t\t<path d=\"M"  << el.x + 1 + border << " " << el.y + 0 + border
					  << " A1 1 0 0 0 " << el.x + 0 + border << " " << el.y + 1 + border
					  << " L" 		  << el.x + 1 + border << " " << el.y + 1 + border 
					  << " Z\"/>\n";
			  break;
			case SvgData::neighbours::_2_DownLeft:
			  svgtext << "\t\t\t<path d=\"M"   << el.x + 0 + border << " " << el.y + 0 + border
					  << " A1 1 0 0 1 "  << el.x + 1 + border << " " << el.y + 1 + border
					  << " L" 		   << el.x + 0 + border << " " << el.y + 1 + border 
					  << " Z\"/>\n";
			  break;
			case SvgData::neighbours::_2_LeftUp:
			  svgtext << "\t\t\t<path d=\"M"   << el.x + 1 + border << " " << el.y + 0 + border
					  << " A1 1 0 0 1 "  << el.x + 0 + border << " " << el.y + 1 + border
					  << " L" 		   << el.x + 0 + border << " " << el.y + 0 + border 
					  << " Z\"/>\n";
			  break;
			case SvgData::neighbours::_trivial_:
			  svgtext << "\t\t\t<path d=\"M"   << el.x + 0 + border << " " << el.y + 0 + border
					  << " h1v1h-1z"
					  << " Z\"/>\n";
			  break;
			default:
				break;
		}
	}
    svgtext << "\t\t</g>\n";
}

void SvgData::makeItCirclesText(SvgData::tile part, std::string color){

    if( part==tile::BODY     )  svgtext << "\t\t<!-- BODY     -->\n\t\t<g id=\"BODY\" ";
    if( part==tile::EYEFRAME )  svgtext << "\t\t<!-- EYEFRAME -->\n\t\t<g id=\"EYEFRAME\" ";
    if( part==tile::EYEBALL  )  svgtext << "\t\t<!-- EYEBALL  -->\n\t\t<g id=\"EYEBALL\" ";

    svgtext << "fill=\"#"<< color <<"\">\n";

    for(auto el : TileDetailvec){
        if(el.ttype != part) continue;
            svgtext << "\t\t\t<circle cx=\"" << el.x+0.5 + border << "\" cy=\"" << el.y+0.5 + border<< "\" r=\"0.5\"/>\n";
    }
    svgtext << "\t\t</g>\n";

}

/*---- End of clss SvgGen ----*/
} /*---- End of namespace svggen ----*/
