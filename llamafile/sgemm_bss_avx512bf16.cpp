// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
// vi: set et ft=cpp ts=4 sts=4 sw=4 fenc=utf-8 :vi
//
// Copyright 2024 Mozilla Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifdef __x86_64__

#include "sgemm.h"
#include "varith.h"
#include <immintrin.h>

#define KN 32

#define D __m512
#define V __m512bh
#define TA ggml_bf16_t
#define TB float
#define TC float

static inline __m512bh load(const ggml_bf16_t *p) {
    return (__m512bh)_mm512_loadu_ps((const float *)p);
}

static inline __m512bh load(const float *p) {
    return _mm512_cvtne2ps_pbh(_mm512_loadu_ps(p + 16), _mm512_loadu_ps(p));
}

template <> inline __m512 madd(__m512bh x, __m512bh y, __m512 z) {
    return _mm512_dpbf16_ps(z, x, y);
}

template <> inline __m512 madder(__m512bh x, __m512bh y, __m512 z, __m512 *e) {
    return _mm512_dpbf16_ps(z, x, y);
}

#include "sgemmer.inc"

bool llamafile_sgemm_bss_avx512bf16(int m, int n, int k, const TA *A, int lda, const TB *B, int ldb,
                                    TC *C, int ldc, int ith, int nth, int task) {
    if (task != GGML_TASK_TYPE_COMPUTE)
        return true;
    SGEMMER tb{k, A, lda, B, ldb, C, ldc, ith, nth};
    tb.matmul(m, n);
    return true;
}

#endif // __x86_64__
