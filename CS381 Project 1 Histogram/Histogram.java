import java.io.*;
import java.util.*;

class Histogram {
  public static void main(String[] args) {
    
    //Create a scanner to read 1 inputs and 2 outputs that the user enters
    Scanner input = new Scanner(System.in);

    //User inputs input file
    System.out.println("Input your input file:");
    String file = input.nextLine();
    System.out.println("File Name: " + file);

    //User inputs first output file
    System.out.println("Input your output file:");
    String outFile1 = input.nextLine();
    System.out.println("File Name: " + outFile1);

    //User inputs second output file
    System.out.println("Input your second output file:");
    String outFile2 = input.nextLine();
    System.out.println("File Name: " + outFile2);

    int numRows, numCols, minVal, maxVal;
    int[] count = new int[1000];
    int[] count1 = new int[1000];
    int[] count2 = new int[1000];

    computeHist(file, count);
    printHist(file, outFile1, count1);
    dispHist(file, outFile2, count2);

    input.close();
  }

  static void computeHist(String inFile, int[] count){
    try{
      Scanner infile = new Scanner(new File(inFile));
      int numRows, numCols, minVal, maxVal;
      numRows = infile.nextInt();
      numCols = infile.nextInt();
      minVal = infile.nextInt();
      maxVal = infile.nextInt();

      while(infile.hasNextInt()) {
        int value = infile.nextInt();
        count[value]++;
      }
      infile.close();
    }
    catch (FileNotFoundException e) {
      System.out.println("File Not Found Error");
    }    
  }

  static void printHist(String iFile, String outFile, int[] count1){
  //Print histogram in format value: amount
    try{
      BufferedWriter out = new BufferedWriter(new FileWriter(outFile));
      try{
        Scanner infile = new Scanner(new File(iFile));
        int numRows, numCols, minVal, maxVal;
        numRows = infile.nextInt();
        numCols = infile.nextInt();
        minVal = infile.nextInt();
        maxVal = infile.nextInt();

        out.write(numRows + " " + numCols + " " + minVal + " " + maxVal);
        out.newLine();

        while(infile.hasNextInt()) {
          int value = infile.nextInt();
          count1[value]++;
        }

        for(int i = minVal; i <= maxVal; i++) {
          if(count1[i] >= 0){
            out.write(i + ": " + count1[i]);
          }
          out.newLine();
        }
        infile.close();
      }
      catch (FileNotFoundException e) {
        System.out.println("File Not Found Error");
      }
      out.close();
    }
    catch (IOException err){
      System.out.println("File Not Found Error");
    }
  }

  static void dispHist(String iFile, String outFile, int[] count2){
    //Displays histogram in format value (count) : +
    try{
      BufferedWriter out = new BufferedWriter(new FileWriter(outFile));
      try{
        Scanner infile = new Scanner(new File(iFile));
        int numRows, numCols, minVal, maxVal;
        numRows = infile.nextInt();
        numCols = infile.nextInt();
        minVal = infile.nextInt();
        maxVal = infile.nextInt();

        while(infile.hasNextInt()) {
          int value = infile.nextInt();
          count2[value]++;
        }

        out.write(numRows + " " + numCols + " " + minVal + " " + maxVal);
        out.newLine();

        int counter = 0;

        for(int i = minVal; i <= maxVal; i++) {
          if(count2[i] >= 0){
            out.write(i + " (" + count2[i] + ") :");
          }
          for(int j = 0; j < count2[i]; j++){
            if(count2[i] < 60){
              out.write("+");
            }
            else {
              for(int k = 0; k < 60; k++){
                out.write("+");
              }
            }
          }
          out.newLine();
          counter = 0;
        }      
        infile.close();
      }
      catch (FileNotFoundException e) {
        System.out.println("File Not Found Error");
      }
      out.close();
    }
    catch (IOException err){
      System.out.println("File Not Found Error");
    }
  }
}