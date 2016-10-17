// Copyright (c) 2016 Islam Badreldin
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
//
// ------------------------------------------------------------------

#ifndef __WAW_H__
#define __WAW_H__

#include <digilent/waveforms/dwf.h> // already got extern "C"
#include <vector>

class WAW
{
public:
  static WAW & instance() {
    static WAW instance;
    return instance;
  }
  ~WAW() {}

  int enumDevices(ENUMFILTER enumFilter) {
    int cDevice;
    BOOL succ;
    succ = FDwfEnum(enumfilterDiscovery, &cDevice);
    return succ ? cDevice : -1;
  }

private:
  WAW() {}
  WAW(WAW const &) {}
  void operator=(WAW const &) {}

  // std::vector<HDWF> _hdwfVec;
  // std::vector<int> _clientsVec;
};

#endif
