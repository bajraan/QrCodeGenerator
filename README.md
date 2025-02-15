<!-- Begin README -->

<div align="center">
    <img src="./docs/images/icon.svg" width="200" height="200"/>
</div>
<br>
<p align="center">
    <a href="https://daringfireball.net/projects/markdown/"><img src="https://img.shields.io/badge/Markdown-1.0.1-000000?style=for-the-badge&logo=markdown" alt="Markdown Badge" /></a>
    <a href="https://github.com/bajraan"><img src="https://img.shields.io/badge/github-follow_me-181717?style=for-the-badge&logo=github&color=181717" alt="GitHub Badge" /></a>
    <a href="mailto:bajran1616@gmail.com"><img src="https://img.shields.io/badge/gmail-contact_me-EA4335?style=for-the-badge&logo=gmail" alt="Email Badge" /></a>
    <br>
</p>

> [!NOTE]
> This project builds upon the foundations of the following GitHub projects:
> 
> <p align="center">
>     <a href="https://github.com/nayuki/QR-Code-generator">
>         <img src="https://img.shields.io/badge/github-nayuki/QR--Code--generator-181717?logo=github&color=181717" alt="GitHub Badge" />
>     </a><br>
>     <a href="https://github.com/scottgriv/markdown-demo">
>         <img src="https://img.shields.io/badge/github-scottgriv/markdown--demo-181717?logo=github&color=181717" alt="GitHub Badge" />
>     </a>
> </p>
>
> Created for educational purposes, this project serves as a library designed to generate QR codes and export them in specified design styles to .svg files.
>
> The repository is based on the nayuki/QR-Code-generator project, encompassing two libraries and a single demo .cpp file. The 'lib_qrcodegen' directory contains the original, unmodified files from the nayuki project, while the 'lib_svggen' directory introduces a newly designed library for exporting QR code objects to SVG files, enabling users to define their own styles.


---------------
> [!TIP]
> Wymagania

<div align="center">
    <img src="./docs/images/prereq.gif" width="800" height="385"/>
</div>

---------------

> [!TIP]
> Pobieranie i budowanie

<div align="center">
    <img src="./docs/images/make.gif" width="800" height="385"/>
</div>

---------------

> [!TIP]
> Example 

> [!NOTE]  
> This all examples may be found in QrCodeGeneratorDemo.cpp. They are genereted using demo fucntion called: doMarkdownExamples()

<br>
<br>
<div align="center">
    <img src="./10_UserSvgStyleSample/CIS-CIE-CIE-4-1.svg" width="200" height="200"/>
</div>

```cpp
// CIS-CIE-CIE-4-1		
st.bodystyle     = SvgData::body::CIRCLES;
st.eyeballstyle  = SvgData::eyeball::CIRCLE;
st.eyeframestyle = SvgData::eyeframe::CIRCLE;
sc.background = "AAFF00";
sc.body = "011620";
sc.eyeball = "1010FF";
sc.eyeframe = "0116AA";
cornerradius = 4;
border = 1;	
```

<br>
<div align="center">
    <img src="./10_UserSvgStyleSample/SQU-CIE-CIS-4-2.svg" width="200" height="200"/>
</div>

```cpp
// SQU-CIE-CIS-4-2
st.bodystyle     = SvgData::body::SQUARE;
st.eyeballstyle  = SvgData::eyeball::CIRCLE;
st.eyeframestyle = SvgData::eyeframe::CIRCLES;
sc.background = "ffee00";
sc.body = "bc0068";
sc.eyeball = "bc0068";
sc.eyeframe = "fb00ff";
cornerradius = 4;
border = 2;	
```

<br>
<div align="center">
    <img src="./10_UserSvgStyleSample/ROD-CIR-ROD-4-2.svg" width="200" height="200"/>
</div>

```cpp
// ROD-CIR-ROD-4-2
st.bodystyle     = SvgData::body::ROUNDED;
st.eyeballstyle  = SvgData::eyeball::ROUNDED;
st.eyeframestyle = SvgData::eyeframe::ROUNDED;
sc.background = "270065";
sc.body = "d4ff00";
sc.eyeball = "ff0000";
sc.eyeframe = "ff0000";
cornerradius = 4;
border = 2;	
```

---------------
---------------
---------------

> [!NOTE]
> This project is built upon the foundations of the following GitHub projects:
> 
> <p align="center">
>     <a href="https://github.com/nayuki/QR-Code-generator">
>         <img src="https://img.shields.io/badge/github-nayuki/QR--Code--generator-181717?logo=github&color=181717" alt="GitHub Badge" />
>     </a><br>
>     <a href="https://github.com/scottgriv/markdown-demo">
>         <img src="https://img.shields.io/badge/github-scottgriv/markdown--demo-181717?logo=github&color=181717" alt="GitHub Badge" />
>     </a>
> </p>

<!-- End README -->
<!-- End README -->
<!-- End README -->