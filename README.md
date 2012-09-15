tesseract-lcm
=============

Decoding an image received over LCM and extracting the text within it using Tesseract.

To build, first generate header files for each LCM type:
    $ lcm-gen -x lcmtypes/*

Then, modify bot_core/image_t.hpp to include the metadata description correctly. Change
    #include "bot_core/image_metadata_t.hpp"
to
    #include "image_metadata_t.hpp"

Finally, compile the project using:
    $ g++ tesseract_lcm.cpp -o build/tesseract_lcm -llept -ltesseract -llcm

You can run the executable using:
    $ build/tesseract_lcm CHANNEL_NAME
which will listen for libbot images on channel CHANNEL_NAME.
