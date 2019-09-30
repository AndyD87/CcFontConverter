# CcFontConverter

Use Qt to convert fonts into bitmuster for embedded software.

Take a look in generated source file CcFontConverter/CFontExample.c for used
output format.

## Features

 - OS independent, thanks to Qt
 - System fonts and font files are supported
 - Generate Sources for C or C++
 - Generated Text will be placed in predefined area, so files can be updated
    without overwriting custom content.
 
## Limitations/Todo's

 - Only signs from U+0000 to U+007f will be generated
 - No preview
 - Textoutput only for C Source, for header and/or C++ Files must be generated.

## Requirements

 - Qt > 5.6 (For earlier versions, some changes might be required)
 
## Examples

Default charset is *Bitstream Vera Sans* wich is fixed in Resources and
loaded on Application start

The procject has generated example files in it's sources:
 - CcFontConverter/CFontExample.h / -.c for generated C Sources
 - CcFontConverter/CFontExampleCpp.h / -.cpp for generated CPP Sources

