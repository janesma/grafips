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

#ifndef CONTROLS_HOST_GFCPU_FREQ_QCONTROL_H_
#define CONTROLS_HOST_GFCPU_FREQ_QCONTROL_H_

#include <QList>
#include <QObject>
#include <QString>
#include <QVariant>
#include <string>

#include "controls/gficontrol.h"
#include "controls/host/gfcontrol_host.h"
#include "os/gfmutex.h"
#include "os/gftraits.h"

namespace Grafips {
class CpuFreqControlModel : public QObject,
                            public ControlSubscriberInterface,
                            NoCopy, NoAssign, NoMove {
  Q_OBJECT
  Q_PROPERTY(QStringList policies
             READ policies NOTIFY onPolicies)
  Q_PROPERTY(int currentPolicy
             READ currentPolicy NOTIFY onPolicyChanged)
 public:
  CpuFreqControlModel();
  ~CpuFreqControlModel() {}
  Q_INVOKABLE void SetControlRounter(ControlRouterHost *router);
  Q_INVOKABLE void SetPolicy(QString policy);
  void OnControlChanged(const std::string &key,
                        const std::string &value);

  // property accessors
  QStringList policies();
  int currentPolicy();

 signals:
  void NotifyPolicies();
  void onPolicies();
  void onPolicyChanged();

 public slots:
  // recives NotifyPolicies() signal on the UI thread, then emits
  // onEnabled for MetricList.qml to recieve
  void HandleNotifyPolicies();

 private:
  ControlRouterHost *m_router;
  QList<QString> m_policies;
  int m_currentSelection;
  mutable Mutex m_protect;
};
}  // namespace Grafips

#endif  // CONTROLS_HOST_GFCPU_FREQ_QCONTROL_H_
