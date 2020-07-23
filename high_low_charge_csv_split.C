#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include "TROOT.h"
#include "TStyle.h"

{
  //Manual Enter:
  
  //FILENAMES
  string Cross_Section_Limits = "Cross_Section_Limits.csv"; //Declare Cross Section Limits file name that will be used for plots
  string Scaled_Cross_Sections = "Scaled_Cross_Sections.csv"; // Declare Scaled Cross Sections file name that will be used for plots
  
  string low_charge_cross_csv = "Cross_Section_Limits_lowcharges.csv"; //Declare the name of the output file which contains the limit plot intersection points
  string low_charge_scaled_csv = "Scaled_Cross_Sections_lowcharges.csv"; //Declare the name of the output file which contains the limit plot intersection points
  
  string high_charge_cross_csv = "Cross_Section_Limits_highcharges.csv"; //Declare the name of the output file which contains the limit plot intersection points
  string high_charge_scaled_csv = "Scaled_Cross_Sections_highcharges.csv"; //Declare the name of the output file which contains the limit plot intersection points

  //End of Manual Enter





  


  ofstream out_cross_low(low_charge_cross_csv.c_str()); //CSV File Name Declare
  out_cross_low<<"Mass / GeV"<< "," << "Magnetic Charge / gD" << "," << "Electric Charge / e" << "," <<  "Spin" << "," << "Cross-Section Limit / fb" <<"\n"; //row 1 title lables - comma seperated

  ofstream out_scaled_low(low_charge_scaled_csv.c_str()); //CSV File Name Declare
  out_scaled_low<<"Mass / GeV"<< "," << "Magnetic Charge / gD" << "," << "Electric Charge / e" << "," <<  "Spin" << "," << "Scaled Cross-Section / fb" <<"\n"; //row 1 title lables - comma seperated

  ofstream out_cross_high(high_charge_cross_csv.c_str()); //CSV File Name Declare
  out_cross_high<<"Mass / GeV"<< "," << "Magnetic Charge / gD" << "," << "Electric Charge / e" << "," <<  "Spin" << "," << "Cross-Section Limit / fb" <<"\n"; //row 1 title lables - comma seperated

  ofstream out_scaled_high(high_charge_scaled_csv.c_str()); //CSV File Name Declare
  out_scaled_high<<"Mass / GeV"<< "," << "Magnetic Charge / gD" << "," << "Electric Charge / e" << "," <<  "Spin" << "," << "Scaled Cross-Section / fb" <<"\n"; //row 1 title lables - comma seperated


  //Variables that will store our CSV file data
  vector<double> GeV, gD, e, spin, fb, GeV_scaled, gD_scaled, e_scaled, spin_scaled, fb_scaled;

  //Temporary Variables used to help store csv file data
  string col0, col1, col2, col3, col4, line; //our csv file format is 5 columns per each row.

  //Load in the Cross Section Limits File
  ifstream myFile(Cross_Section_Limits.c_str());
  
  getline(myFile,line); //discard first row

  int row_total = 0;  //variable used to count rows


  //Begin reading sequence
  while (!myFile.eof()) //careful, eof for some reason reads 1 extra column after the file has ended, the if statement is used to correct that.
    {
      getline(myFile,col0,',');
      if (col0 == ""){
	break;} //prevent eof from reading one extra line at the end
      GeV.push_back(stof(col0));

      getline(myFile,col1,',');
      gD.push_back(stof(col1));

      getline(myFile,col2,',');
      e.push_back(stof(col2));

      getline(myFile,col3,',');
      spin.push_back(stof(col3));

      getline(myFile,col4,'\n');
      fb.push_back(stof(col4));

      row_total += 1; //add one count to lines everytime it scrolls through all the columns of 1 line
    }

  //Load in Scaled Cross Section File
  ifstream myFile_scaled(Scaled_Cross_Sections.c_str());

  getline(myFile_scaled,line);  //discard first row

  int row_total_scaled = 0;  //count rows

  //Begin reading sequence
  while (!myFile_scaled.eof()) //again careful with eof
    {
      getline(myFile_scaled,col0,',');
      if (col0 == ""){
	break;} //prevent eof from reading one extra line at the end
      GeV_scaled.push_back(stof(col0));

      getline(myFile_scaled,col1,',');
      gD_scaled.push_back(stof(col1));

      getline(myFile_scaled,col2,',');
      e_scaled.push_back(stof(col2));

      getline(myFile_scaled,col3,',');
      spin_scaled.push_back(stof(col3));

      getline(myFile_scaled,col4,'\n');
      fb_scaled.push_back(stof(col4));

      row_total_scaled += 1; //add one count to lines everytime it scrolls through all the columns of 1 line
    }


  //Begin reading the value and sorting them based on the following:

  for(int i = 0; i < row_total; i++)
    {
      if (e[i] >= 165) //Cross_Section_Limit.csv file, pick out anything higher than 165e and place it into high charges file
	out_cross_high << GeV[i] << "," << gD[i] << "," << e[i] << "," << spin[i] << "," << fb[i] << "\n";
      else if (e[i] <= 15) //Cross_Section_Limit.csv file, pick out anything lower than 15e and place it into the low charges file
	out_cross_low << GeV[i] << "," << gD[i] << "," << e[i] << "," << spin[i] << "," << fb[i] << "\n";
    }

  for(int i = 0; i < row_total_scaled; i++)
    {
      if (e_scaled[i] >= 165) //Scaled_Cross_Section.csv file, pick out anything higher than 165e and plce it into higher charges file, MAKE SURE ITS THE SAME AS THE FIRST FOR LOOP (e.g 165e)
	out_scaled_high << GeV_scaled[i] << "," << gD_scaled[i] << "," << e_scaled[i] << "," << spin_scaled[i] << "," << fb_scaled[i] << "\n";
      else if (e_scaled[i] <=15) //Scaled_Cross_Section.csv file, pick out anything lower than 15e and plce it into lower charges file, MAKE SURE ITS THE SAME AS THE FIRST FOR LOOP (e.g 15e)
	out_scaled_low << GeV_scaled[i] << "," << gD_scaled[i] << "," << e_scaled[i] << "," << spin_scaled[i] << "," << fb_scaled[i] << "\n";
    }
  

  out_cross_low.close();
  out_scaled_low.close();
  out_cross_high.close();
  out_scaled_high.close();

}
