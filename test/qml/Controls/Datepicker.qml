import QtQuick 2.0
import QtQuick.Controls 1.2

TextField
{
    property string dateValue
    property string str
    width: 200

    Calendar{
        id: calendar
        anchors.topMargin: 0
        anchors.top: parent.bottom
        visible: false
        activeFocusOnTab: true
        onReleased: {
            text = date;
            text = text.substr(0, 10);
            dateValue = text;
            visible = false;
        }


//        MouseArea {
//            anchors.fill: parent
//            hoverEnabled:true

//            onClicked: {
//                str = calendar.date;
//                str = str.substr(0, 10);
//                dateValue = str;
//                visible = false;
//                console.log("click")
//            }
//            onExited: {
//                calendar.visible = false;
//                console.log("exit")
//            }
//        }
    }

    Button{
        id: downBtn
        width: 22
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        iconSource: "images/arrow_bc.png"
        onClicked: calendar.visible = !calendar.visible
    }

    onDateValueChanged: {
        text = dateValue;
        calendar.selectedDate = dateValue;
    }

}
