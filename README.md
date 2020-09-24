# character_renderer
Compact, easy-to-use library for drawing ASCII symbols using OpenGL.
The library allows for text drawing by either directly writing characters using the pixels on-screen,
or by creating GL_LINES to generate the appropriate text.

Usage:
In the OpenGL rendering context, call either the PixelInitialize or LineInitialize function
to initialize the relevant data, depending on whether the user would like to draw the symbols
using lines or points. Then, load the character color and position data of the string being
rendered using LoadPixelData or LoadLineData, whichever applies. Parameters include x- and y-
position, scale, character spacing, outlining/color options, and cap on max number of characters
to draw. The character position vector is now ready to be used for rendering.
