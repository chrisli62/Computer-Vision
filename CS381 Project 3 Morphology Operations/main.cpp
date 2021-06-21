#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

class Morphology{
public:
  int numImgRows;
  int numImgCols;
  int imgMin;
  int imgMax;
  int numStructRows;
  int numStructCols;
  int structMin;
  int structMax;
  int rowOrigin;
  int colOrigin;
  int rowFrameSize; 
  int colFrameSize; 
  int extraRows;
  int extraCols;

  int** zeroFramedAry;
  int** morphAry;
  int** tempAry;
  int** structAry;

  Morphology(ifstream& iFile, ifstream& sFile, ofstream& dOutFile, ofstream& eOutFile, ofstream& oOutFile, ofstream& cOutFile, ofstream& pPrintFile, int** zAry, int** mAry, int** tAry, int** sAry, int imgRow, int imgCol, int iMin, int iMax, int struRow, int struCol, int sMin, int sMax, int rOrigin, int cOrigin, int rFrame, int cFrame){

    zeroFramedAry = zAry;
    morphAry = mAry;
    tempAry = tAry;
    structAry = sAry;

    numImgRows = imgRow;
    numImgCols = imgCol;
    imgMin = iMin;
    imgMax = iMax;

    numStructRows = struRow;
    numStructCols = struCol;
    structMin = sMin;
    structMax = sMax;
    rowOrigin = rOrigin;
    colOrigin = cOrigin;
    rowFrameSize = rFrame;
    colFrameSize = cFrame;
  }

  ~Morphology(){
    for(int i = 0; i < numImgRows+rowFrameSize; i++){
      delete[] zeroFramedAry[i];
      delete[] morphAry[i];
      delete[] tempAry[i];
    }
    delete[] zeroFramedAry;
    delete[] morphAry;
    delete[] tempAry;
    
    for(int i = 0; i < numStructRows; i++){
      delete[] structAry[i];
    }
    delete[] structAry;
  }

  void loadImg(ifstream& imgFile, int** zeroFramedAry){
    int image;
    for(int i = rowFrameSize/2; i <(numImgRows+rowFrameSize)-(rowFrameSize/2); i++){
      for(int j = colFrameSize/2; j <(numImgCols+colFrameSize)-(colFrameSize/2); j++){
        imgFile >> image;
        zeroFramedAry[i][j] = image;
      }
    }
  }

  void loadStruct(ifstream& structFile, int** structAry){
    int structImg;
    for(int i = 0; i < numStructRows; i++){
      for(int j = 0 ; j < numStructCols; j++){
        structFile >> structImg;
        structAry[i][j] = structImg;
      }
    }
  }

  void zero2DAry(int** inAry, int row, int col){
    for(int i = 0; i < row; i++){
      for(int j = 0; j < col; j++){
        inAry[i][j] = 0;
      }
    }
  }

  void computeDilation(int** inAry, int** outAry){
    int i = rowFrameSize;
    while(i < (numImgRows + rowFrameSize)){
      int j = colFrameSize;
      while(j < (numImgCols + colFrameSize)){
        if(inAry[i][j] > 0){ 
          dilation(i, j, inAry, outAry);
        }
        j++;
      }
      i++;
    }
  }

  void computeErosion(int** inAry, int** outAry){
    int i = rowFrameSize;
    while(i < (numImgRows + rowFrameSize)){
      int j = colFrameSize;
      while(j < (numImgCols + colFrameSize)){
        if(inAry[i][j] > 0){ 
          erosion(i, j, inAry, outAry);
        }
        j++;
      }
      i++;
    }
  }

  void computeOpening(int** zeroFramedAry, int** morphAry, int** tempAry){
    computeErosion (zeroFramedAry, tempAry);
    computeDilation (tempAry, morphAry);
  }

  void computeClosing(int** zeroFramedAry, int** morphAry, int** tempAry){
    computeDilation (zeroFramedAry, tempAry);
    computeErosion (tempAry, morphAry); 
  }

