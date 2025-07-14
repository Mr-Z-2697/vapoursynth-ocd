`core.ocd.dct(clip, inverse=0)`    
```
core.ocd.gaussianblur(clip, sigmax=1, sigmay=0, sizex=0, sizey=0, bordertype=4)

borderType	pixel extrapolation method, see BorderTypes. BORDER_WRAP is not supported.

enum BorderTypes {
    BORDER_CONSTANT    = 0, //!< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`
    BORDER_REPLICATE   = 1, //!< `aaaaaa|abcdefgh|hhhhhhh`
    BORDER_REFLECT     = 2, //!< `fedcba|abcdefgh|hgfedcb`
    BORDER_WRAP        = 3, //!< `cdefgh|abcdefgh|abcdefg`
    BORDER_REFLECT_101 = 4, //!< `gfedcb|abcdefgh|gfedcba`
    BORDER_TRANSPARENT = 5, //!< `uvwxyz|abcdefgh|ijklmno` - Treats outliers as transparent.

    BORDER_REFLECT101  = BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
    BORDER_DEFAULT     = BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
    BORDER_ISOLATED    = 16 //!< Interpolation restricted within the ROI boundaries.
};
```    

### build
1. Have VapourSynth headers    
2. Run ocd.sh    
(I only tried mingw)    
**opencv has so many default enabled 3rd party components which is not needed for this simple plugin turn them off I'm too lazy to write them down 😥**    
