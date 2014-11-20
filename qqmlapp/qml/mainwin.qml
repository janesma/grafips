import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import Grafips 1.0

ApplicationWindow {
    width: 600
    height: 500
    visible: true

    PublisherImpl {
        id: mainPublisher
    }

    PublisherImpl {
        id: secondPublisher
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

    CpuProvider {
        id: cpuProvider2
        publisher: secondPublisher
        Component.onCompleted: {
            start()
        }
        Component.onDestruction: {
            stop()
        }
    }

    Rectangle {
        id: connectionDialog
        anchors.fill: parent
        Component.onCompleted: visible = true
        Text {
            id: enterText
            width: 400
            height: 20
            anchors.centerIn: parent
            text: "Enter the address of the target system."
        }
        Rectangle {
            anchors.left: enterText.left
            anchors.right: enterText.right
            anchors.top: enterText.bottom
            border.color: "black"
            border.width: 1
            height: 20
            
            TextInput {
                anchors.fill: parent
                anchors.margins: 3
                verticalAlignment: Text.AlignVCenter
                text: "localhost"
                onAccepted: {
                    mainView.visible = true
                    connectionDialog.visible = false
                }
            }
        }
    }

    ColumnLayout
    {
        id: mainView
        anchors.fill: parent
        visible: false
        
        MetricGroup {
            id: cpu
            color: "red"
            publisher: mainPublisher
        }

        MetricGroup {
            id: network
            color: "blue"
            publisher: secondPublisher
        }

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
