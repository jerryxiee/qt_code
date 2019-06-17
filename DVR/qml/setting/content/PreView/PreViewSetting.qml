import QtQuick 2.0
import QtQuick.Controls 1.2
import "../../../Controls"

Item {
    id:preview
    property int fontpixelSize : 32
    property int boxwidth: 150
    signal pressed(int setindex)


    TabView{
        id: tabView
        focus: true
        anchors.fill: parent

        Tab{
            title: "Normal"
            NormalSet{
                index: 0


            }
        }
        Tab{
            title: "Other"
            Other{
                index: 1


            }
        }
    }

    Connections{
        target: setparent
        onSuresignal:{
            pressed(tabView.currentIndex)
            console.log("sure press")
        }
        onCancelsignal:{

            console.log("cancel press")
        }
    }



}
