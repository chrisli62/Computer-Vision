#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

class ImageProcessing {
  private:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newMinVal;
    int newMaxVal;
    int** zeroFramedAry;
    int** skeletonAry;
    int neighborAry[9];
  
  public:
    ImageProcessing(ifstream& inFile, int** zAry, int** sAry, int numR, int numC, int minV, int maxV, int newMin, int newMax) {

      zeroFramedAry = zAry;
      skeletonAry = sAry;

      numRows = numR;
      numCols = numC;
      minVal = minV;
      maxVal = maxV;
      newMinVal = newMin;
      newMaxVal = newMax;
    }

    ~ImageProcessing() {
      for(int i = 0; i < numRows+2; i++) {
        delete[] zeroFramedAry[i];
        delete[] skeletonAry[i];
      }
      delete[] zeroFramedAry;
      delete[] skeletonAry;
    }

    void setZero(int** ary) {
      for(int i = 0; i < numRows+2; i++) {
        for(int j = 0; j < numCols+2; j++) {
          ary[i][j] = 0;
        }
      }
    }

    void loadImage(ifstream& imgFile, int** ary) {
      int image;
      for(int i = 1; i <= numRows; i++){
        for(int j = 1; j <= numCols; j++){
          imgFile >> image;
          ary[i][j] = image;
        }
      }
    }

    void compute8Distance(int** zeroFramedAry, ofstream& outFile) {
      firstPass8Distance(zeroFramedAry);
      outFile << "First Pass 8 Distance:\n";
      reformatPrettyPrint(zeroFramedAry, outFile);
      secondPass8Distance(zeroFramedAry);
      outFile << "Second Pass 8 Distance:\n";
      reformatPrettyPrint(zeroFramedAry, outFile);
    }

    int getMin(bool check, int i, int j) {
      if(check == true) {
        neighborAry[0] = zeroFramedAry[i-1][j-1] + 1;
        neighborAry[1] = zeroFramedAry[i-1][j] + 1;
        neighborAry[2] = zeroFramedAry[i-1][j+1] + 1;
        neighborAry[3] = zeroFramedAry[i][j-1] + 1;
        int min = neighborAry[0];
        for(int k = 1; k < 4; k++) {
          if(min > neighborAry[k]) min = neighborAry[k];
        }
        return min;
      }
      else {
        neighborAry[4] = zeroFramedAry[i][j+1] + 1;
        neighborAry[5] = zeroFramedAry[i+1][j-1] + 1;
        neighborAry[6] = zeroFramedAry[i+1][j] + 1;
        neighborAry[7] = zeroFramedAry[i+1][j+1] + 1;
        neighborAry[8] = zeroFramedAry[i][j];
        int min = neighborAry[4];
        for(int k = 4; k <= 8; k++) {
          if(min > neighborAry[k]) min = neighborAry[k];
        }
        return min;
      }
    }

