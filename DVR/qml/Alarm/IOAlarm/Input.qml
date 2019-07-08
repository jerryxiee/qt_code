import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls 2.2 as Control2
import "../../Controls"

Item {
    property int fontpixelSize : 32
    property int boxwidth: 300
    property int index: 0
    Connections{
        target: ioalarmroot
        onPressed:{
            if(value == index){
                console.log("input")
                AlarmConfig.setIoAlarm(ioportbox.currentIndex,textfield_alarmname.text,electypebox.currentIndex,
                                       recordchnbox.currentIndex,dealtype1.checked,dealtype2.checked,dealtype3.checked)

            }
        }
    }

    Text {
        id: ioport
        width: boxwidth-100
        text: qsTr("IO端口")
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 100
        font.pixelSize:fontpixelSize
    }

    ComboBox{
        id:ioportbox
        width: boxwidth
        height: ioport.height
        anchors.left: ioport.right
        anchors.leftMargin: 50
        anchors.verticalCenter: ioport.verticalCenter
        model: 10
    }

    Text{
        id:alarmname
        width: boxwidth-100
        anchors.top: ioport.bottom
        anchors.topMargin: 20
        anchors.left: ioport.left
//        anchors.leftMargin: 100
        text:qsTr("报警名称")
        font.pixelSize: fontpixelSize
    }

    Control2.TextField{
        id:textfield_alarmname
        width: boxwidth
        anchors.left: alarmname.right
        anchors.leftMargin: 50
        anchors.top: alarmname.top
        height:alarmname.height
        text: AlarmConfig.getAlarmName(ioportbox.currentIndex)
    }

    Text {
        id: electype
        width: boxwidth-100
        text: qsTr("触发电平")
        anchors.top: alarmname.bottom
        anchors.topMargin: 20
        anchors.left: alarmname.left

        font.pixelSize:fontpixelSize
    }

    ComboBox{
        id:electypebox
        width: boxwidth
        height: electype.height
        anchors.left: electype.right
        anchors.leftMargin: 50
        anchors.verticalCenter: electype.verticalCenter
        model: ["低电平","高电平"]
        currentIndex: AlarmConfig.getTrigLevel(ioportbox.currentIndex)
    }

    Text {
        id: recordchn
        width: boxwidth-100
        text: qsTr("触发录像通道")
        anchors.top: electype.bottom
        anchors.topMargin: 20
        anchors.left: electype.left

        font.pixelSize:fontpixelSize
    }

    ComboBox{
        id:recordchnbox
        width: boxwidth
        height: recordchn.height
        anchors.left: recordchn.right
        anchors.leftMargin: 50
        anchors.verticalCenter: recordchn.verticalCenter
        model: ["无","模拟通道0","模拟通道1","模拟通道2","模拟通道3",
            "模拟通道4","模拟通道5","模拟通道6","模拟通道7"]
        currentIndex: AlarmConfig.getConnectVideoChn(ioportbox.currentIndex)
    }

    Text {
        id: dealtype
        width: boxwidth-100
        text: qsTr("处理方式")
        anchors.top: recordchn.bottom
        anchors.topMargin: 20
        anchors.left: recordchn.left

        font.pixelSize:fontpixelSize
    }

    MyCheckBox{
        id:dealtype1
        anchors.left: dealtype.right
        anchors.leftMargin: 50
        anchors.verticalCenter: dealtype.verticalCenter
        checked: AlarmConfig.getIoDispEnable(ioportbox.currentIndex)
        pixSize:fontpixelSize

        str:qsTr("弹出报警画面")

    }
    MyCheckBox{
        id:dealtype2
        anchors.left: dealtype1.right
        anchors.leftMargin: 20
        anchors.verticalCenter: dealtype.verticalCenter
        checked: AlarmConfig.getIoSoundEnable(ioportbox.currentIndex)
        pixSize:fontpixelSize
        str:qsTr("声音报警")
    }

    MyCheckBox{
        id:dealtype3
        anchors.left: dealtype2.right
        anchors.leftMargin: 20
        anchors.verticalCenter: dealtype.verticalCenter
        checked: AlarmConfig.getIoReportEnable(ioportbox.currentIndex)
        pixSize:fontpixelSize
        str:qsTr("报警输出")
    }



}
