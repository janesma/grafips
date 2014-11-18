#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <google/protobuf/stubs/common.h>

#include "gfcpu_provider.h"
#include "gfpublisher.h"
#include "gfsubscriber.h"
#include "gfgraph_view.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Grafips::CpuProvider>("Grafips", 1, 0, "CpuProvider");

    qmlRegisterType<Grafips::PublisherInterface>();
    qmlRegisterType<Grafips::PublisherImpl>("Grafips", 1, 0, "PublisherImpl");

    qmlRegisterInterface<Grafips::SubscriberInterface>("SubscriberInterface");
    qmlRegisterType<Grafips::GraphSetSubscriber>("Grafips", 1, 0, "GraphSetSubscriber");

    qmlRegisterType<Grafips::QMetric>("Grafips", 1, 0, "QMetric");

    qmlRegisterType<Grafips::GraphView>("Grafips", 1, 0, "Renderer");
    
    QQmlApplicationEngine engine(QUrl("qrc:///qml/mainwin.qml"));
    int ret = app.exec();
    ::google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
