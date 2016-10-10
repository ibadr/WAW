#ifndef __WAW_H__
#define __WAW_H__

class WAW
{
public:
  static WAW & instance()
  {
    static WAW instance;
    return instance;
  }

private:
  WAW();
  WAW(WAW const &);
  void operator=(WAW const &);
};

#endif
