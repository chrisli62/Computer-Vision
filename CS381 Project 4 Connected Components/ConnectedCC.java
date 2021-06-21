import java.io.*;
import java.util.*;

public class ConnectedCC {
  private int numRow;
  private int numCol;
  private int minVal;
  private int maxVal;
  private int newMin;
  private int newMax;
  private int newLabel;
  private int trueNumCC;
  private int[][] zeroFrameAry;
  public int[] NonZeroNeighborAry = new int[5];
  private int[] EQAry;
  private int EQArySize;
  private Node[] CCproperty;

  public ConnectedCC(int numR, int numC, int minV, int maxV, int newLabel){
    // Obtain numRow, numCol, minVal, maxVal
    numRow = numR;
    numCol = numC;
    minVal = minV;
    maxVal = maxV;

    // Initialize arrays
    zeroFrameAry = new int[numRow+2][numCol+2];
    EQArySize = (numRow*numCol)/4;
    EQAry = new int[EQArySize];
    for(int i = 0; i < EQArySize; i++) {
      EQAry[i] = i;
    }
    newMin = 0;
    newMax = 0;
    newLabel = 0;
  }

  // Initialize zeroFrameAry to 0
  public void zero2D(int[][] zeroFrameAry) {
    for(int i = 0; i < numRow+2; i++) {
      for(int j = 0; j < numCol+2; j++) {
        zeroFrameAry[i][j] = 0;
      }
    }
  }

  // Initialize 1-D array to -1
  public void minus1D(int[] array) {
    for(int i = 0; i < array.length; i++) {
      array[i] = -1;
    }
  }

  public void loadImage(Scanner inFile, int[][] zeroFrameAry){
      for(int i = 1; i <= numRow; i++) {
        for(int j = 1; j <= numCol; j++) {
          if(inFile.hasNextInt()) zeroFrameAry[i][j] = inFile.nextInt();
        }
      }
  }

  public void imgReformat(int[][] zeroFrameAry, FileWriter reformFile) {
    try {
      //reformFile.write(numRow + " " + numCol + " " + newMin + " " + newMax + "\n");
      String str = Integer.toString(newMax);
      int width = str.length();

      int r = 1;

      while(r <= numRow){
        int c = 1;
        while(c <= numCol){
          if(zeroFrameAry[r][c] != 0) {
            reformFile.write(String.valueOf(zeroFrameAry[r][c]) + " ");
          }
          else {
            reformFile.write(". ");
          }
          str = Integer.toString(zeroFrameAry[r][c]);
          int ww = str.length();
          while(ww < width){
            reformFile.append(" ");
            System.out.print(" ");
            ww++;
          }
          c++;  
        }
        r++;
        reformFile.append("\n");
      }
    }
    catch(IOException e) {
      e.printStackTrace();
    }
  }

  public int min(int[] NonZeroNeighborAry) {
    int temp = Integer.MAX_VALUE;
    for(int i = 0; i < NonZeroNeighborAry.length; i++){
      if(NonZeroNeighborAry[i] != 0){
        if(temp > NonZeroNeighborAry[i]){
          temp = NonZeroNeighborAry[i];
        }
      }
    }
    return temp;
  }

  // Switch for 8-Connectedness Pass 1
  public int eightConPass1(int[] NonZeroNeighborAry) {   
    if(NonZeroNeighborAry[0] == 0 && NonZeroNeighborAry[1] == 0 && NonZeroNeighborAry[2] == 0 && NonZeroNeighborAry[3] == 0) {
      return 1;
    }
    else if(NonZeroNeighborAry[0] == NonZeroNeighborAry[1] && NonZeroNeighborAry[1] == NonZeroNeighborAry[2] && NonZeroNeighborAry[2] == NonZeroNeighborAry[3]){
      return 2;
    }
    else {
      return 3;
    }
  }

  // Switch for 8-Connectedness Pass 2
  public int eightConPass2(int[] NonZeroNeighborAry) {
    if(NonZeroNeighborAry[0] == 0 && NonZeroNeighborAry[1] == 0 && NonZeroNeighborAry[2] == 0 && NonZeroNeighborAry[3] == 0) {
      return 1;
    }
    else if(NonZeroNeighborAry[0] == NonZeroNeighborAry[1] && NonZeroNeighborAry[1] == NonZeroNeighborAry[2] && NonZeroNeighborAry[2] == NonZeroNeighborAry[3] && NonZeroNeighborAry[3] == NonZeroNeighborAry[4]) {
      return 2;
    }
    else {
      return 3;
    }
  }

  // Switch for 4-Connectedness Pass 1
  public int fourConPass1(int[] NonZeroNeighborAry) {
    if(NonZeroNeighborAry[0] == 0 && NonZeroNeighborAry[1] == 0) {
      return 1;
    }
    else if(NonZeroNeighborAry[0] == NonZeroNeighborAry[1]) {
      return 2;
    }
    else {
      return 3;
    }
  }

