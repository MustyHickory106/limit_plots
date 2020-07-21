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
  string Cross_Section_Limits = "Cross_Section_Limits_highcharges.csv"; //Declare Cross Section Limits file name that will be used for plots
  string Scaled_Cross_Sections = "Scaled_Cross_Sections_highcharges.csv"; // Declare Scaled Cross Sections file name that will be used for plots
  
  string Intersection_File_Name = "Intersection_highcharges.csv"; //Declare the name of the output file which contains the limit plot intersection points
  
  //Values
  float sqrt_s = 13; //Center of Mass Energy (TeV)
  float fb_inv = 6.64; //fb^{-1]

  //Plot
  string Only_Plot_Intersection = "y"; //Do you only want to plot the curves with a mass limit? y/n
  
  //End of Manual Enter






  
  //Vectors for the csv output of intersection
  vector<double> spin_out, gD_out, e_out, x_out, y_out;

  ofstream out(Intersection_File_Name.c_str()); //CSV File Name Declare
  out<<"Spin"<< "," << "gD" << "," << "e" << "," <<  "x" << "," << "y" <<"\n"; //row 1 title lables - comma seperated
  
  //Variables that will store our CSV file data
  vector<double> GeV, gD, e, spin, fb, GeV_scaled, gD_scaled, e_scaled, spin_scaled, fb_scaled;

  //Temporary Variables used to help store csv file data
  string col0, col1, col2, col3, col4, line;//our csv file format is 5 columns per each row.


  //Load in the Cross Section Limits File
  ifstream myFile(Cross_Section_Limits.c_str());
  
  getline(myFile,line);  //discard first row

  int row_total = 0; //variable used to count rows

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

  int row_total_scaled = 0; //count rows

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
  
  //Specific Markers that will be used for the plot - 6 in total
  vector<int> markerlist = {8,21,22,23,29,33};

  //Next 2 FOR loops will scroll through the data and pick out the different kinds of spin values that exist
  vector<double> possible_spin, possible_spin_scaled;
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

  sort(possible_spin.begin(), possible_spin.end());

  
  //Next 2 FOR loops will scroll through the data and pick out the different kinds of magnetic charge values that exist
  vector<double> possible_gD, possible_gD_scaled;
  for (int i = 0; i < row_total; i++)
    {
      if( std::find(possible_gD.begin(), possible_gD.end(), gD[i]) == possible_gD.end()) //true if not already seen
	{
	  possible_gD.push_back(gD[i]);
	}
    }
  for (int i = 0; i < row_total_scaled; i++)
    {
      if( std::find(possible_gD_scaled.begin(), possible_gD_scaled.end(), gD_scaled[i]) == possible_gD_scaled.end() ) //true if not already seen
	{
	  possible_gD_scaled.push_back(gD_scaled[i]);
	}
    }
  //The magnetic charge values that exist between the two files should be the SAME, if not this error message will show!
  if (possible_gD.size() != possible_gD_scaled.size())
    cout << "MAGNETIC CHARGE VALUES BETWEEN TWO FILES DO NOT MATCH UP, DOUBLE CHECK!!!" << endl;
  sort(possible_gD.begin(), possible_gD.end());

  //Next 2 FOR loops will scroll through the data and pick out the different kinds of electric charge values that exist
  vector<double> possible_e, possible_e_scaled;
  for (int i = 0; i < row_total; i++)
    {
      if( std::find(possible_e.begin(), possible_e.end(), e[i]) == possible_e.end() ) //true if not already seen
	{
	  possible_e.push_back(e[i]);
	}
    }
  sort(possible_e.begin(),possible_e.end()); //into order
  for (int i = 0; i < row_total_scaled; i++)
    {
      if( std::find(possible_e_scaled.begin(), possible_e_scaled.end(), e_scaled[i]) == possible_e_scaled.end() ) 
	{
	     possible_e_scaled.push_back(e_scaled[i]);
	   }
    }
  //The Electric charge values that exist between the two files should be the SAME, if not this error message will show!
  if (possible_e.size() != possible_e_scaled.size())
    cout << "ELECTRIC CHARGE VALUES BETWEEN TWO FILES DO NOT MATCH UP, DOUBLE CHECK!!!" << endl;
  sort(possible_e_scaled.begin(),possible_e_scaled.end()); // into order


  //Algorithm for finding intersection between Cross Section and Scaled Cross Section lines
  vector<double> intersect_e, intersect_spin, intersect_gD;
  vector<tuple<string,string,string,string,string>> intersection_info;
  vector<tuple<float,int,int>> intersection_plot;
  for (int s = 0; s < possible_spin.size(); s++)
    {
      for (int g = 0 ; g < possible_gD.size(); g++)
	{
	  for (int c = 0; c < possible_e.size(); c++)
	    {
	      
	      vector<pair<double,double>> cross, scaled;
	      //first the algorithm will sort and isolate the data for each electric charge Cross Section line and its corrosponding Scaled Cross Section Line
	      for (int h = 0; h < row_total; h++)
		if (spin[h] == possible_spin[s] and gD[h] == possible_gD[g] and e[h] == possible_e[c])
		  cross.push_back( make_pair(GeV[h] , fb[h]) );

	      for (int h = 0; h < row_total_scaled; h++)
		if (spin_scaled[h] == possible_spin[s] and gD_scaled[h] == possible_gD[g] and e_scaled[h] == possible_e[c])
		  scaled.push_back( make_pair( GeV_scaled[h] , fb_scaled[h]) );

	      //It Will take the 2 sorted curves and see if they intersect at any point
	      if (scaled.size() > 1 and cross.size() > 1)
		{
		  sort(cross.begin(),cross.end());
		  sort(scaled.begin(),scaled.end());

		  int intersection_cout = 0;
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
				float y = m1*(x-cross[i].first)+cross[i].second;
				ostringstream spin_str,gD_str,e_str,x_str, y_str;
				spin_str << possible_spin[s];
				gD_str << possible_gD[g];
				e_str << possible_e[c];
				x_str << x;
				y_str << y;
				intersection_info.push_back( make_tuple(spin_str.str(),gD_str.str(),e_str.str(), x_str.str(), y_str.str()));
				intersection_plot.push_back( make_tuple(possible_spin[s],possible_gD[g],possible_e[c]) );
			      }
			      else
				cout << "CROSSED MORE THAN ONCE" << endl;
			      
			      intersection_cout += 1.;
			    }
			  
			}
		    }
		  
		}
	    }
	}
    }
  
  for(int i = 0; i < intersection_info.size(); i++)
    out << get<0>(intersection_info[i]) << "," << get<1>(intersection_info[i]) << "," << get<2>(intersection_info[i]) << "," <<  get<3>(intersection_info[i]) << "," << get<4>(intersection_info[i]) << "\n";

  out.close();
 
  //Center of Mass Energy and fb_inv Title Location (Change the 4 values if you need to move it!)
  auto title1 = new TLegend(0.41,0.85,0.67,0.92); //(xmin, ymin, xmax, ymax) when moving, try to keep the intervals between min and max the same so the size of the title stays constant

  ostringstream title1label;
  title1label << "#sqrt{s} = " << sqrt_s << " TeV, " << fb_inv << " fb^{-1}";
  title1 -> SetHeader(title1label.str().c_str(),"C");
  title1 -> SetBorderSize(0);
  title1 -> SetMargin(0);

  //"MoEDAL" title (Change the 4 values if you need to move it!)
  auto title2 = new TLegend(0.41,0.79,0.67,0.85); //(xmin, ymin, xmax, ymax) when moving, try to keep the intervals between min and max the same so the size of the title stays constant

  title2 -> SetHeader("MoEDAL","C");
  title2 -> SetBorderSize(0);
  title2 -> SetMargin(0);

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

  //1st columnn legend settings.
  auto legend1 = new TLegend(0.63,0.18,0.80,0.49); //(xmin, ymin, xmax, ymax) try to keep the intervals between min and max constant

  ostringstream legend1title;
  legend1title << "95% CL Limits, " << gD[0] << " g_{D}"; //CHANGE gD[0] if you manually change the Magnetic Charge Values
  legend1 -> SetHeader(legend1title.str().c_str()); 
  vector<TLegendEntry*> leg1{6};
  
  int graph_number = 0;
  TCanvas* c1 = new TCanvas("c1","c1", 1200,700);
  TImage *img = TImage::Open("MoEDAL-logo.png");
  
  //MoEDAL Logo location settings
  TPad *l = new TPad("l","l", 0.18, 0.145, .29, 0.33);//(xmin, ymin, xmax, ymax) try to keep the intervals between min and max constant
  
  for (int i = 0 ; i < intersection_plot.size() ; i++)
    {
      TGraph* plot  = new TGraph();
      vector<pair<double,double>> tempvec;
      int number_of_points = 0;
      //first isolate the values you need -> then sort them so you can plot them by order along the x-axis
      for (int h = 0; h < row_total; h++)
	{
	  if (e[h] == get<2>(intersection_plot[i]) and gD[h] == get<1>(intersection_plot[i]) and spin[h] == get<0>(intersection_plot[i]) )
	    {
	      tempvec.push_back( make_pair(GeV[h], fb[h]));
	      number_of_points += 1;
	      /*	      
	      //MANUAL REMOVAL OF POINTS
	      if(e[h] == 150 and gD[h] == gD[0] and spin[h] == 1){
		if (GeV[h] == 3100 or GeV[h] == 3150 or GeV[h] == 3050){
		  tempvec.pop_back();
		  number_of_points -=1;
		}}
	      */
	    }
	}
      sort(tempvec.begin(), tempvec.end() );
      //plotting the sorted values
      for (int h = 0; h < number_of_points; h++)
	{
	  plot -> SetPoint(h, tempvec[h].first, tempvec[h].second);
	}

      
      plot -> SetMarkerStyle(markerlist[graph_number]); //choose from markerlist and cycle with %
      plot -> SetLineStyle(2);
      plot -> SetLineWidth(4);
      plot -> SetMarkerSize(2);
      
      string draw_variable = "PL";
      if (graph_number == 0)
	draw_variable = "APL";
      
      plot -> GetXaxis() -> SetTitle("Mass [GeV]");
      plot -> GetYaxis() -> SetTitle("#sigma [fb]");
      plot -> GetYaxis() -> SetRangeUser(1,pow(10,5)); //Y WINDOW SETTINGS
      plot -> GetXaxis() -> SetLimits(-200,7200); //X WINDOW SETTINGS

      ostringstream label1;

      string spinlabel;
      if (get<0>(intersection_plot[i]) == 0.)
	spinlabel = "0";
      else if (get<0>(intersection_plot[i]) == 1.)
	spinlabel = "1";
      else if (get<0>(intersection_plot[i]) == 0.5)
	spinlabel = "1/2";
      label1 << get<2>(intersection_plot[i]) << " e, spin " << spinlabel;
      leg1[graph_number] = legend1 -> AddEntry(plot,label1.str().c_str());

      
      //Color Sorting
      if (graph_number == 0){
	plot -> SetLineColor(kBlack);
	plot -> SetMarkerColor(kBlack);
	leg1[graph_number] -> SetTextColor(kBlack); //change legend text to the same color as the plot
      }
      else if (graph_number == 1){
	plot -> SetLineColor(kRed);
	plot -> SetMarkerColor(kRed);
	leg1[graph_number] -> SetTextColor(kRed);
      }
      else if (graph_number == 2){
	plot -> SetLineColor(kGreen+1);
	plot -> SetMarkerColor(kGreen+1);
	leg1[graph_number] -> SetTextColor(kGreen+1);
	      }	   
      else if (graph_number == 3){
	plot -> SetLineColor(kBlue);
	plot -> SetMarkerColor(kBlue);
	leg1[graph_number] -> SetTextColor(kBlue);
      }	 
      else if (graph_number == 4){
	plot -> SetLineColor(kMagenta);
	plot -> SetMarkerColor(kMagenta);
	leg1[graph_number] -> SetTextColor(kMagenta);
      }	 
      else if (graph_number == 5){
	plot -> SetLineColor(kViolet+1);
	plot -> SetMarkerColor(kViolet+1);
	leg1[graph_number] -> SetTextColor(kViolet+1);
      }
      else if (graph_number == 6){
	plot -> SetLineColor(kCyan+1);
	plot -> SetMarkerColor(kCyan+1);
	leg1[graph_number] -> SetTextColor(kCyan+1);
      }
	      
      plot -> Draw(draw_variable.c_str() );
      graph_number += 1;
    }
  legend1 -> SetBorderSize(0);
  legend1 -> SetTextSize(0.03);
  legend1 -> SetFillStyle(0); //make the background transparent
  legend1 -> Draw();
  
  //2nd column legend settings
  auto legend2 = new TLegend(0.785, 0.18 , 0.905,0.49); //(xmin, ymin, xmax, ymax) try to keep interval consistent between max and min

  ostringstream legend2title;
  legend2title << "LO DY, " << gD[0] << " g_{D}";
  legend2-> SetHeader(legend2title.str().c_str()); //CHANGE MAGNETIC CHARGE IN THE LEGEND TITLE HERE
  vector<TLegendEntry*> leg2{6};
  
  int graph_number_scaled = 0;
  for (int i = 0 ; i < intersection_plot.size() ; i++)
    {
      TGraph* plot  = new TGraph();
      vector<pair<double,double>> tempvec;
      int number_of_points = 0;
      for (int h = 0; h < row_total_scaled; h++)
	{
	  if (e_scaled[h] == get<2>(intersection_plot[i])  and gD_scaled[h] == get<1>(intersection_plot[i]) and spin_scaled[h] == get<0>(intersection_plot[i]) )
	    {
	      tempvec.push_back( make_pair(GeV_scaled[h], fb_scaled[h]));
	      number_of_points += 1;
	    }
	}
      sort(tempvec.begin(), tempvec.end() );
      
      for (int h = 0; h < number_of_points; h++)
	{
	  plot -> SetPoint(h, tempvec[h].first, tempvec[h].second);
	}
      
      plot -> SetMarkerSize(0);
      plot -> SetLineWidth(2);
      
      plot -> GetXaxis() -> SetTitle("Mass [GeV]");
      plot -> GetYaxis() -> SetTitle("#sigma [fb]");

      
      ostringstream label2;
      string spinlabel;
      if (get<0>(intersection_plot[i]) == 0.)
	spinlabel = "0";
      else if (get<0>(intersection_plot[i]) == 1.)
	spinlabel = "1";
      else if (get<0>(intersection_plot[i]) == 0.5)
	spinlabel = "1/2";
      label2 << get<2>(intersection_plot[i]) << " e, spin " << spinlabel;
      leg2[graph_number_scaled] = legend2 -> AddEntry(plot,label2.str().c_str());

      //Color Sorting
      if (graph_number_scaled == 0){
	plot -> SetLineColor(kBlack);
	plot -> SetMarkerColor(kBlack);
	leg2[graph_number_scaled] -> SetTextColor(kBlack);
      }
      else if (graph_number_scaled == 1){
	plot -> SetLineColor(kRed);
	plot -> SetMarkerColor(kRed);
	leg2[graph_number_scaled] -> SetTextColor(kRed);
      }
      else if (graph_number_scaled == 2){
	plot -> SetLineColor(kGreen+1);
	plot -> SetMarkerColor(kGreen+1);
	leg2[graph_number_scaled] -> SetTextColor(kGreen+1);
      }	   
      else if (graph_number_scaled == 3){
	plot -> SetLineColor(kBlue);
	plot -> SetMarkerColor(kBlue);
	leg2[graph_number_scaled] -> SetTextColor(kBlue);
      }	 
      else if (graph_number_scaled == 4){
	plot -> SetLineColor(kMagenta);
	plot -> SetMarkerColor(kMagenta);
	leg2[graph_number_scaled] -> SetTextColor(kMagenta);
      }	 
      else if (graph_number_scaled == 5){
	plot -> SetLineColor(kViolet+1);
	plot -> SetMarkerColor(kViolet+1);
	leg2[graph_number_scaled] -> SetTextColor(kViolet+1);
      }
      else if (graph_number_scaled == 6){
	plot -> SetLineColor(kCyan+1);
	plot -> SetMarkerColor(kCyan+1);
	leg2[graph_number_scaled] -> SetTextColor(kCyan+1);
      }
      
      plot -> Draw("PL");
      
      graph_number_scaled += 1;
    }
  
  legend2 -> SetBorderSize(0);
  legend2 -> SetTextSize(0.03);
  legend2 -> SetFillStyle(0);
  legend2 -> Draw();
  
  gStyle -> SetLegendFont(42); //Set the title and charge label to a not BOLD text.
  title1 -> SetFillStyle(0);
  title1 -> SetTextSize(0.05);
  title1 -> Draw();
  title2 -> SetFillStyle(0);
  title2 -> SetTextSize(0.05);
  title2 -> Draw();
  
  c1 -> SetLogy();
		      
  l -> SetFillStyle(4000);
  l -> Draw();
  l -> cd();
  img ->Draw();
  
  c1 ->SaveAs("limit_highcharges.png"); //File name of the plot

  cout << "Number of Mass Limits: " << intersection_plot.size() << endl;
}





	      
	  

	
    

