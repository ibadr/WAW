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
