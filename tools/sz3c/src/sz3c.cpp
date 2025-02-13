//
// Created by Kai Zhao on 10/27/22.
//

#include "sz3c.h"

#include <emscripten.h>

#include <iostream>

#include "SZ3/api/sz.hpp"

using namespace SZ3;

unsigned char *SZ_compress_args(int dataType, void *data, size_t *outSize, int errBoundMode, double absErrBound,
                                double relBoundRatio, double pwrBoundRatio, size_t r5, size_t r4, size_t r3, size_t r2,
                                size_t r1) {
    SZ3::Config conf;
    if (r2 == 0) {
        conf = SZ3::Config(r1);
    } else if (r3 == 0) {
        conf = SZ3::Config(r2, r1);
    } else if (r4 == 0) {
        conf = SZ3::Config(r3, r2, r1);
    } else if (r5 == 0) {
        conf = SZ3::Config(r4, r3, r2, r1);
    } else {
        conf = SZ3::Config(r5 * r4, r3, r2, r1);
    }
    //    conf.loadcfg(conPath);
    conf.absErrorBound = absErrBound;
    conf.relErrorBound = relBoundRatio;
    //    conf.pwrErrorBound = pwrBoundRatio;
    if (errBoundMode == ABS) {
        conf.errorBoundMode = EB_ABS;
    } else if (errBoundMode == REL) {
        conf.errorBoundMode = EB_REL;
    } else if (errBoundMode == ABS_AND_REL) {
        conf.errorBoundMode = EB_ABS_AND_REL;
    } else if (errBoundMode == ABS_OR_REL) {
        conf.errorBoundMode = EB_ABS_OR_REL;
    } else {
        printf("errBoundMode %d not support\n ", errBoundMode);
        exit(0);
    }

    unsigned char *cmpr_data = NULL;
    if (dataType == SZ_FLOAT) {
        cmpr_data = reinterpret_cast<unsigned char *>(SZ_compress<float>(conf, static_cast<float *>(data), *outSize));
#if (!SZ3_DEBUG_TIMINGS)
    } else if (dataType == SZ_DOUBLE) {
        cmpr_data = reinterpret_cast<unsigned char *>(SZ_compress<double>(conf, static_cast<double *>(data), *outSize));
#endif
    } else {
        printf("dataType %d not support\n", dataType);
        exit(0);
    }

    // convert c++ memory (by 'new' operator) to c memory (by malloc)
    auto *cmpr = static_cast<unsigned char *>(malloc(*outSize));
    memcpy(cmpr, cmpr_data, *outSize);
    delete[] cmpr_data;

    return cmpr;
}

extern "C" {
EMSCRIPTEN_KEEPALIVE
int add(int a, int b) { return a + b; }
}

void *SZ_decompress(int dataType, unsigned char *bytes, size_t byteLength, size_t r5, size_t r4, size_t r3, size_t r2,
                    size_t r1) {
    size_t n = 0;
    if (r2 == 0) {
        n = r1;
    } else if (r3 == 0) {
        n = r1 * r2;
    } else if (r4 == 0) {
        n = r1 * r2 * r3;
    } else if (r5 == 0) {
        n = r1 * r2 * r3 * r4;
    } else {
        n = r1 * r2 * r3 * r4 * r5;
    }

    SZ3::Config conf;
    if (dataType == SZ_FLOAT) {
        auto dec_data = static_cast<float *>(malloc(n * sizeof(float)));
        SZ_decompress<float>(conf, reinterpret_cast<char *>(bytes), byteLength, dec_data);
        return dec_data;
#if (!SZ3_DEBUG_TIMINGS)
    } else if (dataType == SZ_DOUBLE) {
        auto dec_data = static_cast<double *>(malloc(n * sizeof(double)));
        SZ_decompress<double>(conf, reinterpret_cast<char *>(bytes), byteLength, dec_data);
        return dec_data;
#endif
    } else {
        printf("dataType %d not support\n", dataType);
        exit(0);
    }
}

extern "C" {
EMSCRIPTEN_KEEPALIVE
void *exported_malloc(size_t size) { return malloc(size); }
}

// extern "C" {
// EMSCRIPTEN_KEEPALIVE
// void comp_test(double *data) {
//     SZ3::Config conf = SZ3::Config(48);
//     conf.cmprAlgo = SZ3::ALGO_INTERP_LORENZO;
//     conf.errorBoundMode = SZ3::EB_ABS;  // refer to def.hpp for all supported error bound mode
//     conf.absErrorBound = 1E-3;          // absolute error bound 1e-3
//
//     size_t outSize;
//     char *compressed = (SZ_compress<double>(conf, data, outSize));
//     // std::vector<double> out(conf.num);
//     // auto out_data = out.data();
//     SZ_decompress<double>(conf, compressed, outSize, data);
// }
// }

extern "C" {
EMSCRIPTEN_KEEPALIVE
void sz_compress_f64(double *data, char *compressed, size_t *comp_size) {
    SZ3::Config conf = SZ3::Config(150);
    conf.cmprAlgo = SZ3::ALGO_INTERP_LORENZO;
    conf.errorBoundMode = SZ3::EB_ABS;  // refer to def.hpp for all supported error bound mode
    conf.absErrorBound = 1E-3;          // absolute error bound 1e-3

    compressed = SZ_compress<double>(conf, data, *comp_size);
}
}

extern "C" {
EMSCRIPTEN_KEEPALIVE
void sz_decompress_f64(char *compressed, size_t comp_size, double *dec_data) {
    SZ3::Config conf = SZ3::Config(150);
    conf.cmprAlgo = SZ3::ALGO_INTERP_LORENZO;
    conf.errorBoundMode = SZ3::EB_ABS;  // refer to def.hpp for all supported error bound mode
    conf.absErrorBound = 1E-3;          // absolute error bound 1e-3

    SZ_decompress<double>(conf, compressed, comp_size, dec_data);
}
}

int main() {
    size_t outSize;
    double data[5] = {0.5678, 1.14567, 1.11111, 7.96969, 6.96969};

    // NOTE: What does the number sent into the constructor actually mean?
    SZ3::Config conf = SZ3::Config(48);
    conf.cmprAlgo = SZ3::ALGO_INTERP_LORENZO;
    conf.errorBoundMode = SZ3::EB_ABS;  // refer to def.hpp for all supported error bound mode
    conf.absErrorBound = 1E-3;          // absolute error bound 1e-3

    // unsigned char *compressed = NULL;
    char *compressed = (SZ_compress<double>(conf, data, outSize));
    std::vector<double> out(conf.num);
    auto out_data = out.data();
    SZ_decompress<double>(conf, compressed, outSize, out_data);

    printf("OutSize after compression: %d\n", outSize);
    for (int i = 0; i < 5; i++) {
        printf("%lf\n", out_data[i]);
    }
    return 0;
}
