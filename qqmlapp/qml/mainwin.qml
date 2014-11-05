import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import Grafips 1.0

ApplicationWindow {
    width: 600
    height: 200
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
    }

    ColumnLayout
    {
        anchors.fill: parent
        //orientation: Qt.Vertical
        MetricGroup {
            id: cpu
            color: "red"
            publisher: mainPublisher
        }

        MetricGroup {
            id: network
            color: "blue"
            publisher: mainPublisher
        }

        // Rectangle {
        //     Layout.fillWidth:  true
        //     Layout.fillHeight: true
        //     Layout.minimumHeight:20
        //     Layout.preferredHeight: 100
        //     color: "blue"
        //     visible: true
        //     MouseArea {
        //         anchors.fill: parent
        //         acceptedButtons: Qt.LeftButton | Qt.RightButton
        //         onClicked: {
        //             parent.Layout.maximumHeight= 20
        //             parent.visible = false
        //             parent.visible = true
        //         }
        //         onWheel: {
        //             parent.Layout.preferredHeight += wheel.angleDelta.y
        //             parent.visible = false
        //             parent.visible = true                    
        //         }
        //     }
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
