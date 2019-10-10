import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.VirtualKeyboard 2.2
import PlatformSet 1.0


Item {
    property int fontpixelSize : 32
    property int boxwidth: 300
    property int index: 0
    property bool valueChanged: false

    signal pressPos(int x,int y,int w,int h)

    onPressPos: {
        if(x+keyboardvir.width > parent.width){
            keyboardvir.x = parent.width - keyboardvir.width
        }else{
            keyboardvir.x = x
        }
        if(y + h + keyboardvir.height > parent.height){
            keyboardvir.y = y - keyboardvir.height
        }else{
            keyboardvir.y = y+h
        }

        keyboardvir.visible = true;

    }

    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
        onClicked: {
            keyboardvir.visible = false

        }
    }

    Connections{
        target: bubiaoconfig
        onPressed:{
            if(setindex == index){

                console.log("位置设置")

            }
        }
    }

    Label{
        id:posrepetstyle
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 80
        text: qsTr("位置汇报策略")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:comboposrepetstyle
        anchors.verticalCenter: posrepetstyle.verticalCenter
        anchors.left: posrepetstyle.right
        anchors.leftMargin: 10
        width: 300
        model: ["定时汇报","定距汇报","定距/定距"]
    }

    Label{
        id:posrepettype
        anchors.top: posrepetstyle.bottom
        anchors.topMargin: 10
        anchors.left: posrepetstyle.left

        text: qsTr("位置汇报方案")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:comboposrepettype
        anchors.verticalCenter: posrepettype.verticalCenter
        anchors.left: posrepettype.right
        anchors.leftMargin: 10
        width: 300
        model: ["根据ACC状态","根据登录状态和ACC状态"]
    }

    Label{
        id:report1
        anchors.top: posrepettype.bottom
        anchors.topMargin: 10
        anchors.left: posrepettype.left
        text: qsTr("紧急报警时汇报间隔")
        width: 400
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:report1text
        width: 100
        anchors.verticalCenter: report1.verticalCenter
        anchors.left: report1.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:report1textsec
        anchors.verticalCenter: report1.verticalCenter
        anchors.left: report1text.right
        anchors.leftMargin: 5
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }
    TextField{
        id:report1text1
        width: 100
        anchors.verticalCenter: report1.verticalCenter
        anchors.left: report1textsec.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:flag
        anchors.verticalCenter: report1.verticalCenter
        anchors.left: report1text1.right
        anchors.leftMargin: 5
        text: qsTr("米")
        font.pixelSize: fontpixelSize
    }


    Label{
        id:report2
        anchors.top: report1.bottom
        anchors.topMargin: 10
        anchors.left: report1.left
        text: qsTr("司机未登录汇报间隔")
        width: 400
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:report2text
        width: 100
        anchors.verticalCenter: report2.verticalCenter
        anchors.left: report2.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:report2textsec
        anchors.verticalCenter: report2.verticalCenter
        anchors.left: report2text.right
        anchors.leftMargin: 5
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }
    TextField{
        id:report2text1
        width: 100
        anchors.verticalCenter: report2.verticalCenter
        anchors.left: report2textsec.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:flag1
        anchors.verticalCenter: report2.verticalCenter
        anchors.left: report2text1.right
        anchors.leftMargin: 5
        text: qsTr("米")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:report3
        anchors.top: report2.bottom
        anchors.topMargin: 10
        anchors.left: report2.left
        text: qsTr("休眠时汇报间隔")
        width: 400
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:report3text
        width: 100
        anchors.verticalCenter: report3.verticalCenter
        anchors.left: report3.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:report3textsec
        anchors.verticalCenter: report3.verticalCenter
        anchors.left: report3text.right
        anchors.leftMargin: 5
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }
    TextField{
        id:report3text1
        width: 100
        anchors.verticalCenter: report3.verticalCenter
        anchors.left: report3textsec.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:flag2
        anchors.verticalCenter: report3.verticalCenter
        anchors.left: report3text1.right
        anchors.leftMargin: 5
        text: qsTr("米")
        font.pixelSize: fontpixelSize
    }


    Label{
        id:report4
        anchors.top: report3.bottom
        anchors.topMargin: 10
        anchors.left: report3.left
        text: qsTr("缺省时间汇报间隔")
        width: 400
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:report4text
        width: 100
        anchors.verticalCenter: report4.verticalCenter
        anchors.left: report4.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:report4textsec
        anchors.verticalCenter: report4.verticalCenter
        anchors.left: report4text.right
        anchors.leftMargin: 5
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }
    TextField{
        id:report4text1
        width: 100
        anchors.verticalCenter: report4.verticalCenter
        anchors.left: report4textsec.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:flag3
        anchors.verticalCenter: report4.verticalCenter
        anchors.left: report4text1.right
        anchors.leftMargin: 5
        text: qsTr("米")
        font.pixelSize: fontpixelSize
    }

    InputPanel {
        id: keyboardvir
        visible: false
//            anchors.right: parent.right
//            anchors.left: parent.left
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.bottom: parent.bottom
        width: parent.width/1.5
//        height: parent.height/3
        onActiveChanged: {
            if(!active) { visible = false; }
        }
    }
}
