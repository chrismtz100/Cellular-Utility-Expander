
import 'package:flutter/material.dart';
import 'package:fab_circular_menu/fab_circular_menu.dart';
import 'dart:async';
import 'bt-controller.dart';

void main() => runApp(MyApp()); // Run Application

// This widget is the root of your application.
// Where the app Begins 
class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'AndroidAPP',
      theme: ThemeData( primarySwatch: Colors.blue, ),
      home: MyHomePage( title: 'Demo' ),
    );
  }
}

// Constructer for the Page
class MyHomePage extends StatefulWidget {
  MyHomePage({Key key, this.title}) : super(key: key); 
  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

// Page and Information of the Page
// This is also the function of the page
class _MyHomePageState extends State<MyHomePage> {
  // INITIATE VALUES
  final GlobalKey<FabCircularMenuState> fabKey = GlobalKey();
  String sensorValue = "N/A";
  bool ledState = false;

  // Initiate the Bluetooth Controler
  @override
  initState() {
    super.initState();
    BTController.init(onData);
    scanDevices();
  }

  // When information is recived, change data state
  void onData(dynamic str) { 
    setState(() { sensorValue = str; }); 
  }

  // Take the list of devices and itterate them
  Future<void> scanDevices() async { 
    BTController.enumerateDevices()
        .then((devices) { onGetDevices(devices); });
  }

  // Itterate the selection into a selectable list
  void onGetDevices(List<dynamic> devices) {
    Iterable<SimpleDialogOption> options = devices.map((device) {
        return SimpleDialogOption(
            child: Text(device.keys.first),
            onPressed: () { selectDevice(device.values.first); },
        );
    });

    SimpleDialog dialog = SimpleDialog(
        title: const Text('Choose a device'),
        children: options.toList(),
    );

    showDialog(
        barrierDismissible: false,
        context: context,
        builder: (BuildContext context) { return dialog; }
    );
  }

  // When device is selected establish the connection.
  selectDevice(String deviceAddress) {
    Navigator.of(context, rootNavigator: true).pop('dialog');
    BTController.connect(deviceAddress);
  }

  // Swap the LED to its opposite State
  void switchLed() { 
    setState(() { ledState = !ledState; });
    BTController.transmit(ledState ? '0' : '1');
  }
 
    @override
    Widget build(BuildContext context) {
      final primaryColor = Theme.of(context).primaryColor; 
      TextTheme theme = Theme.of(context).textTheme;

      return Scaffold(
        appBar: AppBar(title: Text(widget.title)),
        body: Container(
          decoration: BoxDecoration(color: Colors.lightBlue),
          child: Center(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[
                Text('Sensor Value', style: theme.display1.copyWith(color: Colors.white)),
                Text(sensorValue, style: theme.display2.copyWith(color: Colors.white)),
              ],
            )
          ),
        ), 
        floatingActionButton: Builder(
          builder: (context) => FabCircularMenu(
            key: fabKey, 
            alignment: Alignment.bottomRight,
            ringColor: Colors.white.withAlpha(25),
            ringDiameter: 500.0,
            // ringWidth: 150.0,
            fabSize: 64.0,
            fabElevation: 8.0,
            
            // Also can use specific color based on wether
            // the menu is open or not:
            // fabOpenColor: Colors.white
            // fabCloseColor: Colors.white
            // These properties take precedence over fabColor
            fabColor: Colors.white,
            fabOpenIcon: Icon(Icons.menu, color: primaryColor),
            fabCloseIcon: Icon(Icons.close, color: primaryColor),
            fabMargin: const EdgeInsets.all(16.0),
            animationDuration: const Duration(milliseconds: 800),
            animationCurve: Curves.ease,
            // onDisplayChange: (isOpen) { _showSnackBar(context, "The menu is ${isOpen ? "open" : "closed"}"); },
            children: <Widget>[
              RawMaterialButton( 
                onPressed: () async { fabKey.currentState.close(); },
                shape: CircleBorder(),
                padding: const EdgeInsets.all(24.0), 
                child: FloatingActionButton(
                  backgroundColor: Colors.white,
                  onPressed: () async { fabKey.currentState.close(); },  
                  tooltip: 'Save Data',  
                  child:  Icon(Icons.save, color: primaryColor),
                ), 
              ),
              RawMaterialButton( 
                onPressed: () async { fabKey.currentState.close(); },
                shape: CircleBorder(),
                padding: const EdgeInsets.all(24.0), 
                child: FloatingActionButton( 
                  backgroundColor: Colors.white,
                  onPressed: () async { fabKey.currentState.close(); },
                  tooltip: 'View Saved',
                  child: Icon(Icons.table_chart , color: primaryColor),
                ), 
              ),
              RawMaterialButton(
                onPressed: () async { fabKey.currentState.close(); },
                shape: CircleBorder(),
                padding: const EdgeInsets.all(24.0), 
                child: FloatingActionButton(
                  backgroundColor: Colors.white,
                  onPressed: () async { 
                    switchLed; 
                    fabKey.currentState.close(); 
                  },
                  tooltip: 'Reconnect',
                  child: Icon(Icons.bluetooth, color: primaryColor),
                ), 
              ),
              RawMaterialButton(
                onPressed: () async { fabKey.currentState.close(); },
                shape: CircleBorder(),
                padding: const EdgeInsets.all(24.0),
                child: FloatingActionButton(
                  backgroundColor: Colors.white,
                  onPressed: () async { fabKey.currentState.close(); },
                  tooltip: 'Settings',
                  child: Icon(Icons.settings, color: primaryColor),
                ),  
              )
            ],
          ),
        ),
      );
  }
  
  void _showSnackBar (BuildContext context, String message) {
    Scaffold.of(context).showSnackBar(
        SnackBar(
          content: Text(message),
          duration: const Duration(milliseconds: 1000),
        )
    );
  }
} 
 