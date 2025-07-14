#ifndef VSOCD_GAUSSIANBLUR
#define VSOCD_GAUSSIANBLUR

#include <cstdlib>
#include "vapoursynth/VapourSynth4.h"
#include "vapoursynth/VSHelper4.h"
#include <opencv2/imgproc.hpp>

typedef struct {
    VSNode *node;
    double sigmaX;
    double sigmaY;
    int sizeX;
    int sizeY;
    int borderType;
} GblurData;
const VSFrame *VS_CC gblurGetFrame(int n, int activationReason, void *instanceData, void **frameData, VSFrameContext *frameCtx, VSCore *core, const VSAPI *vsapi);
void VS_CC gblurFree(void *instanceData, VSCore *core, const VSAPI *vsapi);
void VS_CC gblurCreate(const VSMap *in, VSMap *out, void *userData, VSCore *core, const VSAPI *vsapi);

#endif // ifndef VSOCD_GAUSSIANBLUR
