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

#include "controls/host/gfcpu_freq_qcontrol.h"

#include <assert.h>

#include <string>

using Grafips::CpuFreqControlModel;

CpuFreqControlModel::CpuFreqControlModel()
    : m_currentSelection(-1) {
  // perhaps need to prepopulate the m_policies list

  connect(this, SIGNAL(NotifyPolicies()),
          this, SLOT(HandleNotifyPolicies()));
}

void
CpuFreqControlModel::OnControlChanged(const std::string &key,
                                      const std::string &value) {
  ScopedLock l(&m_protect);
  // fill out the m_policies list, if this is the first publication
  // (indicating root access)
  if (key != "CpuFrequencyPolicy")
    return;

  m_currentSelection = m_policies.indexOf(QString::fromStdString(value));

  emit NotifyPolicies();
}

void
CpuFreqControlModel::SetControlRounter(ControlRouterHost *router) {
  ScopedLock l(&m_protect);
  m_router = router;
  m_router->Subscribe("CpuFrequencyPolicy", this);
}

void
CpuFreqControlModel::SetPolicy(QString policy) {
  ScopedLock l(&m_protect);
  assert(m_router);
  m_router->Set("CpuFrequencyPolicy", policy.toStdString());
}

void
CpuFreqControlModel::HandleNotifyPolicies() {
  ScopedLock l(&m_protect);
  if (m_policies.empty()) {
    m_policies.append("performance");
    m_policies.append("powersave");
    m_policies.append("min_freq");
    m_policies.append("max_freq");
    emit(onPolicies());
  }
  emit(onPolicyChanged());
}

QStringList
CpuFreqControlModel::policies() {
  return m_policies;
}

int
CpuFreqControlModel::currentPolicy() {
  return m_currentSelection;
}
