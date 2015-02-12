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

#include "controls/gfcontrol.h"

#include <assert.h>

#include <string>

#include "controls/gfcontrol_stub.h"

using Grafips::ControlRouterTarget;
using Grafips::ControlRouterHost;
using Grafips::ControlStub;

ControlRouterTarget::ControlRouterTarget() {}

bool
ControlRouterTarget::Set(const std::string &key, const std::string &value) {
  auto i = m_targets.find(key);
  if (i == m_targets.end())
    return false;
  i->second->Set(key, value);
  return true;
}

void
ControlRouterTarget::AddControl(const std::string &key,
                                ControlInterface* target) {
  auto i = m_targets.find(key);
  assert(i == m_targets.end());
  m_targets[key] = target;
  target->Subscribe(this);
}

void
ControlRouterTarget::Subscribe(ControlSubscriberInterface *sub) {
  // might need to cache all publications, to send initial state on a
  // tardy subscribe
  m_subscriber = sub;
}

void
ControlRouterTarget::OnControlChanged(const std::string &key,
                                      const std::string &value) {
  if (m_subscriber)
    m_subscriber->OnControlChanged(key, value);
}


ControlRouterHost::ControlRouterHost(const std::string &address, int port)
    : m_stub(new ControlStub(address, port)) {
  m_stub->Subscribe(this);
}

ControlRouterHost::~ControlRouterHost() {
  delete m_stub;
}

void
ControlRouterHost::Subscribe(const std::string &key,
                             ControlSubscriberInterface *value) {
  m_subscribers[key] = value;
}

void
ControlRouterHost::OnControlChanged(const std::string &key,
                                    const std::string &value) {
  auto target = m_subscribers.find(key);
  if (target == m_subscribers.end())
    return;
  target->second->OnControlChanged(key, value);
}