  void dilation(int i, int j, int** inAry, int** outAry){
    int iOffset = i - rowOrigin;
    int jOffset = j - colOrigin;

    int rIndex = 0;
    while(rIndex < numStructRows){
      int cIndex = 0;
      while(cIndex < numStructCols){
        //The if statement was just reproducing the input image.
        //Fixed so that it will go through the structure to set the 1s.
        if(structAry[rIndex][cIndex] == 1){
          outAry[iOffset + rIndex][jOffset + cIndex] = 1;
        }
        cIndex++;
      }
      rIndex++;
    }
  }

  void erosion(int i, int j, int** inAry, int** outAry){
    int iOffset = i - rowOrigin;
    int jOffset = j - colOrigin;
    bool matchFlag = true;

    int rIndex = 0;
    int cIndex;
    while(matchFlag == true && rIndex < numStructRows){
      cIndex = 0;
      while(matchFlag == true && cIndex < numStructCols){
        if(structAry[rIndex][cIndex] > 0 && inAry[iOffset + rIndex][jOffset + cIndex] <= 0){
          matchFlag = false;
        }
        cIndex++;
      }
      rIndex++;
    }
    //If statement in specs had an error where offset + index should have
    //been the original row and column.
    if(matchFlag == true){
      outAry[i][j] = 1;
    }
    else{
      outAry [i][j] = 0;
    }
  }

  void aryToFile(int** ary, ofstream& outFile, int row, int col){
    outFile << numImgRows << " " << numImgCols << " " << imgMin << " " << imgMax << endl;
    for(int i = rowFrameSize/2; i < row-(rowFrameSize/2); i++){
      for(int j = colFrameSize/2; j < col-(colFrameSize/2); j++){
        outFile << ary[i][j] << " ";
      }
      outFile << endl;
    }
  }

  void prettyPrint(int** ary, ofstream& outFile, int row, int col){
    //Used to print out structure
    if(int (row == numStructRows) && (col == numStructCols)){
      for(int r = 0; r < numStructRows; r++){
        for(int c = 0; c < numStructCols; c++){
          if(ary[r][c] == 0){
            outFile << ". ";
          }
          else{
            outFile << ary[r][c] << " ";
          }
        }
        outFile << endl;
      }
    }
    //Used to print everything else
    else{
      for(int i = rowFrameSize/2; i < row-(rowFrameSize/2); i++){
        for(int j = colFrameSize/2; j < col-(colFrameSize/2); j++){
          if(ary[i][j] == 0){
            outFile << ". ";
          }
          else{
            outFile << ary[i][j] << " ";
          }
        }
        outFile << endl;
      }
    }
  }
};


