# Limit Plots

This repository contains macros run by ROOT that will construct MOEDAL Style limit plots.

Each one of these macros are used for specific cases and particles including: dyons, highly electrically charged particles (HECO), and monopoles. Additionally, these macros require the Cross_Section_Limits.csv and Scaled_Cross_Section.csv files which must contain: mass (Gev), magnetic charge (gD), electric charge (e), spin, and cross section limit (fb) in the first 5 columns of the csv file in that order. 

The macros will produce plots with the information given by the csv files, and within the macro there is a option to plot only the lines that gives a mass limit (two corrosponding lines that intersect). Additionally, the macro will also output a csv file containing all the mass limit intersection informations. It should be noted that, the mass limit information is obtained from linear interpolation. 
