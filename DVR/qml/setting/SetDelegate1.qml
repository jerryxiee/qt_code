import QtQuick 2.0

Item {
    id: root
    width: parent.width
    height: 88

    property alias backgroundcolor:back.color
    property alias text: textitem.text
    property alias textcolor:textitem.color
    signal clicked

    Rectangle {
        id:back
        anchors.fill: parent
//        color: mouse.pressed ? "red":"white"
        visible: mouse.pressed
    }

    Text {
        id: textitem
//        color: "white"
        font.pixelSize: 32
        text: modelData
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 30
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 15
        height: 1
        color: "#424246"
    }

//    Image {
//        anchors.right: parent.right
//        anchors.rightMargin: 20
//        anchors.verticalCenter: parent.verticalCenter
//        source: "images/navigation_next_item.png"
//    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: root.clicked()

    }

}