    void firstPass8Distance(int** zeroFramedAry) {
      // Using a switch to differenitate 1st pass and 2nd pass mins
      bool check = true;
      int counter = 0;
      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          if(zeroFramedAry[i][j] > 0) {
            zeroFramedAry[i][j] = getMin(check, i, j);
          }
          if(counter == 0) {
            newMinVal = zeroFramedAry[i][j];
            newMaxVal = zeroFramedAry[i][j];
            counter++;
          }
          else {
            if(newMinVal > zeroFramedAry[i][j]) newMinVal = zeroFramedAry[i][j];
            if(newMaxVal > zeroFramedAry[i][j]) newMaxVal = zeroFramedAry[i][j];
          }
        }
      }
    }

    void secondPass8Distance(int** zeroFramedAry) {
      bool check = false;
      int counter = 0;
      for(int i = numRows; i >= 1; i--) {
        for(int j = numCols; j >= 1; j--) {
          if(zeroFramedAry[i][j] > 0) {
            zeroFramedAry[i][j] = getMin(check, i, j);
          }
          if(counter == 0) {
            newMinVal = zeroFramedAry[i][j];
            newMaxVal = zeroFramedAry[i][j];
            counter++;
          }
          else {
            if(newMinVal > zeroFramedAry[i][j]) newMinVal = zeroFramedAry[i][j];
            if(newMaxVal > zeroFramedAry[i][j]) newMaxVal = zeroFramedAry[i][j];
          }
        }
      }
    }

    bool isLocalMaxima(int** zeroFramedAry, int i, int j) {
      // If zeroFramedAry (i, j) >= all its *8-connected* neighbors
      if(zeroFramedAry[i][j] >= zeroFramedAry[i-1][j-1]
         && zeroFramedAry[i][j] >= zeroFramedAry[i-1][j]
         && zeroFramedAry[i][j] >= zeroFramedAry[i-1][j+1]
         && zeroFramedAry[i][j] >= zeroFramedAry[i][j-1]
         && zeroFramedAry[i][j] >= zeroFramedAry[i][j+1]
         && zeroFramedAry[i][j] >= zeroFramedAry[i+1][j-1]
         && zeroFramedAry[i][j] >= zeroFramedAry[i+1][j]
         && zeroFramedAry[i][j] >= zeroFramedAry[i+1][j+1]) {
          return 1;
      }
      else return 0;
    }

    void computeLocalMaxima(int** zeroFramedAry, int** skeletonAry) {
      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          if(isLocalMaxima(zeroFramedAry, i, j) == 1){
            skeletonAry[i][j] = zeroFramedAry[i][j];
          }
          else {
            skeletonAry[i][j] = 0;
          }
        }
      }
    }

    void extractLocalMaxima(int** skeletonAry, ofstream& skeletonFile) {
      skeletonFile << numRows << " " << numCols << " " << newMinVal << " " << newMaxVal << endl;
      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          if(skeletonAry[i][j] > 0) {
            skeletonFile << i << " " << j << " " << skeletonAry[i][j] << endl;
          }
        }
      }
    }

    void skeletonExtraction(int** zeroFramedAry, int** skeletonAry, ofstream& skeletonFile, ofstream& outFile) {
      computeLocalMaxima(zeroFramedAry, skeletonAry);
      outFile << "Skeleton Extraction:\n";
      reformatPrettyPrint(skeletonAry, outFile);
      extractLocalMaxima(skeletonAry, skeletonFile);
    }

    void skeletonExpansion(int** zeroFramedAry, ifstream& skeletonFile, ofstream& outFile) {
      setZero(zeroFramedAry);
      loadSkeleton(skeletonFile,zeroFramedAry);
      firstPassExpansion(zeroFramedAry);
      outFile << "First Pass Skeleton Expansion:\n";
      reformatPrettyPrint(zeroFramedAry, outFile);
      secondPassExpansion(zeroFramedAry);
      outFile << "Second Pass Skeleton Expansion:\n";
      reformatPrettyPrint(zeroFramedAry, outFile);
    }

    void loadSkeleton(ifstream& skeletonFile, int** zeroFramedAry) {
      int value;
      if(skeletonFile>>value) numRows = value;
      if(skeletonFile>>value) numCols = value;
      if(skeletonFile>>value) minVal = value;
      if(skeletonFile>>value) maxVal = value;
      int row, col, val;
      while(skeletonFile>>row>>col>>val) {
        zeroFramedAry[row][col] = val;
      }
    }

    int getMax(bool check, int i, int j) {
      if(check) {
        neighborAry[0] = zeroFramedAry[i-1][j-1] - 1;
        neighborAry[1] = zeroFramedAry[i-1][j] - 1;
        neighborAry[2] = zeroFramedAry[i-1][j+1] - 1;
        neighborAry[3] = zeroFramedAry[i][j-1] - 1;
        neighborAry[4] = zeroFramedAry[i][j+1] - 1;
        neighborAry[5] = zeroFramedAry[i+1][j-1] - 1;
        neighborAry[6] = zeroFramedAry[i+1][j] - 1;
        neighborAry[7] = zeroFramedAry[i+1][j+1] - 1;
        int max = neighborAry[0];
        for(int k = 1; k < 8; k++) {
          if(max < neighborAry[k]) max = neighborAry[k];
        }
        return max;
      }
      else {
        neighborAry[0] = zeroFramedAry[i-1][j-1];
        neighborAry[1] = zeroFramedAry[i-1][j];
        neighborAry[2] = zeroFramedAry[i-1][j+1];
        neighborAry[3] = zeroFramedAry[i][j-1];
        neighborAry[4] = zeroFramedAry[i][j+1];
        neighborAry[5] = zeroFramedAry[i+1][j-1];
        neighborAry[6] = zeroFramedAry[i+1][j];
        neighborAry[7] = zeroFramedAry[i+1][j+1];
        int max = neighborAry[0];
        for(int k = 1; k < 8; k++) {
          if(max < neighborAry[k]) max = neighborAry[k];
        }
        return max;
      }
    }

    void firstPassExpansion(int** zeroFramedAry) {
      bool check = true;
      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          if(zeroFramedAry[i][j] == 0) {
            if(zeroFramedAry[i][j] < getMax(check, i, j)){
              zeroFramedAry[i][j] = getMax(check, i, j);
            }
          }
        }
      }
    }

    void secondPassExpansion(int** zeroFramedAry) {
      bool check = false;
      for(int i = numRows; i >= 1; i--) {
        for(int j = numCols; j >= 1; j--) {
          if(zeroFramedAry[i][j] < getMax(check, i, j)){
            zeroFramedAry[i][j] = getMax(check, i, j)-1;
          }
        }
      }
    }

    void ary2File(int** zeroFramedAry, ofstream& decompressFile) {
      decompressFile << numRows << " " << numCols << " " << newMinVal << " " << newMaxVal << endl;
      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          if(zeroFramedAry[i][j] >= 1) {
            decompressFile << "1 ";
          }
          else {
            decompressFile << "0 ";
          }
        }
        decompressFile << endl;
      }
    }

    void reformatPrettyPrint(int** zeroFramedAry, ofstream& outFile) {
      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          if(zeroFramedAry[i][j] == 0) {
            outFile << ". ";
          }
          else {
            outFile << zeroFramedAry[i][j] << " ";
          }
        }
        outFile << endl;
      }
    }
};

