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

#define TA block_q8_0
#define TB block_q8_0
#define TC float

#include "sgemmer0.inc"

bool llamafile_sgemm_q0q0s_avxvnni(int m, int n, int k, const TA *A, int lda, const TB *B, int ldb,
                                   TC *C, int ldc, int ith, int nth, int task) {
    if (task != GGML_TASK_TYPE_COMPUTE)
        return true;
    SGEMMER0 tb{k, A, lda, B, ldb, C, ldc, ith, nth};
    tb.matmul(m, n);
    return true;
}

#endif // __x86_64__