int main(int argc, char* argv[]) {
  ifstream imgFile, structFile;
  ofstream dilateOutFile, erodeOutFile, openingOutFile, closingOutFile, prettyPrintFile;

  imgFile.open(argv[1]);
  structFile.open(argv[2]);
  dilateOutFile.open(argv[3]);
  erodeOutFile.open(argv[4]);
  openingOutFile.open(argv[5]); 
  closingOutFile.open(argv[6]); 
  prettyPrintFile.open(argv[7]);

  int numImgRows, numImgCols, imgMin, imgMax;
  int numStructRows, numStructCols, structMin, structMax;
  int rowOrigin, colOrigin;

  int value;
  if(imgFile>>value) numImgRows = value;
  if(imgFile>>value) numImgCols = value;
  if(imgFile>>value) imgMin = value;
  if(imgFile>>value) imgMax = value;

  int svalue;
  if(structFile>>value) numStructRows = value;
  if(structFile>>value) numStructCols = value;
  if(structFile>>value) structMin = value;
  if(structFile>>value) structMax = value;
  if(structFile>>value) rowOrigin = value;
  if(structFile>>value) colOrigin = value;

  int rowFrameSize, colFrameSize;
  rowFrameSize = numStructRows/2;
  colFrameSize = numStructCols/2;

  int extraRows, extraCols;
  extraRows = 2*numStructRows;
  extraCols = 2*numStructCols;

  //Required as 1st column was missing with above definition of rowFrameSize and colFrameSize
  if(numStructRows%2==0) rowFrameSize = numStructRows;
  else rowFrameSize = numStructRows+1;
  if(numStructCols%2==0) colFrameSize = numStructCols;
  else colFrameSize = numStructCols+1;

  //variables to shorten the amount of code being read
  int aryRows = numImgRows+rowFrameSize;
  int aryCols = numImgCols+colFrameSize;

  int** zeroFramedAry;
  int** structAry;
  int** morphAry;
  int** tempAry;

  //Dynamically allocate the 2D arrays
  zeroFramedAry = new int*[aryRows];
  morphAry = new int*[aryRows];
  tempAry = new int*[aryRows];
  for(int i = 0; i < aryRows; i++) {
    zeroFramedAry[i] = new int[aryCols];
    morphAry[i] = new int[aryCols];
    tempAry[i] = new int[aryCols];
  }

  structAry = new int*[numStructRows];
  for(int i = 0; i < numStructRows; i++){
    structAry[i] = new int[numStructCols];
  }

  Morphology m(imgFile, structFile, dilateOutFile, erodeOutFile, openingOutFile,closingOutFile, prettyPrintFile, zeroFramedAry, morphAry, tempAry, structAry, numImgRows, numImgCols, imgMin, imgMax, numStructRows, numStructCols, structMin, structMax, rowOrigin, colOrigin, rowFrameSize, colFrameSize);

  m.zero2DAry(zeroFramedAry, aryRows, aryCols);
  m.loadImg (imgFile, zeroFramedAry); 
  prettyPrintFile << "Loaded Image" << endl;
  m.prettyPrint (zeroFramedAry, prettyPrintFile, aryRows, aryCols);
  prettyPrintFile << endl; //line buffer between pretty prints

  m.zero2DAry(structAry, numStructRows, numStructCols);
  m.loadStruct(structFile, structAry);
  prettyPrintFile << "Loaded Structure" << endl;
  m.prettyPrint (structAry, prettyPrintFile, numStructRows, numStructCols);
  prettyPrintFile << endl;

  m.zero2DAry(morphAry, aryRows, aryCols);
  m.computeDilation (zeroFramedAry, morphAry); 
  m.aryToFile (morphAry, dilateOutFile, aryRows, aryCols); 
  prettyPrintFile << "Dilated Image" << endl;
  m.prettyPrint (morphAry, prettyPrintFile, aryRows, aryCols);
  prettyPrintFile << endl;

  m.zero2DAry(morphAry, aryRows, aryCols);
  m.computeErosion (zeroFramedAry, morphAry); 
  m.aryToFile (morphAry, erodeOutFile, aryRows, aryCols);
  prettyPrintFile << "Eroded Image" << endl;
  m.prettyPrint (morphAry, prettyPrintFile, aryRows, aryCols);
  prettyPrintFile << endl;

  m.zero2DAry(morphAry, aryRows, aryCols);
  m.computeOpening (zeroFramedAry, morphAry, tempAry);
  m.aryToFile (morphAry, openingOutFile, aryRows, aryCols);
  prettyPrintFile << "Opening Image" << endl;
  m.prettyPrint (morphAry, prettyPrintFile, aryRows, aryCols);
  prettyPrintFile << endl;

  m.zero2DAry(morphAry, aryRows, aryCols);
  m.computeClosing (zeroFramedAry, morphAry, tempAry); 
  m.aryToFile (morphAry, closingOutFile, aryRows, aryCols);
  prettyPrintFile << "Closing Image" << endl;
  m.prettyPrint (morphAry, prettyPrintFile, aryRows, aryCols);

  imgFile.close();
  structFile.close();
  dilateOutFile.close();
  erodeOutFile.close();
  openingOutFile.close(); 
  closingOutFile.close(); 
  prettyPrintFile.close();
  return 0;
}