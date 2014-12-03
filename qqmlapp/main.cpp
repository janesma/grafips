#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <google/protobuf/stubs/common.h>

#include "gfcpu_source.h"
#include "publishers/gfpublisher_remote.h"
#include "gfsubscriber.h"
#include "graph/gfgraph_view.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Grafips::PublisherStub>("Grafips", 1, 0, "PublisherStub");

    qmlRegisterInterface<Grafips::SubscriberInterface>("SubscriberInterface");
    qmlRegisterType<Grafips::GraphSetSubscriber>("Grafips", 1, 0, "GraphSetSubscriber");

    qmlRegisterType<Grafips::QMetric>("Grafips", 1, 0, "QMetric");

    qmlRegisterType<Grafips::GraphView>("Grafips", 1, 0, "Renderer");
    
    QQmlApplicationEngine engine(QUrl("qrc:///qml/mainwin.qml"));
    int ret = app.exec();
    ::google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
