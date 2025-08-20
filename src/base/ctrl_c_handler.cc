/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dejaview/ext/base/ctrl_c_handler.h"

#include "dejaview/base/build_config.h"
#include "dejaview/base/compiler.h"
#include "dejaview/base/logging.h"

#if DEJAVIEW_BUILDFLAG(DEJAVIEW_OS_WIN)
#include <Windows.h>
#include <io.h>
#else
#include <signal.h>
#include <unistd.h>
#endif

namespace dejaview {
namespace base {

namespace {
CtrlCHandlerFunction g_handler = nullptr;
}

void InstallCtrlCHandler(CtrlCHandlerFunction handler) {
  DEJAVIEW_CHECK(g_handler == nullptr);
  g_handler = handler;

#if DEJAVIEW_BUILDFLAG(DEJAVIEW_OS_WIN)
  auto trampoline = [](DWORD type) -> int {
    if (type == CTRL_C_EVENT) {
      g_handler();
      return true;
    }
    return false;
  };
  ::SetConsoleCtrlHandler(trampoline, true);
#elif DEJAVIEW_BUILDFLAG(DEJAVIEW_OS_LINUX) || \
    DEJAVIEW_BUILDFLAG(DEJAVIEW_OS_ANDROID) || \
    DEJAVIEW_BUILDFLAG(DEJAVIEW_OS_APPLE)
  // Setup signal handler.
  struct sigaction sa {};

// Glibc headers for sa_sigaction trigger this.
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma GCC diagnostic ignored "-Wdisabled-macro-expansion"
#endif
  sa.sa_handler = [](int) { g_handler(); };
  sa.sa_flags = static_cast<decltype(sa.sa_flags)>(SA_RESETHAND | SA_RESTART);
#pragma GCC diagnostic pop
  sigaction(SIGINT, &sa, nullptr);
  sigaction(SIGTERM, &sa, nullptr);
#else
  // Do nothing on NaCL and Fuchsia.
  ignore_result(handler);
#endif
}

}  // namespace base
}  // namespace dejaview
