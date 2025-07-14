#ifndef VSOCD_DCT
#define VSOCD_DCT

#include <cstdlib>
#include "vapoursynth/VapourSynth4.h"
#include "vapoursynth/VSHelper4.h"
#include <opencv2/opencv.hpp>

typedef struct {
    VSNode *node;
    int inverse;
} DctData;
const VSFrame *VS_CC dctGetFrame(int n, int activationReason, void *instanceData, void **frameData, VSFrameContext *frameCtx, VSCore *core, const VSAPI *vsapi);
void VS_CC dctFree(void *instanceData, VSCore *core, const VSAPI *vsapi);
void VS_CC dctCreate(const VSMap *in, VSMap *out, void *userData, VSCore *core, const VSAPI *vsapi);

#endif // ifndef VSOCD_DCT
