# Limit Plots

This repository contains macros run by ROOT that will construct MOEDAL Style limit plots.

Each one of these macros are used for specific cases and particles including: dyons, highly electrically charged particles (HECO), and monopoles. Additionally, these macros require the Cross_Section_Limits.csv and Scaled_Cross_Section.csv files which must contain: mass (Gev), magnetic charge (gD), electric charge (e), spin, and cross section limit (fb) in the first 5 columns of the csv file in that order. 

The macros will produce plots with the information given by the csv files, and within the macro there is a option to plot only the lines that gives a mass limit (two corrosponding lines that intersect). Additionally, the macro will also output a csv file containing all the mass limit intersection informations. It should be noted that, the mass limit information is obtained from linear interpolation. 

The inital chunk of code in each macro is dedicated to manual user inputs settings, which includes: name of the csv files it needs to read, name of the intersection csv file that will be outputted, Center of Mass Energy (TeV) values and inverse cross section (fb^{-1}) values that the experiment used. Additionally, there is a yes or no (y/n) option that will tell the macro to either plot all the data points or only the data points that result in a mass limit intersection. Aside from the inital chunk, there is a few settings which the user should be aware of that dictates the position of all the legends and labels shown on the plot and the x and y axis range. These parameters will need to be manipulated from time to time to move the labels away from obstructing the actual plot it self. The Center of Mass Energy and Cross Section value labels are initalized as the "title1" variable and instruction to changing its position can be found in the comments near the actual line of code where this variable pointer is initalized. Similarly, the MoEDAL title, MoEDAL logo, left side legend, right side legend, and the upper right charge label (if applicable) are initalized as "title2", "l", "legend1", "legend2", and "chargelabel" respectively. Changing the text in these labels are pretty self-explainatory if the user is familiar with ROOT and know which variable pointers are assigned to the object they are trying to edit. Additionally, near the initialization of these variables, there is commented instructions to help with any changes the user needs to make. 
