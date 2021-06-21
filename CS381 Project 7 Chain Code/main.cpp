#include <iostream>
#include <fstream>
using namespace std;

class Image {
  private:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int label;
    int numBoundaryPts;
    int** zeroFramedAry;
    int** imgAry;
    ifstream inFile, chainCodeFile1, boundaryFile1;
    ofstream chainCodeFile, boundaryFile, decompressedFile;
  
  public:
    Image(ifstream& inFile, int** zAry, int** iAry, int numR, int numC, int minV, int maxV) {

      zeroFramedAry = zAry;
      imgAry = iAry;

      numRows = numR;
      numCols = numC;
      minVal = minV;
      maxVal = maxV;
    }

    ~Image() {
      for(int i = 0; i < numRows+2; i++) {
        delete[] zeroFramedAry[i];
        delete[] imgAry[i];
      }
      delete[] zeroFramedAry;
      delete[] imgAry;
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

    class ChainCode {
      struct Point {
        int row;
        int col;
      };

      private:
        Image *im;
        Point startP;
        Point currentP;
        Point nextP;
        Point neighborCoord[8];
        int lastZero;
        int dirTable[8] = {6,0,0,2,2,4,4,6};
        int chainDir;
        int iRow;
        int jCol;
        int direction;
        int imgvalue;
        int dirIndex;

      public:
        void getChainCode(int numRows, int numCols, int minVal, int maxVal, int label, int** zeroFramedAry, ofstream& chainCodeFile) {
          chainCodeFile<<numRows<<" "<<numCols<<" "<<minVal
            <<" "<<maxVal<<"\n";
          
          for(int i = 1; i <= numRows; i++) {
            for(int j = 1; j <= numCols; j++) {
              startP.row = i;
              startP.col = j;
              if(zeroFramedAry[i][j] > 0) {
                int startRow = i;
                int startCol = j;
                currentP.row = i;
                currentP.col = j;
                int lastZero = 4;

                chainCodeFile<<currentP.row <<" "<<currentP.col
                  <<" "<<zeroFramedAry[i][j]<<"\n";
                
                imgvalue = zeroFramedAry[i][j];

                //goto is used here to break out of nested for loop
                goto endOfLoop;
              }
            }
          }
          
          endOfLoop:
            //counter keeps track of how many times startP is visited
            int counter = 0;
            label = 1;
            chainCodeFile<<label<<" ";
            while(counter < 2) {
              if(currentP.row == startP.row && currentP.col == startP.col) {
                counter++;
              } 
              direction = (lastZero+1)%8;
              chainDir = findNextP(direction, currentP, imgvalue, zeroFramedAry);
              nextP.row = neighborCoord[chainDir].row;
              nextP.col = neighborCoord[chainDir].col;
              chainCodeFile<<chainDir<<" ";
              if(chainDir == 0) dirIndex = 7;
              else dirIndex = chainDir-1;
              currentP.row = nextP.row;
              currentP.col = nextP.col;
              lastZero = dirTable[dirIndex];
            }
        }

        int findNextP(int direction, Point currentP, int imgvalue, int** zeroFramedAry) {
          int chainDir;
          loadNeighborCoord(currentP);
          while(true) {  
            if(zeroFramedAry[neighborCoord[direction].row][neighborCoord[direction].col] == imgvalue) {
              chainDir = direction;
              break;
            }
            direction = (direction+1)%8;
          }
          return chainDir;
        }

        void loadNeighborCoord(Point p) {
          neighborCoord[0].row = p.row;
          neighborCoord[0].col = p.col+1;
          neighborCoord[1].row = p.row-1;
          neighborCoord[1].col = p.col+1;
          neighborCoord[2].row = p.row-1;
          neighborCoord[2].col = p.col;
          neighborCoord[3].row = p.row-1;
          neighborCoord[3].col = p.col-1;
          neighborCoord[4].row = p.row;
          neighborCoord[4].col = p.col-1;
          neighborCoord[5].row = p.row+1;
          neighborCoord[5].col = p.col-1;
          neighborCoord[6].row = p.row+1;
          neighborCoord[6].col = p.col;
          neighborCoord[7].row = p.row+1;
          neighborCoord[7].col = p.col+1;
        }

        void constructBoundary(int numRows, int numCols, int minVal, int maxVal, int** imgAry, ifstream& chainCodeFile, ofstream& boundaryFile) {
          int value;
          int labels, startR, startC, startVal;
          if(chainCodeFile>>value) numRows = value;
          if(chainCodeFile>>value) numCols = value;
          if(chainCodeFile>>value) minVal = value;
          if(chainCodeFile>>value) maxVal = value;
          if(chainCodeFile>>value) startR = value;
          if(chainCodeFile>>value) startC = value;
          if(chainCodeFile>>value) startVal = value;
          if(chainCodeFile>>value) labels = value;

          boundaryFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<labels<<"\n";

          imgAry[startR][startC] = labels;
          currentP.row = startR;
          currentP.col = startC;
          while(chainCodeFile>>value) {
            direction = value;
            //Update currentP
            switch(direction) {
              case 0:
                currentP.row = currentP.row;
                currentP.col = currentP.col+1;
                break;
              case 1:
                currentP.row = currentP.row-1;
                currentP.col = currentP.col+1;
                break;
              case 2:
                currentP.row = currentP.row-1;
                currentP.col = currentP.col;
                break;
              case 3:
                currentP.row = currentP.row-1;
                currentP.col = currentP.col-1;
                break;
              case 4:
                currentP.row = currentP.row;
                currentP.col = currentP.col-1;
                break;
              case 5:
                currentP.row = currentP.row+1;
                currentP.col = currentP.col-1;
                break;
              case 6:
                currentP.row = currentP.row+1;
                currentP.col = currentP.col;
                break;
              case 7:
                currentP.row = currentP.row+1;
                currentP.col = currentP.col+1;
                break;
            }
            imgAry[currentP.row][currentP.col] = labels;
          }

          for(int i = 1; i <= numRows; i++) {
            for(int j = 1; j <= numCols; j++) {
              boundaryFile<<imgAry[i][j]<<" ";
            }
            boundaryFile<<endl;
          }
        }

        void fillInterior(int numRows, int numCols, int minVal, int maxVal, int** imgAry, ifstream& boundaryFile, ofstream& decompressedFile) {
          int value;
          if(boundaryFile>>value) numRows = value;
          if(boundaryFile>>value) numCols = value;
          if(boundaryFile>>value) minVal = value;
          if(boundaryFile>>value) maxVal = value;

          decompressedFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
          
          for(int i = 1; i <= numRows; i++) {
            for(int j = 1; j <= numCols; j++) {
              if(imgAry[i][j] == 0) {
                if((imgAry[i][j-1] > 0 || imgAry[i][j-1] == -1) && (imgAry[i-1][j] > 0 || imgAry[i-1][j] == -1)) {
                  imgAry[i][j] = -1;
                }
              }
            }
          }
          for(int r = numRows; r >= 1; r--) {
            for(int c = numCols; c >= 1; c--) {
              if(imgAry[r][c] == -1){
                if(imgAry[r][c+1] == 0 || imgAry[r+1][c] == 0) {
                  imgAry[r][c] = 0;
                }
                else imgAry[r][c] = 1;
              }
            }
          }
          for(int i = 1; i <= numRows; i++) {
            for(int j = 1; j <= numCols; j++) {
              decompressedFile<<imgAry[i][j]<<" ";
            }
            decompressedFile<<"\n";
          }
        }
    };
};

int main(int argc, char* argv[]) {
  ifstream inFile;
  inFile.open(argv[1]);

  int numRows, numCols, minVal, maxVal, label;
  int newMinVal, newMaxVal;
  int value;
  if(inFile>>value) numRows = value;
  if(inFile>>value) numCols = value;
  if(inFile>>value) minVal = value;
  if(inFile>>value) maxVal = value;

  int** zeroFramedAry;
  int** imgAry;
  //Dynamically allocate the 2D arrays
  zeroFramedAry = new int*[numRows+2];
  imgAry = new int*[numRows+2];
  for(int i = 0; i < numRows+2; i++) {
    zeroFramedAry[i] = new int[numCols+2];
    imgAry[i] = new int[numCols+2];
  }

  ofstream chainCodeFile, boundaryFile, decompressedFile;
  
  string ccFileName = argv[1];
  string skeletonFileName = argv[1];
  string decompressedFileName = argv[1];
  
  chainCodeFile.open(ccFileName + "_chainCode.txt");
  boundaryFile.open(boundaryFileName + "_boundary.txt");
  decompressedFile.open(decompressedFileName + "_decompressed.txt");

  Image m(inFile, zeroFramedAry, imgAry, numRows, numCols, minVal, maxVal);
  m.loadImage(inFile, zeroFramedAry);

  Image::ChainCode cc;
  cc.getChainCode(numRows,numCols,minVal,maxVal,label,zeroFramedAry,chainCodeFile);

  chainCodeFile.close();
  ifstream chainCodeFile1;
  chainCodeFile1.open(ccFileName + "_chainCode.txt");

  cc.constructBoundary(numRows,numCols,minVal,maxVal,imgAry,chainCodeFile1,boundaryFile);

  boundaryFile.close();
  ifstream boundaryFile1;
  boundaryFile1.open(boundaryFileName + "_boundary.txt");
  
  cc.fillInterior(numRows,numCols,minVal,maxVal,imgAry,boundaryFile1,decompressedFile);

  inFile.close();
  chainCodeFile.close();
  boundaryFile.close();
  decompressedFile.close();
}