  // Switch for 4-Connectedness Pass 2
  public int fourConPass2(int[] NonZeroNeighborAry) {
    if(NonZeroNeighborAry[0] == 0 && NonZeroNeighborAry[1] == 0) {
      return 1;
    }
    else if(NonZeroNeighborAry[0] == NonZeroNeighborAry[1] && NonZeroNeighborAry[1] == NonZeroNeighborAry[2]) {
      return 2;
    }
    else {
      return 3;
    }
  }

  // 8-Connectedness Pass 1
  public void connect8Pass1(int[][] zeroFrameAry) {
    System.out.println("Processing 8-Connectedness Pass 1...");
    newMin = minVal;
    newLabel = minVal;

    for(int i = 1; i < numRow+1; i++) {
      for(int j = 1; j < numCol+1; j++){
        if(zeroFrameAry[i][j] > 0) {
          // a, b, c, d, P(i,j)
          NonZeroNeighborAry[0] = zeroFrameAry[i-1][j-1];
          NonZeroNeighborAry[1] = zeroFrameAry[i-1][j];
          NonZeroNeighborAry[2] = zeroFrameAry[i-1][j+1];
          NonZeroNeighborAry[3] = zeroFrameAry[i][j-1];
          //NonZeroNeighborAry[4] = zeroFrameAry[i][j];
 
          switch(eightConPass1(NonZeroNeighborAry)) {
            // a = b = c = d = 0
            case 1:
              newLabel++;
              zeroFrameAry[i][j] = newLabel;
              break;
            // some/all of a,b,c,d already have the same label
            case 2:
              zeroFrameAry[i][j] = NonZeroNeighborAry[0];
              break;
            // some/all of a,b,c,d already have labels but they are not the same
            case 3:
              zeroFrameAry[i][j] = min(NonZeroNeighborAry);
              break;
          }
        }
      }
    }
    newMax = newLabel;
    System.out.println("8-Connectedness Pass 1 Complete!");
  }

  // 8-Connectedness Pass 2
  public void connect8Pass2(int[][] zeroFrameAry) {
    System.out.println("Processing 8-Connectedness Pass 2...");
    newMin = minVal;

    for(int i = numRow; i > 0; i--) {
      for(int j = numCol; j > 0; j--){
        if(zeroFrameAry[i][j] > 0) {
          // e, f, g, h, P(i,j)
          NonZeroNeighborAry[0] = zeroFrameAry[i][j+1];
          NonZeroNeighborAry[1] = zeroFrameAry[i+1][j-1];
          NonZeroNeighborAry[2] = zeroFrameAry[i+1][j];
          NonZeroNeighborAry[3] = zeroFrameAry[i+1][j+1];
          //NonZeroNeighborAry[4] = zeroFrameAry[i][j];

          switch(eightConPass2(NonZeroNeighborAry)) {
            // a = b = c = d = 0
            case 1:
              // Does nothing. Keeps label
              break;
            // some/all of a,b,c,d already have the same label
            case 2:
              // Does nothing. Keeps label
              break;
            // some/all of a,b,c,d already have labels but they are not the same
            case 3:
              int minLabel = min(NonZeroNeighborAry);
              if(zeroFrameAry[i][j] > minLabel) {
                EQAry[zeroFrameAry[i][j]] = minLabel;
                zeroFrameAry[i][j] = minLabel;
              }
              break;
          }
        }
      }
    }
    System.out.println("8-Connectedness Pass 2 Complete!");
  }

  // 4-Connectedness Pass 1
  public void connect4Pass1(int[][] zeroFrameAry) {
    System.out.println("Processing 4-Connectedness Pass 1...");
    newMin = minVal;
    newLabel = minVal;

    for(int i = 1; i < numRow+1; i++) {
      for(int j = 1; j < numCol+1; j++){
        if(zeroFrameAry[i][j] > 0) {
          // a, b, P(i,j)
          NonZeroNeighborAry[0] = zeroFrameAry[i-1][j];
          NonZeroNeighborAry[1] = zeroFrameAry[i][j-1];
          //NonZeroNeighborAry[2] = zeroFrameAry[i][j];

          switch(fourConPass1(NonZeroNeighborAry)) {
            // a = b = 0
            case 1:
              newLabel++;
              zeroFrameAry[i][j] = newLabel;
              break;
            // some/all of a,b already have the same label
            case 2:
              zeroFrameAry[i][j] = NonZeroNeighborAry[0];
              break;
            // some/all of a,b already have labels but they are not the same
            case 3:
              zeroFrameAry[i][j] = min(NonZeroNeighborAry);
              break;
          }
        }
      }
    }
    newMax = newLabel;
    System.out.println("4-Connectedness Pass 1 Complete!");
  }

