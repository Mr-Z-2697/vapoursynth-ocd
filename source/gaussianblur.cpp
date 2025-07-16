#include "gaussianblur.h"

const VSFrame *VS_CC gblurGetFrame(int n, int activationReason, void *instanceData, void **frameData, VSFrameContext *frameCtx, VSCore *core, const VSAPI *vsapi) {
    GblurData *d = (GblurData *)instanceData;

    if (activationReason == arInitial) {
        vsapi->requestFrameFilter(n, d->node, frameCtx);
    } else if (activationReason == arAllFramesReady) {
        const VSFrame *src = vsapi->getFrameFilter(n, d->node, frameCtx);
        const VSVideoFormat *fi = vsapi->getVideoFrameFormat(src);
        int height = vsapi->getFrameHeight(src, 0);
        int width = vsapi->getFrameWidth(src, 0);


        VSFrame *dst = vsapi->newVideoFrame(fi, width, height, src, core);

        for (int plane = 0; plane < fi->numPlanes; plane++) {
            const uint8_t * VS_RESTRICT srcp = (const uint8_t *)vsapi->getReadPtr(src, plane);
            ptrdiff_t src_stride = vsapi->getStride(src, plane);
            uint8_t * VS_RESTRICT dstp = (uint8_t *)vsapi->getWritePtr(dst, plane);
            ptrdiff_t dst_stride = vsapi->getStride(dst, plane);
            int h = vsapi->getFrameHeight(src, plane);
            int w = vsapi->getFrameWidth(src, plane);

            int sampleSize, cvFMT;
            switch (fi->bitsPerSample) {
                case 8:
                    sampleSize = sizeof(uint8_t);
                    cvFMT = CV_8U;
                    break;
                case 16:
                    sampleSize = sizeof(uint16_t);
                    cvFMT = CV_16U;
                    break;
                case 32:
                    sampleSize = sizeof(float);
                    cvFMT = CV_32F;
                    break;
            }

            cv::Mat inM(h, w, cvFMT), outM(h, w, cvFMT);
            for (int y = 0; y < h; y++) {
                memcpy(inM.data + inM.step * y, srcp, w * sampleSize);
                srcp += src_stride;
            }

            if (w != width || h != height) {
                int sizeXS = int(d->sizeX * ((double)w / width)) | 1, sizeYS = int(d->sizeY * ((double)h / height)) | 1;
                cv::GaussianBlur(inM, outM, cv::Size(sizeXS, sizeYS), d->sigmaX * ((double)w / width), d->sigmaY * ((double)h / height), d->borderType, cv::ALGO_HINT_ACCURATE);
            }
            else
                cv::GaussianBlur(inM, outM, cv::Size(d->sizeX, d->sizeY), d->sigmaX, d->sigmaY, d->borderType, cv::ALGO_HINT_ACCURATE);

            for (int y = 0; y < h; y++) {
                memcpy(dstp, outM.data + outM.step * y, w * sampleSize);
                dstp += dst_stride;
            }
        }

        vsapi->freeFrame(src);

        return dst;
    }

    return NULL;
}

void VS_CC gblurFree(void *instanceData, VSCore *core, const VSAPI *vsapi) {
    GblurData *d = (GblurData *)instanceData;
    vsapi->freeNode(d->node);
    free(d);
}

void VS_CC gblurCreate(const VSMap *in, VSMap *out, void *userData, VSCore *core, const VSAPI *vsapi) {
    GblurData d;
    GblurData *data;
    int err;

    d.node = vsapi->mapGetNode(in, "clip", 0, 0);
    const VSVideoInfo *vi = vsapi->getVideoInfo(d.node);

    if (!vsh::isConstantVideoFormat(vi) ||
         !(vi->format.sampleType == stFloat && vi->format.bitsPerSample == 32) &&
         !(vi->format.sampleType == stInteger && (vi->format.bitsPerSample == 8 || vi->format.bitsPerSample == 16))) {
        vsapi->mapSetError(out, "ocd.gaussianblur: only constant format uint8 uint16 float32 input supported");
        vsapi->freeNode(d.node);
        return;
    }

    d.sigmaX = vsapi->mapGetFloat(in, "sigmax", 0, &err);
    if (err)
        d.sigmaX = 1.;

    if (d.sigmaX < 0) {
        vsapi->mapSetError(out, "ocd.gaussianblur: sigmaX must be >= 0");
        vsapi->freeNode(d.node);
        return;
    }

    d.sigmaY = vsapi->mapGetFloat(in, "sigmay", 0, &err);
    if (err)
        d.sigmaY = 0.;

    if (d.sigmaY < 0) {
        vsapi->mapSetError(out, "ocd.gaussianblur: sigmaY must be >= 0");
        vsapi->freeNode(d.node);
        return;
    }

    d.sizeX = vsapi->mapGetInt(in, "sizex", 0, &err);
    if (err)
        d.sizeX = 0;
    d.sizeY = vsapi->mapGetInt(in, "sizey", 0, &err);
    if (err)
        d.sizeY = 0;

    if (!(d.sizeX == 0 && d.sizeY == 0) && (d.sizeX < 0 || d.sizeY < 0 || !(d.sizeX % 2) || !(d.sizeY % 2))) {
        vsapi->mapSetError(out, "ocd.gaussianblur: sizeX and sizeY must be positive and odd, or both zero");
        vsapi->freeNode(d.node);
        return;
    }

    if (d.sigmaX == 0. && d.sigmaY == 0. && d.sizeX == 0 && d.sizeY == 0) {
        vsapi->mapSetError(out, "ocd.gaussianblur: size and sigma can't be all zero's");
        vsapi->freeNode(d.node);
        return;
    }

    d.borderType = vsapi->mapGetInt(in, "bordertype", 0, &err);
    if (err)
        d.borderType = 4;

    if (d.borderType != 0 && d.borderType != 1 && d.borderType != 2 && d.borderType != 4 && d.borderType != 16) {
        vsapi->mapSetError(out, "ocd.gaussianblur: specified borderType is not supported");
        vsapi->freeNode(d.node);
        return;
    }

    data = (GblurData *)malloc(sizeof(d));
    *data = d;


    VSFilterDependency deps[] = {{d.node, rpStrictSpatial}};
    vsapi->createVideoFilter(out, "gaussianblur", vi, gblurGetFrame, gblurFree, fmParallel, deps, 1, data, core);
}
