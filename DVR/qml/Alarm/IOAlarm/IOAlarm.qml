import QtQuick 2.0
import QtQuick.Controls 1.2

Item {
    id:ioalarmroot

    signal pressed(int value)


    TabView{
        id: tabView
        focus: true

        anchors.fill: parent

        Tab{
            title: "Input"
            Input{
                index:0

            }
        }

        Tab{
            title: "Output"
            Output{
                index:1
            }
        }
    }


    Connections{
        target: alarmsetroot
        onSuresignal:{
            pressed(tabView.currentIndex)
            console.log("ioalarm set")
        }
        onCancelsignal:{

            console.log("cancel press")
        }
    }


}
