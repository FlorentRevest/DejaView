// Copyright (C) 2023 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SRC_TRACE_PROCESSOR_DEJAVIEW_SQL_INTRINSICS_FUNCTIONS_MATH_H_
#define SRC_TRACE_PROCESSOR_DEJAVIEW_SQL_INTRINSICS_FUNCTIONS_MATH_H_

#include "dejaview/base/status.h"

namespace dejaview::trace_processor {

class DejaViewSqlEngine;

// Registers LN, EXP, and SQRT.
// We do not compile the SQLite library with -DSQLITE_ENABLE_MATH_FUNCTIONS so
// these functions are not provided by default.
base::Status RegisterMathFunctions(DejaViewSqlEngine& engine);

}  // namespace dejaview::trace_processor

#endif  // SRC_TRACE_PROCESSOR_DEJAVIEW_SQL_INTRINSICS_FUNCTIONS_MATH_H_
