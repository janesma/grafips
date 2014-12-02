#pragma once

#include <string>
#include <vector>

#include <QString>
#include <QList>

#include "os/gftraits.h"

namespace Grafips
{
    class Pattern : NoCopy, NoAssign, NoMove
    {
      public:
        explicit Pattern(const QString &filter);
        bool Matches(const std::string &path) const;
      private:
        std::vector<std::string> m_tokens;
    };

    class PatternSet : NoCopy, NoAssign, NoMove
    {
      public:
        explicit PatternSet(const QList<QString> &filters);
        bool Matches(const std::string &path) const;
      private:
        std::vector<Pattern *> m_patterns;
    };
}

