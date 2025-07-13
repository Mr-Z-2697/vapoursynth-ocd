#include "dct.h"

const VSFrame *VS_CC dctGetFrame(int n, int activationReason, void *instanceData, void **frameData, VSFrameContext *frameCtx, VSCore *core, const VSAPI *vsapi) {
    DctData *d = (DctData *)instanceData;

    if (activationReason == arInitial) {
        vsapi->requestFrameFilter(n, d->node, frameCtx);
    } else if (activationReason == arAllFramesReady) {
        const VSFrame *src = vsapi->getFrameFilter(n, d->node, frameCtx);
        const VSVideoFormat *fi = vsapi->getVideoFrameFormat(src);
        int height = vsapi->getFrameHeight(src, 0);
        int width = vsapi->getFrameWidth(src, 0);


        VSFrame *dst = vsapi->newVideoFrame(fi, width, height, src, core);

        for (int plane = 0; plane < fi->numPlanes; plane++) {
            const float * VS_RESTRICT srcp = (const float *)vsapi->getReadPtr(src, plane);
            ptrdiff_t src_stride = vsapi->getStride(src, plane);
            float * VS_RESTRICT dstp = (float *)vsapi->getWritePtr(dst, plane);
            ptrdiff_t dst_stride = vsapi->getStride(dst, plane);
            int h = vsapi->getFrameHeight(src, plane);
            int w = vsapi->getFrameWidth(src, plane);

            cv::Mat inM(h, w, CV_32F), outM(h, w, CV_32F);
            for (int y = 0; y < h; y++) {
                memcpy(inM.data + inM.step * y, srcp, w * sizeof(float));
                srcp += src_stride / sizeof(float);
            }

            cv::dct(inM, outM, d->inverse);

            for (int y = 0; y < h; y++) {
                memcpy(dstp, outM.data + outM.step * y, w * sizeof(float));
                dstp += dst_stride / sizeof(float);
            }
        }

        vsapi->freeFrame(src);

        return dst;
    }

    return NULL;
}

void VS_CC dctFree(void *instanceData, VSCore *core, const VSAPI *vsapi) {
    DctData *d = (DctData *)instanceData;
    vsapi->freeNode(d->node);
    free(d);
}

void VS_CC dctCreate(const VSMap *in, VSMap *out, void *userData, VSCore *core, const VSAPI *vsapi) {
    DctData d;
    DctData *data;
    int err;

    d.node = vsapi->mapGetNode(in, "clip", 0, 0);
    const VSVideoInfo *vi = vsapi->getVideoInfo(d.node);

    if (!vsh::isConstantVideoFormat(vi) || vi->format.sampleType != stFloat || vi->format.bitsPerSample != 32) {
        vsapi->mapSetError(out, "dct: only constant format float32 input supported");
        vsapi->freeNode(d.node);
        return;
    }

    if (vi->width % 2 || vi->height % 2 || vi->width / (1 << vi->format.subSamplingW) % 2 || vi->height / (1 << vi->format.subSamplingH) % 2) {
        vsapi->mapSetError(out, "dct: odd dimension(s) not supported, check subsampling as well");
        vsapi->freeNode(d.node);
        return;
    }

    d.inverse = vsapi->mapGetInt(in, "inverse", 0, &err);
    if (err)
        d.inverse = 0;

    if (d.inverse < 0 || d.inverse > 1) {
        vsapi->mapSetError(out, "dct: inverse must be 0 or 1");
        vsapi->freeNode(d.node);
        return;
    }

    data = (DctData *)malloc(sizeof(d));
    *data = d;


    VSFilterDependency deps[] = {{d.node, rpStrictSpatial}};
    vsapi->createVideoFilter(out, "dct", vi, dctGetFrame, dctFree, fmParallel, deps, 1, data, core);
}
