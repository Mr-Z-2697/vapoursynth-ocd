#include <cstdlib>
#include "vapoursynth/VapourSynth4.h"
#include "vapoursynth/VSHelper4.h"
#include <opencv2/opencv.hpp>

#include "dct.h"
#include "gaussianblur.h"

VS_EXTERNAL_API(void) VapourSynthPluginInit2(VSPlugin *plugin, const VSPLUGINAPI *vspapi) {
    vspapi->configPlugin("void.ocd", "ocd", "vapoursynth opencv dct", VS_MAKE_VERSION(1, 0), VAPOURSYNTH_API_VERSION, 0, plugin);
    vspapi->registerFunction("dct", "clip:vnode;inverse:int:opt;", "clip:vnode;", dctCreate, NULL, plugin);
    vspapi->registerFunction("gaussianblur", "clip:vnode;sigmax:float:opt;sigmay:float:opt;sizex:int:opt;sizey:int:opt;bordertype:int:opt;", "clip:vnode;", gblurCreate, NULL, plugin);
}
