import java.io.*;
import java.util.*;

class Main {
  public static void main(String[] args) {
    if(args.length < 4) {
      System.out.println("Invalid number of arguements.");
    }

    try {
      int numRows = 0, numCols = 0, minVal = 0, maxVal = 0;
      int cycleCount = 0;

      File inputFile = new File(args[0]);
      Scanner inFile = new Scanner(inputFile);

      FileWriter outFile1 = new FileWriter(args[1]);
      FileWriter outFile2 = new FileWriter(args[2]);
      
      outFile1.flush();
      outFile2.flush();

      if(inFile.hasNextInt()) numRows = inFile.nextInt();
      if(inFile.hasNextInt()) numCols = inFile.nextInt();
      if(inFile.hasNextInt()) minVal = inFile.nextInt();
      if(inFile.hasNextInt()) maxVal = inFile.nextInt();
      outFile1.write(numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");

      //Initialize arrays
      int[][] firstAry = new int[numRows+2][numCols+2];
      int[][] secondAry = new int[numRows+2][numCols+2];

      ThinningSkeleton ts = new ThinningSkeleton(numRows, numCols, minVal, maxVal);
      ts.zeroFrame(firstAry);
      ts.zeroFrame(secondAry);
      ts.loadImage(inFile, firstAry, secondAry);

      outFile2.write("Before thinning:\n");
      ts.reformatPrettyPrint(firstAry,outFile2);

      while(ts.changeFlag > 0) {
        ts.changeFlag = 0;
        
        ts.northThinning(firstAry, secondAry);
        ts.copyArys(firstAry, secondAry);

        ts.southThinning(firstAry, secondAry);
        ts.copyArys(firstAry, secondAry);
        
        ts.westThinning(firstAry, secondAry);
        ts.copyArys(firstAry, secondAry);
        
        ts.eastThinning(firstAry, secondAry);
        ts.copyArys(firstAry, secondAry);
        
        cycleCount++;
        outFile2.write("After thinning cycle " + cycleCount + ":\n");
        ts.reformatPrettyPrint(firstAry, outFile2);
      }

      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          outFile1.write(firstAry[i][j] + " ");
        }
        outFile1.write("\n");
      }

      inFile.close();
      outFile1.close();
      outFile2.close();
    }  
    catch(IOException e){
      e.printStackTrace();
    }
  }
}