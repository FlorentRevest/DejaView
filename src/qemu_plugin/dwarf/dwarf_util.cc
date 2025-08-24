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

#include "dwarf_util.h"
#include "../qemu_helpers.h"

using string_view = std::string_view;

namespace dwarf {

// uint64/32 max is a tombstone value added by https://reviews.llvm.org/D81784.
bool IsValidDwarfAddress(uint64_t addr, uint8_t address_size) {
  if (addr == 0)
    return false;
  if (address_size == 4 && addr == std::numeric_limits<uint32_t>::max())
    return false;
  if (address_size == 8 && addr == std::numeric_limits<uint64_t>::max())
    return false;
  return true;
}

uint64_t ReadLEB128Internal(bool is_signed, string_view* data) {
  uint64_t ret = 0;
  int shift = 0;
  int maxshift = 70;
  const char* ptr = data->data();
  const char* limit = ptr + data->size();

  while (ptr < limit && shift < maxshift) {
    char byte = *(ptr++);
    ret |= static_cast<uint64_t>(byte & 0x7f) << shift;
    shift += 7;
    if ((byte & 0x80) == 0) {
      data->remove_prefix(static_cast<size_t>(ptr - data->data()));
      if (is_signed && shift < 64 && (byte & 0x40)) {
        ret |= -(1ULL << shift);
      }
      return ret;
    }
  }

  QEMU_LOG() << "corrupt DWARF data, unterminated LEB128\n";
  exit(1);
}

void SkipLEB128(string_view* data) {
  size_t limit =
      std::min(static_cast<size_t>(data->size()), static_cast<size_t>(10));
  for (size_t i = 0; i < limit; i++) {
    if (((*data)[i] & 0x80) == 0) {
      data->remove_prefix(i + 1);
      return;
    }
  }

  QEMU_LOG() << "corrupt DWARF data, unterminated LEB128\n";
  exit(1);
}

std::string_view ReadDebugStrEntry(std::string_view section, size_t ofs) {
  SkipBytes(ofs, &section);
  return ReadNullTerminated(&section);
}

}  // namespace dwarf
