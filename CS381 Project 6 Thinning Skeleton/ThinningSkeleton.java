import java.io.*;
import java.util.*;

public class ThinningSkeleton {
  private int numRow;
  private int numCol;
  private int minVal;
  private int maxVal;
  public int changeFlag = 1;
  public int[][] firstAry;
  public int[][] secondAry;

  public ThinningSkeleton(int numR, int numC, int minV, int maxV){
    // Obtain numRow, numCol, minVal, maxVal
    numRow = numR;
    numCol = numC;
    minVal = minV;
    maxVal = maxV;

    // Initialize arrays
    firstAry = new int[numRow+2][numCol+2];
    secondAry = new int[numRow+2][numCol+2];
  }

  public void zeroFrame(int[][] ary) {
    for(int i = 0; i < numRow+2; i++) {
      for(int j = 0; j < numCol+2; j++) {
        ary[i][j] = 0;
      }
    }
  }

  public void loadImage(Scanner inFile, int[][] firstAry, int[][] secondAry) {
    for(int i = 1; i <= numRow; i++) {
      for(int j = 1; j <= numCol; j++) {
        if(inFile.hasNextInt()) firstAry[i][j] = inFile.nextInt();
        secondAry[i][j] = firstAry[i][j];
      }
    }
  }  

  public void copyArys(int[][] firstAry, int[][] secondAry) {
    for(int i = 1; i <= numRow; i++) {
      for(int j = 1; j <= numCol; j++) {
        firstAry[i][j] = secondAry[i][j];
      }
    }
  }
  
  public boolean checkConditions(int[][] ary, int i, int j) {
    //Case 1
    if(ary[i][j-1] == 0 && ary[i][j+1] == 0) {
      if((ary[i-1][j-1] == 1 || ary[i-1][j] == 1 || ary[i-1][j+1] == 1) &&
          (ary[i+1][j-1] == 1 || ary[i+1][j] == 1 || ary[i+1][j+1] == 1)) {
        return true;
      }
    }

    //Case 2
    if(ary[i-1][j] == 0 && ary[i+1][j] == 0) {
      if((ary[i-1][j-1] == 1 ||ary[i][j-1] == 1 || ary[i+1][j-1] == 1) &&
          (ary[i-1][j+1] == 1 || ary[i][j+1] == 1 || ary[i+1][j+1] == 1)) {
        return true;
      }  
    }

    //Case alpha
    if(ary[i][j-1] == 0 && ary[i-1][j] == 0 && ary[i-1][j-1] == 1) {
      return true;
    }

    //Case beta
    if(ary[i][j-1] == 0 && ary[i+1][j] == 0 && ary[i+1][j-1] == 1) {
      return true;
    }

    //Case gamma
    if(ary[i-1][j] == 0 && ary[i][j+1] == 0 && ary[i-1][j+1] == 1) {
      return true;
    }

    //Case delta
    if(ary[i][j+1] == 0 && ary[i+1][j] == 0 && ary[i+1][j+1] == 1) {
      return true;
    }
    return false;
  }

  public int objectNeighbors(int[][] ary, int i, int j) {
    int counter = 0;
    for(int r = i-1; r <= i+1; r++) {
      for(int c = j-1; c <= j+1; c++) {
        if(r == i && c == j) {
          continue;
        }
        if(ary[r][c] > 0) {
          counter++;
        }
      }
    }
    return counter;
  }

  public boolean ntcheck(int[][] firstAry, int i, int j) {
    if(firstAry[i-1][j] != 0) return false;
    if(objectNeighbors(firstAry, i, j) < 4) return false;
    if(checkConditions(firstAry, i, j)) return false;
    return true;
  }

  public boolean stcheck(int[][] firstAry, int i, int j) {
    if(firstAry[i+1][j] != 0) return false;
    if(objectNeighbors(firstAry, i, j) < 4) return false;
    if(checkConditions(firstAry, i, j)) return false;
    return true;
  }

  public boolean wtcheck(int[][] firstAry, int i, int j) {
    if(firstAry[i][j-1] != 0) return false;
    if(objectNeighbors(firstAry, i, j) < 3) return false;
    if(checkConditions(firstAry, i, j)) return false;
    return true;
  }

  public boolean etcheck(int[][] firstAry, int i, int j) {
    if(firstAry[i][j+1] != 0) return false;
    if(objectNeighbors(firstAry, i, j) < 3) return false;
    if(checkConditions(firstAry, i, j)) return false;
    return true;
  }

  public void northThinning(int[][] firstAry, int[][] secondAry) {
    for(int i = 1; i <= numRow; i++) {
      for(int j = 1; j <= numCol; j++) {
        if(firstAry[i][j] > 0) {
          if(ntcheck(firstAry, i, j)) {
            secondAry[i][j] = 0;
            changeFlag++;
          }
        }
      }
    }
  }

  public void southThinning(int[][] firstAry, int[][] secondAry) {
    for(int i = 1; i <= numRow; i++) {
      for(int j = 1; j <= numCol; j++) {
        if(firstAry[i][j] > 0) {
          if(stcheck(firstAry, i, j)) {
            secondAry[i][j] = 0;
            changeFlag++;
          }
        }
      }
    }
  }

  public void westThinning(int[][] firstAry, int[][] secondAry) {
    for(int i = 1; i <= numRow; i++) {
      for(int j = 1; j <= numCol; j++) {
        if(firstAry[i][j] > 0) {
          if(wtcheck(firstAry, i, j)) {
            secondAry[i][j] = 0;
            changeFlag++;
          }
        }
      }
    }
  }

  public void eastThinning(int[][] firstAry, int[][] secondAry) {
    for(int i = 1; i <= numRow; i++) {
      for(int j = 1; j <= numCol; j++) {
        if(firstAry[i][j] > 0) {
          if(etcheck(firstAry, i, j)) {
            secondAry[i][j] = 0;
            changeFlag++;
          }
        }
      }
    }
  }

  public void reformatPrettyPrint(int[][]firstAry, FileWriter file) {
    try {
      file.write(numRow + " " + numCol + " " + minVal + " " + maxVal + "\n");
      for(int i = 1; i <= numRow; i++) {
        for(int j = 1; j <= numCol; j++) {
          if(firstAry[i][j] > 0) {
            file.write(String.valueOf(firstAry[i][j]) + " ");
          }
          else{
            file.write(". ");
          }
        }
        file.write("\n");
      }
    }
    catch(IOException e) {
      e.printStackTrace();
    }
  }
}