  // 4-Connectedness Pass 2
  public void connect4Pass2(int[][] zeroFrameAry) {
    System.out.println("Processing 4-Connectedness Pass 2...");
    newMin = minVal;

    for(int i = numRow; i > 0; i--) {
      for(int j = numCol; j > 0; j--){
        if(zeroFrameAry[i][j] > 0) {
          // c, d, P(i,j)
          NonZeroNeighborAry[0] = zeroFrameAry[i][j+1];
          NonZeroNeighborAry[1] = zeroFrameAry[i+1][j];
          //NonZeroNeighborAry[2] = zeroFrameAry[i][j];

          switch(fourConPass2(NonZeroNeighborAry)) {
            // a = b = 0
            case 1:
              // Does nothing. Keeps label
              break;
            // some/all of a,b already have the same label
            case 2:
              // Does nothing. Keeps label
              break;
            // some/all of a,b already have labels but they are not the same
            case 3:
              int minLabel = min(NonZeroNeighborAry);
              if(zeroFrameAry[i][j] > minLabel) {
                EQAry[zeroFrameAry[i][j]] = minLabel;
                zeroFrameAry[i][j] = minLabel;
              }
              break;
          }
        }
      }
    }
    System.out.println("4-Connectedness Pass 2 Complete!");
  }

  // Pass 3
  public void connectPass3() {
    System.out.println("Processing Pass 3...");
    CCproperty = new Node[trueNumCC+1];
    
    for(int i = 0; i < trueNumCC+1; i++) {
      CCproperty[i] = new Node();
      CCproperty[i].label = i+1;
    }
    newMax = trueNumCC;

    for(int i = 1; i < numRow+1; i++) {
      for(int j = 1; j < numCol+1; j++) {
        if(zeroFrameAry[i][j] > 0) {
          zeroFrameAry[i][j] = EQAry[zeroFrameAry[i][j]];
          CCproperty[zeroFrameAry[i][j]-1].numPixels++;
        }
      }
    }
    System.out.println("Pass 3 Complete!");

    for(int k = 0; k < trueNumCC; k++) {
      for(int i = 1; i < numRow+1; i++) {
        for(int j = 1; j < numCol+1; j++) {
          CCproperty[k].label = k;
          if(i <= CCproperty[k].minR) {
            CCproperty[k].minR = i;
          }
          if(j <= CCproperty[k].minC) {
            CCproperty[k].minC = j;
          }
          if(i >= CCproperty[k].maxR) {
            CCproperty[k].maxR = i;
          }
          if(j >= CCproperty[k].maxC) {
            CCproperty[k].maxC = j;
          }
        }
      }
    }
  }

  public void drawBoxes(int[][] zeroFrameAry, Node[] CCproperty) {
    /*int index = 1;
    while(index < trueNumCC+1) {
      int minR = CCproperty[index].minR + 1;
      int minC = CCproperty[index].minC + 1;
      int maxR = CCproperty[index].maxR + 1;
      int maxC = CCproperty[index].minC + 1;
      int label = CCproperty[index].label;

      for(int i = minC; i < maxC; i++){
        zeroFrameAry[minR][i] = label;
      }

      for(int i = minC; i < maxC; i++){
        zeroFrameAry[minR][i] = label;
      }

      for(int i = minR; i < maxR; i++){
        zeroFrameAry[i][minC] = label;
      }

      for(int i = minR; i < maxR; i++){
        zeroFrameAry[i][maxC] = label;
      }
      
      index++;*/
    }

  public void updateEQ() {
    int readLabel = 0;
    int index = 1;
    while(index <= newLabel) {
      if(index == EQAry[index]) {
        readLabel++;
        EQAry[index] = readLabel;
      }
      else {
        EQAry[index] = EQAry[EQAry[index]];
      }
      index++;
    }
    trueNumCC = readLabel;
  }

  public int manageEQAry(int[] EQAry) {
    updateEQ();
    return trueNumCC;
  }

  public void printCCproperty(FileWriter file) {
    try {
      file.write(numRow + " " + numCol + " " + minVal + " " + maxVal + "\n" + trueNumCC + "\n");
      for(int i = 0; i < trueNumCC; i++){
        file.write(CCproperty[i].label + "\n" + CCproperty[i].numPixels + "\n" + CCproperty[i].minR + " " + CCproperty[i].minC + "\n" + CCproperty[i].maxR + " " + CCproperty[i].maxC + "\n");
      }
    }
    catch(IOException e) {
      e.printStackTrace();
    }
  }

  public void printEQAry(FileWriter file) {
    try {
      for(int i = 0; i < newLabel; i++){
        file.write(i + " " + EQAry[i] + "\n");
      }
    }
    catch(IOException e) {
      e.printStackTrace();
    }
  }

  public void printImg(FileWriter file) {
    try {
      file.write(numRow + " " + numCol + " " + newMin + " " + newMax + "\n");
      for(int i = 1; i <= numRow; i++) {
        for(int j = 1; j <= numCol; j++) {
          file.write(String.valueOf(zeroFrameAry[i][j]) + " ");
        }
        file.write("\n");
      }
    }
    catch(IOException e) {
      e.printStackTrace();
    }
  }
}
