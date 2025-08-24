// Copyright 2016 Google Inc. All Rights Reserved.
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

#include "util.h"

#include "dejaview/base/compiler.h"
#include "../qemu_helpers.h"

using std::string_view;

std::string_view ReadUntilConsuming(std::string_view* data, char c) {
  std::string_view ret = ReadUntil(data, c);

  if (data->empty() || data->front() != c) {
    // Nothing left, meaning we didn't find the terminating character.
    if (c == '\0') {
      QEMU_LOG() << "string is not NULL-terminated\n";
      exit(1);
    }
    QEMU_LOG() << "could not find terminating character '" << c << "'\n";
    exit(1);
  }

  data->remove_prefix(1);  // Remove the terminating character also.
  return ret;
}

std::string_view ReadUntil(std::string_view* data, char c) {
  const char* found =
      static_cast<const char*>(memchr(data->data(), c, data->size()));

  size_t len = (found == NULL) ? data->size() : static_cast<size_t>(found - data->data());
  std::string_view val = data->substr(0, len);
  data->remove_prefix(len);
  return val;
}
