import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import Grafips 1.0

ApplicationWindow {
    width: 600
    height: 500
    visible: true

    PublisherImpl {
        id: mainPublisher
    }

    CpuProvider {
        id: cpuProvider
        publisher: mainPublisher
        Component.onCompleted: {
            start()
        }
        Component.onDestruction: {
            stop()
        }
    }

    ColumnLayout
    {
        anchors.fill: parent

        MetricGroup {
            id: cpu
            color: "red"
            publisher: mainPublisher
        }

        // MetricGroup {
        //     id: network
        //     color: "blue"
        //     publisher: mainPublisher
        // }

        Rectangle {
            Layout.fillWidth:  true
            Layout.fillHeight: true
            Layout.minimumHeight:20
            Layout.preferredHeight: 100
            color: "green"
        }
        Rectangle {
            Layout.fillWidth:  true
            Layout.fillHeight: true
            Layout.minimumHeight:20
            Layout.preferredHeight: 100
            color: "yellow"
        }
    }
}
