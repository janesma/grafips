import QtQuick 2.0
import Grafips 1.0

Item {
    id: currentList
    property var model
    property MetricRouter publisher
    property ActiveMetrics activeMetrics
    property int contentHeight: metricList.contentHeight

    // draggable item needs to be parented in the item containing drag
    // targets, or else it will be clipped as it is drug out of the
    // ListView
    property var dragParent

    ListView {
        anchors.fill: parent
        model: currentList.model
        id: metricList

        delegate: Rectangle {
            id: currentRect
            height: nameText.height + 15
            width: currentList.width
            property int theId: model.met_id
            
            color: "light blue"
            Text {
                id: nameText
                width: parent.width
                wrapMode: Text.WordWrap
                text: name
            }
            Rectangle {
                id: info_rect
                border.width: 1
                color: "light blue"
                width: nameText.height
                height: nameText.height
                anchors.top: nameText.top
                anchors.right: nameText.right
                Text {
                    anchors.centerIn: parent
                    text: "?"
                }
            }
            
            Text {
                id: help
                text: helpText
                visible: false
                width: parent.width
                anchors.top: nameText.bottom
                wrapMode: Text.WordWrap
            }
            Rectangle {
                // copies the delegate exactly, with some alpha.  This
                // leaves the original item in place during drag, but
                // propogates the drag event to the target.
                id: dragVisualizer
                height: currentRect.height
                width: currentRect.width
                // must be reparented so it can be drug outside the ListView
                parent: dragParent
                color: currentRect.color
                visible: false
                opacity: 0.5
                property int theId: model.met_id
                property string name: model.name

                Text {
                    id: name_text
                    text: name
                }

                // manually drive the drag events, because qml seems
                // to have a bug here.
                // http://stackoverflow.com/questions/24532317/new-drag-and-drop-mechanism-does-not-work-as-expected-in-qt-quick-qt-5-3
                property bool dragActive: elementMouse.drag.active
                onDragActiveChanged: {
                    if (dragActive) {
                        // show the drag visualizer
                        visible = true
                        Drag.start();
                        return;
                    }
                    Drag.drop();
                    visible = false
                }

            }
            MouseArea {
                id: elementMouse
                anchors.fill: parent
                drag.target: dragVisualizer
                drag.filterChildren: true
                hoverEnabled: true
                onPressed : {
                    // reposition the drag visualizer so it overlays
                    // the selected item
                    print("drag pressed")
                    dragVisualizer.x = currentRect.mapToItem(dragVisualizer.parent).x;
                    dragVisualizer.y = currentRect.mapToItem(dragVisualizer.parent).y;
                }
            }
            MouseArea {
                anchors.fill: info_rect
                onPressed : {
                    help.visible =  ! help.visible
                    currentRect.height += help.visible ? help.height : - help.height
                    print("info pressed")
                    metricList.visible = false
                    metricList.visible = true
                    z: 200
                }
            }
        }
    }
}
