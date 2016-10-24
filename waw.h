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

#ifdef WIN32
#include <dwf.h> // already got extern "C"
#else
#include <digilent/waveforms/dwf.h> // already got extern "C"
#endif

#include <map>

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
    succ = FDwfEnum(enumFilter, &cDevice);
    return succ ? cDevice : -1;
  }

  bool openDevice(int idx, ENUMFILTER enumFilter = enumfilterAll) {
  	bool isSuccess = false;
  	if (idx < 0) {
  		return isSuccess;
  	}

    std::map<int,int>::iterator itr = _numClientsMap.find(idx);
    if (itr == _numClientsMap.end() || itr->second == 0) {// first encounter,
                                // or second encounter with zero clients
    	int cDevice; BOOL succ;
    	succ = FDwfEnum(enumfilterAll, &cDevice); // enumerate first to help with checking isInUse next
      if (!succ)
        return isSuccess;

    	BOOL isInUse;
    	succ = FDwfEnumDeviceIsOpened(idx, &isInUse);
      if (!succ)
        return isSuccess;
    	if (isInUse) {
    		return isSuccess;
    	}
      HDWF hdwf;
    	isSuccess = (FDwfDeviceOpen(idx, &hdwf)>0) ? true : false;
      isSuccess = isSuccess && (hdwf>0);

      if (isSuccess) {
        if (itr == _numClientsMap.end()) {
          _numClientsMap.insert(std::pair<int,int>(idx,1));
          _hdwfMap.insert(std::pair<int,HDWF>(idx,hdwf));
        } else {
          _numClientsMap[idx] += 1;
          _hdwfMap[idx] = hdwf;
        }
      }
    } else {
      _numClientsMap[idx] += 1; // idx is already open under our control
      isSuccess = true;
    }
  	return isSuccess;
  }

  HDWF hdwfDevice(int idx) {
    HDWF hdwf = -1;
    std::map<int,int>::iterator itr = _numClientsMap.find(idx);
    if (itr != _numClientsMap.end() && itr->second > 0) {
      std::map<int,HDWF>::iterator itrH = _hdwfMap.find(idx);
      if (itrH != _hdwfMap.end() ) {
        hdwf = itrH->second;
      }
    }
    return hdwf;
  }

  int clientsDevice(int idx) {
    int clients = -1;
    std::map<int,int>::iterator itr = _numClientsMap.find(idx);
    if (itr != _numClientsMap.end()) {
      clients = itr->second;
    }
    return clients;
  }

  bool closeDevice(int idx) {
    bool isSuccess = false;
    std::map<int,int>::iterator itr = _numClientsMap.find(idx);
    if (itr != _numClientsMap.end() && itr->second > 0) {
      itr->second -= 1;
      if (itr->second > 0)
        isSuccess = true;
      else { // numClients = 0 so close hardware
        std::map<int,HDWF>::iterator itrH = _hdwfMap.find(idx);
        if (itrH != _hdwfMap.end() ) {
          HDWF hdwf = itrH->second;
          BOOL succ;
          succ = FDwfDeviceClose(hdwf);
          isSuccess = (succ>0) ? true : false;
          if (!isSuccess)
            itr->second += 1; // this client is still registered because
              // device was not closed
        }
      }
    }
    return isSuccess;
  }

  void clearDeviceMaps() {
    _numClientsMap.clear();
    _hdwfMap.clear();
  }

private:
  WAW() {}
  WAW(WAW const &) {}
  void operator=(WAW const &) {}

  std::map<int,HDWF> _hdwfMap;
  std::map<int,int> _numClientsMap;
};

#endif
