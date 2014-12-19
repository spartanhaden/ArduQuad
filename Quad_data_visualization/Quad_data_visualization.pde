import processing.serial.*;

Serial myPort;

final String serialPort = "/dev/cu.usbmodem1411";

final int VIEW_SIZE_X = 800;
final int VIEW_SIZE_Y = 800;

String [] accel = new String[3];

PFont font;
float rotate = .5;

void setup() {
    size(VIEW_SIZE_X, VIEW_SIZE_Y);
    myPort = new Serial(this, serialPort, 9600);
    font = loadFont("SansSerif-24.vlw");
    delay(100);
    myPort.clear();
    myPort.write("1");
    accel[0] = "hi";
    accel[1] = "hi";
    accel[2] = "hi";
    myPort.bufferUntil(13);
}

void draw() {
    background(#ffffff);

    textFont(font);
    textAlign(LEFT, TOP);
    text("X: " + accel[0] + "\nY: " + accel[1] + "\nZ: " + accel[2], 20, 20);

    //translate(400, 400);
    //rotateY(rotate);
    //rotate+=.01f;
    //noFill();
    //box(400);
}

void serialEvent(Serial p) { 
  String inString = p.readString();
  accel = split(inString, ",");
} 
