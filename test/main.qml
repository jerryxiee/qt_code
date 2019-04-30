import QtQuick 2.0
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3


Rectangle {
    visible: true
//    width: 800
//    height: 600
    color: "blue"
//    opacity: 0.2

    MouseArea{
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
            onClicked: {
                if(mouse.button === Qt.LeftButton){
                    console.log("left")
                }else if(mouse.button === Qt.RightButton){
                    console.log("right")
                }else{

                }

            }

        }

    Action{
        id:quitAction
        text:qsTr("Quit")
        shortcut: "ctrl+q"
        iconSource: "image/1.png"
        onTriggered: Qt.quit()
    }

    Action{
        id:cutAction
        text:qsTr("cut")
        shortcut: "ctrl+x"
        iconSource: "image/1.png"
        onTriggered: mycontent.cut()
    }
    Action{
        id:copyAction
        text:qsTr("copy")
        shortcut: "ctrl+c"
        iconSource: "image/1.png"
        onTriggered: mycontent.copy()
    }
    Action{
        id:pasteAction
        text:qsTr("paste")
        shortcut: "ctrl+v"
        iconSource: "image/1.png"
        onTriggered: mycontent.paste()
    }
    MenuBar{
        Menu{
            title:qsTr("File")
            MenuItem{
                text: qsTr("退出")
                onTriggered: Qt.quit()
                action: quitAction
            }
        }
        Menu{
            title: qsTr("Edit")
            MenuItem{
                action: copyAction
            }
            MenuItem{
                action: cutAction
            }
            MenuItem{
                action: pasteAction
            }
        }
    }

     ToolBar{
        id:mytoolbar
        width: parent.width
        Row{
            anchors.fill: parent
            ToolButton{action: copyAction}
            ToolButton{action: cutAction}
            ToolButton{action: pasteAction}
        }


    }


}
