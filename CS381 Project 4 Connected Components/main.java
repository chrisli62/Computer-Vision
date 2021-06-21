import java.io.*;
import java.util.*;

class Main {
  public static void main(String[] args) {
    if(args.length < 4) {
      System.out.println("Invalid number of arguements.");
    }

    try {
      int numRow = 0, numCol = 0, minVal = 0, maxVal = 0;
      /*File inputFile = new File(args[0]);
      Scanner inFile = new Scanner(inputFile);

      FileWriter RFprettyPrintFile = new FileWriter(args[1]);
      FileWriter labelFile = new FileWriter(args[2]);
      FileWriter propertyFile = new FileWriter(args[3]);*/
      File inputFile = new File("data1.txt");
      Scanner inFile = new Scanner(inputFile);

      FileWriter RFprettyPrintFile = new FileWriter("pp.txt");
      FileWriter labelFile = new FileWriter("label.txt");
      FileWriter propertyFile = new FileWriter("prop.txt");
      RFprettyPrintFile.flush();
      labelFile.flush();
      propertyFile.flush();

      if(inFile.hasNextInt()) numRow = inFile.nextInt();
      if(inFile.hasNextInt()) numCol = inFile.nextInt();
      if(inFile.hasNextInt()) minVal = inFile.nextInt();
      if(inFile.hasNextInt()) maxVal = inFile.nextInt();

      int[][] zeroFramedAry = new int[numRow+2][numCol+2];
      int EQArySize = (numRow*numCol)/4;
      int[] EQAry = new int[EQArySize];
      int newLabel = 0;
      int newMin = 0;
      int newMax = 0;

      ConnectedCC cc = new ConnectedCC(numRow, numCol, minVal, maxVal, newLabel);
      cc.zero2D(zeroFramedAry);
      cc.loadImage(inFile, zeroFramedAry);

      Scanner user = new Scanner(System.in);
      System.out.println("Input which connectedness algorithm should be used. 4 or 8?");
      String value = user.nextLine();
      int connectedness = Integer.parseInt(value);
      user.close();
      
      if(connectedness == 4) {
        cc.connect4Pass1(zeroFramedAry);
        RFprettyPrintFile.write("4-Connectedness Pass 1\n");
        cc.imgReformat(zeroFramedAry, RFprettyPrintFile);
        RFprettyPrintFile.write("4-Connectedness Pass 1 EQ Array\n");
        cc.printEQAry(RFprettyPrintFile);
        cc.connect4Pass2(zeroFramedAry);
        RFprettyPrintFile.write("4-Connectedness Pass 2\n");
        cc.imgReformat(zeroFramedAry, RFprettyPrintFile);
        RFprettyPrintFile.write("4-Connectedness Pass 2 EQ Array\n");
        cc.printEQAry(RFprettyPrintFile);
      }

      if(connectedness == 8) {
        cc.connect8Pass1(zeroFramedAry);
        RFprettyPrintFile.write("8-Connectedness Pass 1\n");
        cc.imgReformat(zeroFramedAry, RFprettyPrintFile);
        RFprettyPrintFile.write("8-Connectedness Pass 1 EQ Array\n");
        cc.printEQAry(RFprettyPrintFile);
        cc.connect8Pass2(zeroFramedAry);
        RFprettyPrintFile.write("8-Connectedness Pass 2\n");
        cc.imgReformat(zeroFramedAry, RFprettyPrintFile);
        RFprettyPrintFile.write("8-Connectedness Pass 2 EQ Array\n");
        cc.printEQAry(RFprettyPrintFile);
      }

      int trueNumCC = cc.manageEQAry(EQAry);
      RFprettyPrintFile.write("EQ Array after management\n");
      cc.printEQAry(RFprettyPrintFile);

      Node[] CCproperty = new Node[trueNumCC];
      cc.connectPass3();
      RFprettyPrintFile.write("Connectedness Pass 3\n");
      cc.imgReformat(zeroFramedAry, RFprettyPrintFile);
      RFprettyPrintFile.write("Connectedness Pass 3 EQ Array\n");
      cc.printEQAry(RFprettyPrintFile);

      cc.printImg(labelFile);

      cc.printCCproperty(propertyFile);
      cc.drawBoxes(zeroFramedAry, CCproperty);
      RFprettyPrintFile.write("Boxes\n");
      cc.imgReformat(zeroFramedAry, RFprettyPrintFile);

      RFprettyPrintFile.write("Value of trueNumCC: " + trueNumCC);

      // Close files
      inFile.close();
      RFprettyPrintFile.close();
      propertyFile.close();
      labelFile.close();
    }
    catch(IOException e){
      e.printStackTrace();
    }
  }
}