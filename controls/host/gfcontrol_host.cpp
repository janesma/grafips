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

#include "controls/host/gfcontrol_host.h"

#include <assert.h>

#include <QStringList>
#include <string>

#include "controls/gfcontrol_stub.h"

using Grafips::ControlRouterTarget;
using Grafips::ControlRouterHost;
using Grafips::ControlStub;

ControlRouterHost::ControlRouterHost()
    : m_stub(NULL) {
}

ControlRouterHost::~ControlRouterHost() {
  delete m_stub;
}

void
ControlRouterHost::setAddress(const QString& address) {
  m_address = address;
  const int colon_pos = m_address.indexOf(":");
  int port;
  std::string host;
  if (colon_pos != -1) {
    QStringList l = address.split(":");
    host = l[0].toStdString();
    port = l[1].toInt() + 1;
  }
  m_stub = new ControlStub(host, port);
  m_stub->Subscribe(this);
}

const QString &
ControlRouterHost::address() {
  return m_address;
}

bool
ControlRouterHost::Set(const std::string &key, const std::string &value) {
  m_stub->Set(key, value);
  return true;
}

void
ControlRouterHost::Subscribe(const std::string &key,
                             ControlSubscriberInterface *value) {
  m_subscribers[key] = value;
  auto state = m_current_state.find(key);
  if (state == m_current_state.end())
    return;
  value->OnControlChanged(key, state->second);
}

void
ControlRouterHost::Flush() {
  m_stub->Flush();
}

void
ControlRouterHost::OnControlChanged(const std::string &key,
                                    const std::string &value) {
  m_current_state[key] = value;
  auto target = m_subscribers.find(key);
  if (target == m_subscribers.end())
    return;
  target->second->OnControlChanged(key, value);
}
