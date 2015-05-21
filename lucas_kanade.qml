import QtQuick 2.4
import QtQuick.Controls 1.3

Item {
    width: 400
    height: 240
    ComboBox {
        id: comboBox1
        x: 150
        y: 0
        model: [ "B-spline", "Bilinear", "Bicubic" ]
    }   
    Label {
        id: countInterpolation
        x: 20
        y: 5
        text: qsTr("Interpolation")
    } 
    SpinBox {
        id: countIteration
        x: 150
        y: 40
        minimumValue : 1
        maximumValue : 100
        value : 10
    }
    SpinBox {
        id: windowSearch
        x: 150
        y: 80
        minimumValue : 2
        maximumValue : 1000
        value : 16
    }
    SpinBox {
        id: stepGrid
        x: 150
        y: 120
        minimumValue : 1
        maximumValue : 500
        value : 16
    }

    Label {
        id: countIterationLabel
        x: 20
        y: 40
        text: qsTr("Count iteration")
    }

    Label {
        id: labelWindowSearch
        x: 20
        y: 80
        text: qsTr("Size window search")
    }

    Label {
        id: labelStepGrid
        x: 20
        y: 120
        text: qsTr("Step for grid")
    }

    TextField {
        id: outDirField
        objectName: "outDirField"
        x: 20
        y: 160
        placeholderText: qsTr("Output directory")
    }

    Button {
        id: buttonOutDirField
        x: 150
        y: 160
        text: qsTr("Browse")
    }

    CheckBox {
        id: debugCheckBox
        x: 20
        y: 200
        text: qsTr("More log")
        checked: false
    }

    CheckBox {
        id: pyramidCheckBox
        x: 150
        y: 200
        text: qsTr("Use pyramid")
        checked: true
    }

    function getcmd()
    {
      return "lucas_kanade";
    }

    function getarg()
    {
      var a = 
      [
        "-l", outDirField.text + "/list",//load list image
        "-o", outDirField.text,//output directory
        "-i", countIteration.value,//count iteration (1 by default)
        "-w", windowSearch.value,//size window search (3px by default)
        "-g", stepGrid.value,//step for grid (5px by default)
        "-b", comboBox1.currentIndex,
        pyramidCheckBox.checked ? "-p" : "",
        debugCheckBox.checked ? "-d" : "",
        /*"-h", //show help
        "-v" //show version*/
      ];

      return a;
    }

    function outdir() 
    {
      return outDirField.text;
    }

}