int main(int argc, char* argv[]) {
  ifstream inFile;
  ofstream outFile1, outFile2, skeletonFile, decompressedFile;

  inFile.open(argv[1]);
  outFile1.open(argv[2]);
  outFile2.open(argv[3]);

  int numRows, numCols, minVal, maxVal;
  int newMinVal, newMaxVal;
  int value;
  if(inFile>>value) numRows = value;
  if(inFile>>value) numCols = value;
  if(inFile>>value) minVal = value;
  if(inFile>>value) maxVal = value;

  int** zeroFramedAry;
  int** skeletonAry;
  //Dynamically allocate the 2D arrays
  zeroFramedAry = new int*[numRows+2];
  skeletonAry = new int*[numRows+2];
  for(int i = 0; i < numRows+2; i++) {
    zeroFramedAry[i] = new int[numCols+2];
    skeletonAry[i] = new int[numCols+2];
  }

  string skeletonFileName = argv[1];
  string decompressedFileName = argv[1];
  skeletonFile.open(skeletonFileName + "_skeleton.txt");
  decompressedFile.open(decompressedFileName + "_decompressed.txt");

  ImageProcessing m(inFile, zeroFramedAry, skeletonAry, numRows, numCols, minVal, maxVal, newMinVal, newMaxVal);
  m.setZero(zeroFramedAry);
  m.setZero(skeletonAry);
  m.loadImage(inFile, zeroFramedAry);
  m.compute8Distance(zeroFramedAry, outFile1);
  m.skeletonExtraction(zeroFramedAry, skeletonAry, skeletonFile, outFile1);
  skeletonFile.close();
  ifstream skeletonFile2;
  skeletonFile2.open(skeletonFileName + "_skeleton.txt");
  m.skeletonExpansion(zeroFramedAry, skeletonFile2, outFile2);
  m.ary2File(zeroFramedAry, decompressedFile);

  // Close all files
  inFile.close();
  outFile1.close();
  outFile2.close();
  skeletonFile.close();
  decompressedFile.close();
}