// Copyright (C) Intel Corp.  2014.  All Rights Reserved.

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice (including the
// next paragraph) shall be included in all copies or substantial
// portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//  **********************************************************************/
//  * Authors:
//  *   Mark Janes <mark.a.janes@intel.com>
//  **********************************************************************/

#include "publishers/gfpattern.h"

#include <assert.h>
#include <QStringList>

#include <string>
#include <vector>

using Grafips::Pattern;
using Grafips::PatternSet;

Pattern::Pattern(const QString &filter) {
  QStringList tokens = filter.split("/");
  for (QStringList::const_iterator i = tokens.begin();
       i != tokens.end(); ++i) {
    if (i->length() > 0)
      m_tokens.push_back(i->toStdString());
  }
}

bool
Pattern::Matches(const std::string &path) const {
  std::vector<char> path_copy(path.length() + 1);
  strncpy(path_copy.data(), path.c_str(), path_copy.size());
  assert(path_copy[path.length()] == '\0');

  char *token = NULL, *saveptr = NULL;
  unsigned int index = 0;

  token = strtok_r(path_copy.data(), "/", &saveptr);

  while ( true ) {
    if (!token) {
      // got to the end of the path, so we have a match if all the
      // tokens matched
      return (index == m_tokens.size());
    }
    if (index >= m_tokens.size())
      // more path entries than the pattern
      return false;

    if (0 != strncmp(m_tokens[index].c_str(), "*", 1)) {
      // pattern specifies a non-wildcard for this token, so we must
      // compare
      if (0 != strcmp(m_tokens[index].c_str(), token))
        return false;
    }

    // else we have a match
    ++index;
    token = strtok_r(NULL, "/", &saveptr);
  }
  assert(false);
}

PatternSet::PatternSet(const QList<QString> &filters) {
  for (QList<QString>::const_iterator i = filters.begin();
       i != filters.end(); ++i) {
    m_patterns.push_back(new Pattern(*i));
  }
}

PatternSet::~PatternSet() {
  while (!m_patterns.empty()) {
    delete m_patterns.back();
    m_patterns.pop_back();
  }
}

bool
PatternSet::Matches(const std::string &path) const {
    for (std::vector<Pattern *>::const_iterator i = m_patterns.begin();
         i != m_patterns.end(); ++i) {
        if ((*i)->Matches(path))
            return true;
    }
    return false;
}

