import QtQuick 2.0
import QtQuick.Controls 1.2
//import QtQuick.Controls.Styles 1.1
import "../../../Controls"

Item {
    property int fontpixelSize : 32
    property int boxwidth: 300
    property int index: 0
    Connections{
        target: channelset
        onPressed:{
            if(setindex == index){

            }
        }
    }

    Text{
        id:setchn
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 100
        text:qsTr("通道选择")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:setchnBox
        width: boxwidth
        anchors.left: setchn.right
        anchors.leftMargin: 100
        anchors.top: setchn.top
        height:setchn.height
        model: ["模拟通道0","模拟通道1","模拟通道2","模拟通道3",
            "模拟通道4","模拟通道5","模拟通道6","模拟通道7"]

    }

    Text{
        id:setchnname
        anchors.top: setchn.bottom
        anchors.topMargin: 20
        anchors.left: setchn.left
//        anchors.leftMargin: 100
        text:qsTr("通道名称")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:chnname
        width: boxwidth
        anchors.left: setchnname.right
        anchors.leftMargin: 100
        anchors.top: setchnname.top
        height:setchnname.height
    }

    Text{
        id:distype
        anchors.top: setchnname.bottom
        anchors.topMargin: 20
        anchors.left: setchnname.left
//        anchors.leftMargin: 100
        text:qsTr("显示类型")
        font.pixelSize: fontpixelSize
    }

    MyCheckBox{
        id:nameCheckbox
        anchors.verticalCenter:distype.verticalCenter
//        anchors.top: distype.top
        anchors.left: distype.right
        anchors.leftMargin: 100
        height: distype.height
        checked: true
        pixSize:fontpixelSize

        str: qsTr("通道名称")
    }

    MyCheckBox{
        id:dateCheckbox
        anchors.verticalCenter: distype.verticalCenter
//        anchors.top: distype.top
        anchors.left: nameCheckbox.right
        anchors.leftMargin: 30
        height: nameCheckbox.height
        checked: true
        pixSize:fontpixelSize

        str: qsTr("日期")
    }

    MyCheckBox{
        id:weekCheckbox
        anchors.verticalCenter: distype.verticalCenter
//        anchors.top: distype.top
        anchors.left: dateCheckbox.right
        anchors.leftMargin: 30
        height: dateCheckbox.height
        checked: true
        pixSize:fontpixelSize

        str: qsTr("星期")
    }

    Text{
        id:datetype
        anchors.top: distype.bottom
        anchors.topMargin: 20
        anchors.left: distype.left
//        anchors.leftMargin: 100
        text:qsTr("日期格式")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:datetypeBox
        width: boxwidth
        anchors.left: datetype.right
        anchors.leftMargin: 100
        anchors.top: datetype.top
        height:datetype.height
        model: ["YYYY/MM/DD","YYYY-MM-DD","YYYY年MM月DD日"]
    }

    Text{
        id:timetype
        anchors.top: datetype.bottom
        anchors.topMargin: 20
        anchors.left: datetype.left
//        anchors.leftMargin: 100
        text:qsTr("时间格式")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:timetypeBox
        width: boxwidth
        anchors.left: timetype.right
        anchors.leftMargin: 100
        anchors.top: timetype.top
        height:timetype.height
        model: ["24小时制","12小时制"]
    }

    Text{
        id:timeposition
        anchors.top: timetype.bottom
        anchors.topMargin: 20
        anchors.left: timetype.left
//        anchors.leftMargin: 100
        text:qsTr("显示位置")
        font.pixelSize: fontpixelSize
    }

    Button{
        id:timepositionBox
        width: boxwidth
        anchors.left: timeposition.right
        anchors.leftMargin: 100
        anchors.top: timeposition.top
        height:timeposition.height
        text:qsTr("位置设置")
        onClicked: regionSetSignal(setchnBox.currentIndex,"display")

    }



}

