# Limit Plots

This repository contains macros run by ROOT that will construct MOEDAL Style limit plots.

Each one of these macros are used for specific cases and particles including: dyons, highly electrically charged particles (HECO), and monopoles. Additionally, these macros require the Cross_Section_Limits.csv and Scaled_Cross_Section.csv files which must contain: mass (Gev), magnetic charge (gD), electric charge (e), spin, and cross section limit (fb) in the first 5 columns of the csv file in that order. 

The macros will produce plots with the information given by the csv files, and within the macro there is a option to plot only the lines that gives a mass limit (two corrosponding lines that intersect). Additionally, the macro will also output a csv file containing all the mass limit intersection informations. It should be noted that, the mass limit information is obtained from linear interpolation. 

The inital chunk of code in each macro is dedicated to manual user inputs settings, which includes: name of the csv files it needs to read, name of the intersection csv file that will be outputted, Center of Mass Energy (TeV) values and inverse cross section (fb^{-1}) values that the experiment used. Additionally, there is a yes or no (y/n) option that will tell the macro to either plot all the data points or only the data points that result in a mass limit intersection. Aside from the inital chunk, there is a few settings which the user should be aware of that dictates the position of all the legends and labels shown on the plot and the x and y axis range. These parameters will need to be manipulated from time to time to move the labels away from obstructing the actual plot it self. The Center of Mass Energy and Cross Section value labels are initalized as the "title1" variable and instruction to changing its position can be found in the comments near the actual line of code where this variable pointer is initalized. Similarly, the MoEDAL title, MoEDAL logo, left side legend, right side legend, and the upper right charge label (if applicable) are initalized as "title2", "l", "legend1", "legend2", and "chargelabel" respectively. Changing the text in these labels are pretty self-explainatory if the user is familiar with ROOT and know which variable pointers are assigned to the object they are trying to edit. Additionally, near the initialization of these variables, there is commented instructions to help with any changes the user needs to make. To change the x and y ranges in the plots, the user must find the line "plot -> GetYaxis() -> SetRangeUser(a,b);" or "plot -> GetXaxis() -> SetLimits(c,d);". These 2 lines would dictate the range of the axis of your plot. a,b,c,d can be set to specific values, but for the y-axis, there is an option to automatically adjust the range based on the data point values. To use this automatic range setting, set a and b as "ymin_auto", "ymax_auto" variables (if applicable). 

To run these macros, there must be 2 csv files in the same directory containing the Cross Section Limit and Scaled Cross Section information. The first columns of the csv files must be mass (GeV), magnetic charge (g_{D}), electric charge (e), spin, and cross-section limit (fb) respectively. Additionally, the MoEDAL logo image named "MoEDAL-logo.png" must also be present in the same directory. After running the macro, there will be a set of plots produced using the csv file information and the naming convention of the plots are defined in the last little chunck of each macro. For example, plotting dyons would create plots with the naming convetion "spin_magnetic charge_lowest-highest electric charge.png", where spin, magnetic charge, lowest electric charge, and highest electric charge are numerical values. Also, a csv file will be produced containing the (x,y) position of all the mass limit intersections found within the data. Again, these intersections are determined using linear interpolation methods.

This is my first time at creating ROOT macros, so the code it self might not be straight-forward nor streamlined. If there is any questions regarding these set of macros please e-mail me at zhigu@ualberta.ca and I'd be happy to answer them.
