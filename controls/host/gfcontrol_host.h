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

#ifndef CONTROLS_HOST_GFCONTROL_HOST_H_
#define CONTROLS_HOST_GFCONTROL_HOST_H_

#include <QObject>
#include <QString>

#include <map>
#include <string>

#include "controls/gficontrol.h"
#include "os/gftraits.h"

namespace Grafips {

class ControlStub;

// multiplexes notifications on the UI side of the socket.  UI
// controls subscribe for the specific control state that they
// display.
class ControlRouterHost : public QObject,
                          public ControlSubscriberInterface,
                          NoCopy, NoAssign, NoMove {
  Q_OBJECT
  Q_PROPERTY(QString address READ address WRITE setAddress)

 public:
  ControlRouterHost();
  ~ControlRouterHost();
  bool Set(const std::string &key, const std::string &value);
  void Subscribe(const std::string &key,
                 ControlSubscriberInterface *value);

  void OnControlChanged(const std::string &key,
                        const std::string &value);
  void Flush();
  void setAddress(const QString& address);
  const QString &address();
 private:
  QString m_address;
  std::map<std::string, ControlSubscriberInterface *> m_subscribers;
  std::map<std::string, std::string> m_current_state;

  ControlStub *m_stub;
};

}  // namespace Grafips

#endif  // CONTROLS_HOST_GFCONTROL_HOST_H_
