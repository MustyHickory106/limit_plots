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
  
  string Intersection_File_Name = "Intersection.csv"; //Declare the name of the output file which contains the limit plot intersection points
  
  //Values
  float sqrt_s = 13; //Center of Mass Energy (TeV)
  float fb_inv = 6.64; //fb^{-1]

  //Plot
  string Only_Plot_Intersection = "y"; //Do you only want to plot the curves with a mass limit? y/n
  
  //End of Manual Enter





  
  //Vectors used for the csv output of intersection
  vector<double> spin_out, gD_out, e_out, x_out, y_out;

  ofstream out(Intersection_File_Name.c_str()); //CSV File Name Declare
  out<<"Spin"<< "," << "gD" << "," << "e" << "," <<  "x" << "," << "y" <<"\n"; //row 1 title lables - comma seperated
  
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

  int total_cross = 0; //variables used for intersection analysis
  int total_cross_unique = 0; //variables used for intersection analysis
  int mass_limits = 0; //variable used to count how many mass limits exist within the 2 csv file

    
  //Specific Markers that will be used for the plot - 6 in total
  vector<int> markerlist = {8,21,22,23,29,33};

  //looking for unique values in spin vector - used to determine what are the possible spins that exist in the csv files
  vector<double> possible_spin, possible_spin_scaled;

  //Next 2 FOR loops will scroll through the data and pick out the different kinds of spin values that exist
  for (int i = 0; i < row_total; i++)
    {
      if( std::find(possible_spin.begin(), possible_spin.end(), spin[i]) == possible_spin.end() ) //true if not already seen
	{
	  possible_spin.push_back(spin[i]);
	    }
    }

  for (int i = 0; i < row_total_scaled; i++)
    {
      if( std::find(possible_spin_scaled.begin(), possible_spin_scaled.end(), spin_scaled[i]) == possible_spin_scaled.end() ) //true if not already seen
	{
	  possible_spin_scaled.push_back(spin_scaled[i]);
	    }
    }
  //The spins values that exist between the two files should be the SAME, if not this error message will show!
  if (possible_spin.size() != possible_spin_scaled.size())
    cout << "SPIN VALUES BETWEEN TWO FILES DO NOT MATCH UP, DOUBLE CHECK!!!" << endl;
  //sort into acsending order... for no particular reason, just easier to debug
  sort(possible_spin.begin(), possible_spin.end());

  //Center of Mass Energy and fb_inv Title Location (Change the 4 values if you need to move it!)
  auto title1 = new TLegend(0.49,0.85,0.75,0.92); //(xmin, ymin, xmax, ymax) when moving, try to keep the intervals between min and max the same so the size of the title stays constant
  
  ostringstream title1label;
  title1label << "#sqrt{s} = " << sqrt_s << " TeV, " << fb_inv << " fb^{-1}";
  title1 -> SetHeader(title1label.str().c_str(),"C");
  title1 -> SetBorderSize(0);
  title1 -> SetMargin(0);

  //"MoEDAL" title (Change the 4 values if you need to move it!)
  auto title2 = new TLegend(0.49,0.79,0.75,0.85); //(xmin, ymin, xmax, ymax) when moving, try to keep the intervals between min and max the same so the size of the title stays constant

  title2 -> SetHeader("MoEDAL","C");
  title2 -> SetBorderSize(0);
  title2 -> SetMargin(0);
  
  
  //START OF THE PLOTTING ALGORITHM, It will cycle thru each invidicual values creating different permuations of spin, magnetic charge, and electric charge.
  for (int s = 0; s < possible_spin.size(); s++)
    {
      
      //looking for unique values in gD vector with a specfic spin values  
      vector<double> possible_gD, possible_gD_scaled;
      for (int i = 0; i < row_total; i++)
	{
	  if( std::find(possible_gD.begin(), possible_gD.end(), gD[i]) == possible_gD.end() and spin[i] == possible_spin[s] ) //true if not already seen
	    {
	      possible_gD.push_back(gD[i]);
	    }
	}
      for (int i = 0; i < row_total_scaled; i++)
	{
	  if( std::find(possible_gD_scaled.begin(), possible_gD_scaled.end(), gD_scaled[i]) == possible_gD_scaled.end() and spin_scaled[i] == possible_spin[s] ) //true if not already seen
	    {
	      possible_gD_scaled.push_back(gD_scaled[i]);
	    }
	}
      //The magnetic charge values that exist between the two files should be the SAME, if not this error message will show!
      if (possible_gD.size() != possible_gD_scaled.size())
	cout << "MAGNETIC CHARGE VALUES BETWEEN TWO FILES DO NOT MATCH UP, DOUBLE CHECK!!!" << endl;
      sort(possible_gD.begin(), possible_gD.end());
      
      //cycling thru Charge gD for a specific spin value
      for (int z = 0; z < possible_gD.size(); z++)
	{

	  //Magnetic Charge title (Change the 4 values if you need to move it!)
	  auto chargelabel = new TLegend(0.85,0.85,0.94,0.92);//(xmin, ymin, xmax, ymax) when moving, try to keep the intervals between min and max the same so the size of the title stays constant

	  ostringstream chargelabellabel;
	  chargelabellabel <<  possible_gD[z] << " gD";
	  chargelabel -> SetHeader(chargelabellabel.str().c_str(),"C");
	  chargelabel -> SetBorderSize(0);
	  chargelabel -> SetMargin(0);
	  
	  //looking for unique values in e vector with a specific magnetic charge and spin value
	  vector<double> possible_e, possible_e_scaled;
	  for (int i = 0; i < row_total; i++)
	    {
	      if( std::find(possible_e.begin(), possible_e.end(), e[i]) == possible_e.end() and gD[i] == possible_gD[z] and spin[i] == possible_spin[s] ) //true if not already seen
		{
		  possible_e.push_back(e[i]);
		}
	    }
	  sort(possible_e.begin(),possible_e.end()); //into order
	  for (int i = 0; i < row_total_scaled; i++)
	    {
	      if( std::find(possible_e_scaled.begin(), possible_e_scaled.end(), e_scaled[i]) == possible_e_scaled.end() and gD_scaled[i] == possible_gD[z] and spin_scaled[i] == possible_spin[s] ) 
		{
		  possible_e_scaled.push_back(e_scaled[i]);
		}
	    }
	  sort(possible_e_scaled.begin(),possible_e_scaled.end()); // into order
	  //The electric charge values that exist between the two files should be the SAME, if not this error message will show!
	  if (possible_e.size() != possible_e_scaled.size())
	    cout << "ELECTRIC CHARGE VALUES BETWEEN TWO FILES DO NOT MATCH UP, DOUBLE CHECK!!!" << endl;
	  
	  //declare tuple for storing intersection information
	  vector<tuple<int,string,string>> intersection_info;

	  //Algorithm for finding intersection between Cross Section and Scaled Cross Section lines
	  vector <double> e_intersect;
	  for (int g = 0; g < possible_e.size(); g++)
	    {
	      //first the algorithm will sort and isolate the data for each electric charge Cross Section line and its corrosponding Scaled Cross Section Line
	      vector<pair<double,double> > cross, scaled;
	      for (int h = 0; h < row_total; h++)
		{
		  if (gD[h] == possible_gD[z] and e[h] == possible_e[g] and spin[h] == possible_spin[s])
		    {
		      cross.push_back( make_pair(GeV[h], fb[h]));
		    }
		}
	      for (int h = 0; h < row_total_scaled; h++)
		{
		  if (gD_scaled[h] == possible_gD[z] and e_scaled[h] == possible_e[g] and spin_scaled[h] == possible_spin[s])
		    {
		      scaled.push_back( make_pair(GeV_scaled[h], fb_scaled[h]));
		    }
		}
	      sort(cross.begin(),cross.end());
	      sort(scaled.begin(),scaled.end());

	      //It Will take the 2 sorted curves and see if they intersect at any point
	      float intersection_cout = 0;
	      if (cross.size() > 1  and scaled.size() > 1)
		{
		  for (int i = 0 ; i<cross.size()-1; i++)
		    {
		      for (int j = 0 ; j<scaled.size()-1; j++)
			{
			  double m1 = (cross[i+1].second - cross[i].second)/ (cross[i+1].first - cross[i].first);
			  double m2 = (scaled[j+1].second - scaled[j].second)/ (scaled[j+1].first - scaled[j].first);
			  
			  double x = (scaled[j].second - cross[i].second - (m2*scaled[j].first) + (m1*cross[i].first)) / (m1-m2);
			  
			  if  (x >= (scaled[j].first) and x >= (cross[i].first) and x <= (cross[i+1].first) and x <= (scaled[j+1].first))
			    {
			      if (intersection_cout == 0){
				e_intersect.push_back(possible_e[g]);
				total_cross_unique += 1;
				float y = m1*(x-cross[i].first)+cross[i].second; //if it does intersection it calcuated the x and y values
				ostringstream x_str, y_str;
				x_str << x;
				y_str << y;
				intersection_info.push_back( make_tuple(possible_e[g], x_str.str(), y_str.str())); // and store the x and y values into this variable
			      }
			      else
				cout << "CROSSED MORE THAN ONCE" << endl;
			      
			      intersection_cout += 1.;
			      total_cross += 1;
			    }
			}
		    }
		}
	      
	    }
	  
	  
	  //IF YOU WANT TO PLOT ONLY THE INTERSECTION VALUES
	  if (Only_Plot_Intersection == "y")
	    possible_e = e_intersect; //Replaced with only Intersected Plots
	  /*
	  //MANUAL ADDITION OF PLOTS
	  //0spin 5gD 25e
	  if (possible_spin[s] == 0 and possible_gD[z] == 5)
	    {
	      possible_e.push_back(25);
	      intersection_info.push_back( make_tuple(25, "Manual", "Manual"));
	    }
	  //0spin 5gD 5e
	  if (possible_spin[s] == 0 and possible_gD[z] == 5)
	    {
	      possible_e.push_back(5);
	      intersection_info.push_back( make_tuple(5, "Manual", "Manual"));
	    }
	    //End of MANUAL ADDITION
	    */
	  
	  //Append lines into our csv file for intersection information
	  sort(intersection_info.begin(), intersection_info.end());
	  for (int k = 0; k < intersection_info.size(); k++)
	    {
	      out << possible_spin[s] << "," << possible_gD[z] << "," << get<0>(intersection_info[k]) << "," << get<1>(intersection_info[k]) << "," << get<2>(intersection_info[k]) << "\n";
	    }
	  
	  
	  //Counting how many intersections
	  mass_limits += e_intersect.size();
	  sort(possible_e.begin(), possible_e.end());

	  //Determine how many plots will be needed for each sub group of a given spin and gD
	  double e_plots_need = possible_e.size() / 6.0;
	  int tracker1 = 0;
	  int tracker2 = 6; // index 6 is the 6th
	  
	  //divide up e plots so its between 3-6 lines per plot.
	  int t = 0; // index for while loop
	  while (t < e_plots_need)
	    {
	      int line_check = possible_e.size() - tracker2; //the little chunk helps seperate the plots so its always 3-6 lines per graph
	      if (line_check == 1)
		tracker2 -= 2;
	      if (line_check == 2)
		tracker2 -=1;
	      
	      if (tracker2 > possible_e.size())
		tracker2 = possible_e.size();
	      
	      //Finding the larges and smallest values for a given plot for windowing
	      vector<double> values;
	      for (int i = tracker1; i<tracker2; i++)
		{
		  for (int j = 0; j < row_total; j++)
		    {
		      if (gD[j] == possible_gD[z] and e[j] == possible_e[i] and spin[j] == possible_spin[s])
			{
			  values.push_back(fb[j]);
			  
			}
		    }
		}
	      
	      //ymax and ymin variables with determine the y-axis window size.
	      double ymax_auto = *max_element(values.begin(),values.end())   *10; //variable max value
	      double ymin_auto = *min_element(values.begin(),values.end())    - 500; //variable min value
	      double ymin = 1; //constant mine value
	      
	      //DELCARE MOEDAL STYLE
	      TStyle *MoEDALStyle = new TStyle("MoEDAL", "MoEDAL style");
	      Int_t icol=0; // WHITE
	      MoEDALStyle->SetFrameBorderMode(icol);
	      MoEDALStyle->SetFrameFillColor(icol);
	      MoEDALStyle->SetCanvasBorderMode(icol);
	      MoEDALStyle->SetCanvasColor(icol);
	      MoEDALStyle->SetPadBorderMode(icol);
	      MoEDALStyle->SetPadColor(icol);
	      MoEDALStyle->SetStatColor(icol);
	      
	      // set the paper & margin sizes
	      MoEDALStyle->SetPaperSize(20,26);
	      
	      // set margin sizes
	      MoEDALStyle->SetPadTopMargin(0.05);
	      MoEDALStyle->SetPadRightMargin(0.05);
	      MoEDALStyle->SetPadBottomMargin(0.16);
	      MoEDALStyle->SetPadLeftMargin(0.16);
	      
	      // set title offsets (for axis label)
	      MoEDALStyle->SetTitleXOffset(1.4);
	      MoEDALStyle->SetTitleYOffset(1.4);
	      
	      // use large fonts
	      Int_t font=42; // Helvetica
	      Double_t tsize=0.059;
	      MoEDALStyle->SetTextFont(font);
	      
	      MoEDALStyle->SetTextSize(tsize);
	      MoEDALStyle->SetLabelFont(font,"x");
	      MoEDALStyle->SetTitleFont(font,"x");
	      MoEDALStyle->SetLabelFont(font,"y");
	      MoEDALStyle->SetTitleFont(font,"y");
	      MoEDALStyle->SetLabelFont(font,"z");
	      MoEDALStyle->SetTitleFont(font,"z");
	      
	      MoEDALStyle->SetLabelSize(tsize,"x");
	      MoEDALStyle->SetTitleSize(tsize,"x");
	      MoEDALStyle->SetLabelSize(tsize,"y");
	      MoEDALStyle->SetTitleSize(tsize,"y");
	      MoEDALStyle->SetLabelSize(tsize,"z");
	      MoEDALStyle->SetTitleSize(tsize,"z");
	      
	      // use bold lines and markers
	      MoEDALStyle->SetMarkerStyle(20);
	      MoEDALStyle->SetMarkerSize(1.2);
	      MoEDALStyle->SetHistLineWidth(2.);
	      MoEDALStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
	      
	      MoEDALStyle->SetEndErrorSize(0.);
	      
	      // do not display any of the standard histogram decorations
	      MoEDALStyle->SetOptTitle(1);
	      MoEDALStyle->SetTitleFillColor(0);
	      MoEDALStyle->SetTitleBorderSize(0);
	      MoEDALStyle->SetTitleFontSize(0.045);
	      MoEDALStyle->SetTitleX(0.16);
	      MoEDALStyle->SetTitleY(1.0);
	      MoEDALStyle->SetOptStat(0);
	      MoEDALStyle->SetOptFit(0);
	      
	      // put tick marks on top and RHS of plots
	      MoEDALStyle->SetPadTickX(1);
	      MoEDALStyle->SetPadTickY(1);
	      MoEDALStyle -> cd();

	      gROOT -> SetStyle("MoEDAL");
	      gROOT -> ForceStyle();
	      
	      
	      TCanvas* c1 = new TCanvas("c1", "c1" , 1200,700); //keep this canvas size constant so it matches other MoEDAL plots
	      c1 -> SetFillStyle(0);

	      //1st columnn legend settings.
	      auto legend1 = new TLegend(0.69,0.18,0.767,0.40); //(xmin, ymin, xmax, ymax) try to keep the intervals between min and max constant

	      legend1 -> SetHeader("95% CL Limits");
	      vector<TLegendEntry*> leg1{6};
	      
	      //load in image
	      TImage *img = TImage::Open("MoEDAL-logo.png");

	      //MoEDAL Logo location settings
	      TPad *l = new TPad("l","l", 0.18, 0.145, .29, 0.33);//(xmin, ymin, xmax, ymax) try to keep the intervals between min and max constant

	      int graphnumber = 0;
	      //plotting e's for given gD and spin
	      for (int i = tracker1; i<tracker2; i++)
		{
		  //first isolate the values you need -> then sort them so you can plot them by order along the x-axis
		  int point_number = 0;
		  vector<pair<double,double> > tempvec; //x values are not in order, this algorthim sorts it by x values of acsending
		  TGraph* g = new TGraph();
		  for (int j = 0; j<row_total; j++)
		    {
		      if (gD[j] == possible_gD[z] and e[j] == possible_e[i] and spin[j] == possible_spin[s])
			{ 
			  tempvec.push_back( make_pair(GeV[j],fb[j]) );
			  point_number += 1;

			  //manually removing points 
			  /*
			  if (gD[j] == 1 and spin[j] == 1 and e[j] == 150)
			    {
			      if (GeV[j] == 3050 or GeV[j] == 3100 or GeV[j] == 3150)
				{
				  tempvec.pop_back();
				  point_number -=1;
				}
			    }
			  */
			}
		    }
		  sort(tempvec.begin(),tempvec.end()); // sorting points by x values
		  //plotting the sorted values
		  for(int k = 0; k < point_number; k++)
		    {
		      g -> SetPoint(k,tempvec[k].first, tempvec[k].second);
		    }

		  //Changes the first graph to APL and rest into PL so all the plots can show ontop of each other
		  string draw_var = "PL";
		  if (graphnumber == 0){
		    draw_var = "APL";}
		  
		  g -> SetMarkerStyle(markerlist[graphnumber]); //choose from markerlist and cycle with %
		  g -> SetLineStyle(2);
		  g -> SetLineWidth(4);
		  g -> SetMarkerSize(2);
      
		  g -> GetXaxis() -> SetTitle("Mass [GeV]");
		  g -> GetYaxis() -> SetTitle("#sigma [fb]");
		  g -> GetYaxis() -> SetRangeUser(ymin,ymax_auto);
		  g -> GetXaxis() -> SetLimits(0 , 6300); //X WINDOW SETTINGS
		  
		  ostringstream label1;
		  label1 << (int)possible_e[i] << " e";
		  leg1[graphnumber] = legend1 -> AddEntry(g,label1.str().c_str());
		  
		  //Color Sorting
		  if (graphnumber == 0){
		    g -> SetLineColor(kBlack); 
		    g -> SetMarkerColor(kBlack);
		    leg1[graphnumber] -> SetTextColor(kBlack);} //change legend text to the same color as the plot
		  else if (graphnumber ==1){
		    g -> SetLineColor(kRed);
		    g -> SetMarkerColor(kRed);
		    leg1[graphnumber] -> SetTextColor(kRed);}
		  else if (graphnumber ==2){
		    g -> SetLineColor(kGreen+1);
		    g -> SetMarkerColor(kGreen+1);
		    leg1[graphnumber] -> SetTextColor(kGreen+1);}	   
		  else if (graphnumber ==3){
		    g -> SetLineColor(kBlue);
		    g -> SetMarkerColor(kBlue);
		    leg1[graphnumber] -> SetTextColor(kBlue);}	 
		  else if (graphnumber ==4){
		    g -> SetLineColor(kMagenta);
		    g -> SetMarkerColor(kMagenta);
		    leg1[graphnumber] -> SetTextColor(kMagenta);}	 
		  else if (graphnumber ==5){
		    g -> SetLineColor(kViolet+1);
		    g -> SetMarkerColor(kViolet+1);
		    leg1[graphnumber] -> SetTextColor(kViolet+1);}
	       
		  graphnumber += 1;
		  g -> Draw(draw_var.c_str());
		  
		}
	      legend1 -> SetBorderSize(0);
	      legend1 -> SetTextSize(0.03);
	      legend1 -> SetFillStyle(0); //make the background transparent
	      legend1 -> Draw();

	      //2nd column legend settings
	      auto legend2 = new TLegend(0.81, 0.18 , 0.89,0.40); //(xmin, ymin, xmax, ymax) try to keep interval consistent between max and min

	      ostringstream legend2label;
	      legend2label << "LO DY Spin " << possible_spin[s];
	      legend2-> SetHeader(legend2label.str().c_str());
	      
	      vector<TLegendEntry*> leg2{6};
	      
	      int graphnumber_scaled = 0;
	      //plotting e_scaled's for given gD and spin
	      for (int i = tracker1; i<tracker2; i++)
		{
		  int point_number_scaled = 0;
		  vector<pair<double,double> > tempvec_scaled; //x values are not in order, this algorthim sorts it by x values of acsending
		  TGraph* g = new TGraph();
		  for (int j = 0; j<row_total_scaled; j++)
		    {
		      if (gD_scaled[j] == possible_gD[z] and e_scaled[j] == possible_e[i] and spin_scaled[j] == possible_spin[s])
			{
			  tempvec_scaled.push_back( make_pair(GeV_scaled[j],fb_scaled[j]) );
			  point_number_scaled += 1;
			}
		    }
		  sort(tempvec_scaled.begin(),tempvec_scaled.end()); // sorting points by x values
		  for(int k = 0; k < point_number_scaled; k++)
		    {
		      g -> SetPoint(k,tempvec_scaled[k].first, tempvec_scaled[k].second);
		    }  
		  
		  g -> SetMarkerSize(0);
		  g -> SetLineWidth(2);
		  	  
		  ostringstream label2;
		  label2 << (int)possible_e[i] << " e";
		  leg2[graphnumber_scaled] = legend2 -> AddEntry(g,label2.str().c_str());
		  
		  //Color Sorting
		  if (graphnumber_scaled == 0){
		    g -> SetLineColor(kBlack);
		    g -> SetMarkerColor(kBlack);
		    leg2[graphnumber_scaled] -> SetTextColor(kBlack);}
		  else if (graphnumber_scaled ==1){
		    g -> SetLineColor(kRed);
		    g -> SetMarkerColor(kRed);
		    leg2[graphnumber_scaled] -> SetTextColor(kRed);}
		  else if (graphnumber_scaled ==2){
		    g -> SetLineColor(kGreen+1);
		    g -> SetMarkerColor(kGreen+1);
		    leg2[graphnumber_scaled] -> SetTextColor(kGreen+1);}	   
		  else if (graphnumber_scaled ==3){
		    g -> SetLineColor(kBlue);
		    g -> SetMarkerColor(kBlue);
		    leg2[graphnumber_scaled] -> SetTextColor(kBlue);}	 
		  else if (graphnumber_scaled ==4){
		    g -> SetLineColor(kMagenta);
		    g -> SetMarkerColor(kMagenta);
		    leg2[graphnumber_scaled] -> SetTextColor(kMagenta);}	 
		  else if (graphnumber_scaled ==5){
		    g -> SetLineColor(kViolet+1);
		    g -> SetMarkerColor(kViolet+1);
		    leg2[graphnumber_scaled] -> SetTextColor(kViolet+1);}
		  
		  graphnumber_scaled += 1;
		  g -> Draw("C");
		  
		}
	      legend2 -> SetBorderSize(0);
	      legend2 -> SetTextSize(0.03);
	      legend2 -> SetFillStyle(0);
	      legend2 -> Draw();
	      
	      gStyle -> SetLegendFont(42); //Set the title and charge label to a not BOLD text.
	      chargelabel -> SetFillStyle(0);
	      chargelabel -> SetTextSize(0.05);
	      chargelabel -> Draw();
	      
	      title1 -> SetFillStyle(0);
	      title1 -> SetTextSize(0.05);
	      title1 -> Draw();
	      
	      title2 -> SetFillStyle(0);
	      title2 -> SetTextSize(0.05);
	      title2 -> Draw();
	      
	      c1->SetLogy();
	      
	      l -> SetFillStyle(4000);
	      l -> Draw();
	      l -> cd();
	      img ->Draw();

	      //Saving the plots, naming formate = spin_magnetic charge_lowest-highest electric charge
	      ostringstream plotname;
	      plotname << possible_spin[s] << "_" << (int)possible_gD[z] << "_" << possible_e[tracker1] << "e-" << possible_e[tracker2-1] << "e.png";
	      c1 ->SaveAs(plotname.str().c_str());
	      
	      t += 1;
	      
	      tracker1 = tracker2;
	      tracker2 = tracker1 + 6;
	      
	      
	    }
	  
	}
      
    }
  //Number of intersections 
  cout << "Number of Mass Limits: " << mass_limits << endl;
  out.close();
  
  
}

 

  
