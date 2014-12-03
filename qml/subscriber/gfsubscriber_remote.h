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

#ifndef QML_SUBSCRIBER_GFSUBSCRIBER_REMOTE_H_
#define QML_SUBSCRIBER_GFSUBSCRIBER_REMOTE_H_

#include <QObject>

#include "os/gfthread.h"

namespace GrafipsProto {
class SubscriberInvocation;
class SubscriberInterface;
}

namespace Grafips {
class Socket;
class ServerSocket;
class SubscriberInterface;

class SubscriberSkeleton : public QObject, public Thread {
 public:
  SubscriberSkeleton(int port, SubscriberInterface *target);
  ~SubscriberSkeleton();
  void Run();
  void Stop();
  int GetPort() const;
 private:
  ServerSocket *m_server;
  Socket *m_socket;
  SubscriberInterface *m_target;
  bool m_running;
};
}  // namespace Grafips

#endif  // QML_SUBSCRIBER_GFSUBSCRIBER_REMOTE_H_
