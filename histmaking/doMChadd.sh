#!/bin/bash
#rm MChists_simple/histsMB.root
#hadd MChists_simple/histsMB.root MChists_simple/*MB*

rm MChists/histsJet5_unsmear.root
hadd MChists/histsJet5_unsmear.root MChists/*Jet5_*_unsmear*
rm MChists/histsJet10_unsmear.root
hadd MChists/histsJet10_unsmear.root MChists/*Jet10*_unsmear*
rm MChists/histsJet20_unsmear.root
hadd MChists/histsJet20_unsmear.root MChists/*Jet20*_unsmear*
rm MChists/histsJet30_unsmear.root
hadd MChists/histsJet30_unsmear.root MChists/*Jet30*_unsmear*
rm MChists/histsJet50_unsmear.root
hadd MChists/histsJet50_unsmear.root MChists/*Jet50*_unsmear*
rm MChists/histsJet70_unsmear.root
hadd MChists/histsJet70_unsmear.root MChists/*Jet70*_unsmear*


rm MChists/histsPhoton5_unsmear.root
hadd MChists/histsPhoton5_unsmear.root MChists/*Photon5*_unsmear*
rm MChists/histsPhoton10_unsmear.root
hadd MChists/histsPhoton10_unsmear.root MChists/*Photon10*_unsmear*
rm MChists/histsPhoton20_unsmear.root
hadd MChists/histsPhoton20_unsmear.root MChists/*Photon20*_unsmear*
