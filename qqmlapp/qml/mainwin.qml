import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import Grafips 1.0
import Qt.labs.settings 1.0

ApplicationWindow {
    width: 600
    height: 500
    visible: true
    id: mainWindow

    Settings {
        property alias x: mainWindow.x
        property alias y: mainWindow.y
        property alias width: mainWindow.width
        property alias height: mainWindow.height
    }

    MetricRouter {
        id: mainPublisher
    }
    ControlRouterHost {
        id: controlRouter
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

                Settings {
                    property alias address: input.text
                }

                onAccepted: {
                    mainPublisher.address = text
                    controlRouter.address = text
                    row.visible = true
                    connectionDialog.visible = false

                    // these graphs will activate metrics from the
                    // previous settings as soon as they are started.
                    // This needs to happen after controlRouter /
                    // mainPublisher get their addresses, which will
                    // open the sockets.
                    cpu.start();
                    glFt.start();
                    glFps.start();
                    glGpu.start();
                }
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        id: row
        visible: false
        spacing: 0


        ColumnLayout {
            Layout.preferredWidth: 500
            Layout.alignment: Qt.AlignTop
            MetricList {
                id: currentList
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: mainPublisher.metrics
                publisher: mainPublisher
                dragParent: mainWindow.contentItem
            }
            Rectangle {
                height: 20
            }
            CpuFreqPolicy {
                id: frequencyControl
                implicitHeight: frequencyControl.suggested_height
                Layout.fillWidth: true
                Component.onCompleted: {
                    frequencyControl.model.SetControlRounter(controlRouter)
                }
            }
            ApiControl {
                id: apiControl
                implicitHeight: apiControl.suggested_height
                Layout.fillWidth: true
                Component.onCompleted: {
                    apiControl.model.SetControlRounter(controlRouter)
                    apiControl.metricsRouter = mainPublisher
                }
            }
        }
        ColumnLayout
        {
            Layout.preferredWidth: 1500
            id: mainView
            
            MetricGroup {
                id: cpu
                publisher: mainPublisher
                // offset is used to address the settings, which hold
                // the active metrics for the graph
                offset: "1"
            }
            MetricGroup {
                id: glFt
                publisher: mainPublisher
                // offset is used to address the settings, which hold
                // the active metrics for the graph
                offset: "2"
            }
            MetricGroup {
                id: glFps
                publisher: mainPublisher
                // offset is used to address the settings, which hold
                // the active metrics for the graph
                offset: "3"
            }
            MetricGroup {
                id: glGpu
                publisher: mainPublisher
                // offset is used to address the settings, which hold
                // the active metrics for the graph
                offset: "4"
            }
        }
    }
}
