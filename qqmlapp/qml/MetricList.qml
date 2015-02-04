import QtQuick 2.0
import Grafips 1.0

Item {
    id: currentList
    property var model
    property MetricRouter publisher
    property ActiveMetrics activeMetrics
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    ListView {
        anchors.fill: parent
        model: currentList.model
        id: metricList

        delegate: Rectangle {
            id: currentRect
            height: nameText.height * 1.5
            width: currentList.width
            property int theId: model.met_id
            
            color: "light blue"
            Text {
                id: nameText
                text: name
            }
            Item {
                id: bogusDrag
                //anchors.fill: currentRect
                height: currentRect.height
                width: currentRect.width
                // color: "red"
                property int theId: model.met_id
                property string name: model.name
                property bool dragActive: elementMouse.drag.active
                onDragActiveChanged: {
                    if (dragActive) {
                        print ("drag started: " + x + " " + y);
                        Drag.start();
                        return;
                    }
                    print ("drag ended: " + x + " " + y);
                    Drag.drop();
                    //print ("drag reset: " + x + " " + y);
                }

                states: [
                    // use 2 states so we can always force a transition
                    State {
                        name: "RESET"
                        PropertyChanges { target: bogusDrag; x: 0}
                        PropertyChanges { target: bogusDrag; y: 0}
                    },
                    State {
                        name: "RESET1"
                        PropertyChanges { target: bogusDrag; x: 0}
                        PropertyChanges { target: bogusDrag; y: 0}
                    }]
                // TODO: look at qml states
                //Drag.active: elementMouse.drag.active 
                //Drag.hotSpot.x: 32
                //Drag.hotSpot.y: 32
                //Drag.dragType: Drag.Automatic
            }
            MouseArea {
                id: elementMouse
                anchors.fill: parent
                drag.target: bogusDrag
                onReleased : {
                    console.log("drag release")
                    if (bogusDrag.Drag.target !== null) {
                        console.log("drag target known")
                        //var t = bogusDrag.Drag.target
                        //t.parent.publisher.Enable(met_id)
                    }
                    //bogusDrag.state = "RESET";
                }
            }            
        }
    }
}
