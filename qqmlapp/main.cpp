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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <google/protobuf/stubs/common.h>

#include "controls/host/gfapi_qcontrol.h"
#include "controls/host/gfcontrol_host.h"
#include "controls/host/gfcpu_freq_qcontrol.h"
#include "graph/gfgraph_view.h"
#include "publishers/gfmetric_router.h"
#include "sources/gfcpu_source.h"
#include "subscriber/gfsubscriber.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Open Source Technology Center");
    app.setOrganizationDomain("intel.com");
    app.setApplicationName("grafips");

    qmlRegisterType<Grafips::MetricRouter>("Grafips", 1, 0, "MetricRouter");
    qmlRegisterType<Grafips::ControlRouterHost>("Grafips", 1, 0,
                                                "ControlRouterHost");

    qmlRegisterType<Grafips::CpuFreqControlModel>("Grafips", 1, 0,
                                                  "CpuFreqControlModel");
    qmlRegisterType<Grafips::ApiControlModel>("Grafips", 1, 0,
                                              "ApiControlModel");

    qmlRegisterInterface<Grafips::SubscriberInterface>("SubscriberInterface");
    qmlRegisterType<Grafips::GraphSetSubscriber>("Grafips", 1, 0,
                                                 "GraphSetSubscriber");

    qmlRegisterType<Grafips::QMetric>("Grafips", 1, 0, "QMetric");

    qmlRegisterType<Grafips::GraphView>("Grafips", 1, 0, "Renderer");

    QQmlApplicationEngine engine(QUrl("qrc:///qml/mainwin.qml"));
    int ret = app.exec();
    ::google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
