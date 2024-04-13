import processing.serial.*;
import static javax.swing.JOptionPane.*;

Serial myPort;        // The serial port
String serialin;
int data[] = new int[360];
PFont f;

final boolean debug = true;

void setup() {
  String COMx, COMlist = "";
  size(1280, 720);
  f = createFont("Verdana", 32, true); // Arial, 16 point, anti-aliasing on
  textFont(f, 20);
  frameRate(60);
  for (int i = 0; i < 360; i++) {
    data[i] = 0;
  }
  try {
    if (debug) printArray(Serial.list());
    int i = Serial.list().length;
    if (i != 0) {
      if (i >= 2) {
        // need to check which port the inst uses -
        // for now we'll just let the user decide
        for (int j = 0; j < i; ) {
          COMlist += char(j+'a') + " = " + Serial.list()[j];
          if (++j < i) COMlist += ",  ";
        }
        COMx = showInputDialog("Which COM port is correct? (a,b,..):\n"+COMlist);
        if (COMx == null) exit();
        if (COMx.isEmpty()) exit();
        i = int(COMx.toLowerCase().charAt(0) - 'a') + 1;
      }
      String portName = Serial.list()[i-1];
      if (debug) println(portName);
      myPort = new Serial(this, portName, 9600); // change baud rate to your liking
      myPort.bufferUntil('\n'); // buffer until CR/LF appears, but not required..
    } else {
      showMessageDialog(frame, "Device is not connected to the PC");
      exit();
    }
  }
  catch (Exception e)
  { //Print the type of error
    showMessageDialog(frame, "COM port is not available (may\nbe in use by another program)");
    println("Error:", e);
    exit();
  }
}


void draw() {
  background(26, 26, 36, 200);
  textSize(18);
  stroke(255, 255, 255, 150);
  fill(255, 50, 200, 200);
  text("Arduino RADAR 2D Visualization", 20, 710);
  text(hour(), 1050, 710);
  text(":", 1075, 710);
  text(minute(), 1085, 710);
  text(":", 1110, 710);
  text(second(), 1120, 710);
  fill(36, 255, 100, 200);
  strokeWeight(3);
  circle(640, 360, 600);
  circle(640, 360, 500);
  circle(640, 360, 400);
  circle(640, 360, 300);
  circle(640, 360, 200);
  circle(640, 360, 100);

  for (int i = 0; i < 360; i++) {
    fill(255, 10, 255, 200);
    stroke(50, 10, 255, 150);
    point(float(640) +  (map_values(data[i]))*cos(radians(i)), float(360) + (map_values(data[i]))*sin(radians(i)));
  }

  while (myPort.available() > 0) {
    serialin = myPort.readStringUntil(10);
    try {
      String serialdata[] = splitTokens(serialin, ",");
      if (serialdata[0] != null) {
        serialdata[0] = trim(serialdata[0]);
        serialdata[1] = trim(serialdata[1]);
        serialdata[2] = trim(serialdata[2]);

        int i = int(serialdata[0]);
        data[179-i] = int(serialdata[1]);
        data[(179-i)+180] = int(serialdata[2]);
      }
    } 
    catch (java.lang.RuntimeException e) {
    }
  }
}

float map_values(float x) {
  float in_min = 0, in_max = 200, out_min = 0, out_max = 700;
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}