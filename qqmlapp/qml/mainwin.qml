import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import Grafips 1.0

ApplicationWindow {
    width: 600
    height: 500
    visible: true

    MetricRouter {
        id: mainPublisher
    }


    Rectangle {
        id: connectionDialog
        anchors.fill: parent
        Component.onCompleted: visible = true
        Text {
            id: enterText
            width: 400
            anchors.centerIn: parent
            text: "Enter the address of the target system."
        }
        Rectangle {
            anchors.left: enterText.left
            anchors.right: enterText.right
            anchors.top: enterText.bottom
            border.color: "black"
            border.width: 1
            height: input.height + 5
            
            TextInput {
                id: input
                focus: true
                anchors.centerIn: parent
                width:parent.width
                verticalAlignment: Text.AlignVCenter
                text: "localhost:53136"
                onAccepted: {
                    cpu.start();
                    glFt.start();
                    glFps.start();
                    glGpu.start();
                    mainPublisher.address = text
                    mainView.visible = true
                    connectionDialog.visible = false
                }
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        id: row
        spacing: 0

        MetricList {
            id: currentList
            width: 200
            model: mainPublisher.metrics
            publisher: mainPublisher
        }

        ColumnLayout
        {
            id: mainView
            visible: false
            
            MetricGroup {
                id: cpu
                color: "red"
                publisher: mainPublisher
            }
            MetricGroup {
                id: glFt
                color: "red"
                publisher: mainPublisher
            }
            MetricGroup {
                id: glFps
                color: "red"
                publisher: mainPublisher
            }
            MetricGroup {
                id: glGpu
                color: "red"
                publisher: mainPublisher
            }
        }
    }
}
