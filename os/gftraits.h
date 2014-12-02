#pragma once

namespace Grafips
{
    class NoCopy
    {
      public:
        NoCopy(){}
      private:
        NoCopy(const NoCopy&);
    };
    class NoAssign
    {
      public:
        NoAssign(){}
      private:
        NoAssign &operator=(const NoAssign&);
    };
    class NoMove
    {
      public:
        NoMove() {}
      private:
        NoMove(NoMove &&);
    };
}
