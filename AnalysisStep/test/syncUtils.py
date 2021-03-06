import os, sys, ctypes
from ctypes import *

def sign(x):
    if x > 0:
        return 1.
    elif x < 0:
        return -1.
    elif x == 0:
        return 0.
    else:
        return x

class Event:

    def __init__(self,run,lumi,event):

        self.run   = run
        self.lumi  = lumi
        self.event = event

    def printOut(self):

        line   = ""
        line  += str(int(self.run))
        line  += ":" + str(int(self.lumi))
        line  += ":" + str(int(self.event))
        
        return line

class KDs:

    def __init__(self,p0plus_VAJHU,p0minus_VAJHU,p0hplus_VAJHU,p1plus_VAJHU,p1_VAJHU,p2_VAJHU,p2qqb_VAJHU,bkg_VAMCFM,p0plus_m4l,bkg_m4l,Dgg10_VAMCFM,pvbf_VAJHU,phjj_VAJHU):

        self.p0plus_VAJHU  = p0plus_VAJHU
        self.p0minus_VAJHU = p0minus_VAJHU
        self.p0hplus_VAJHU = p0hplus_VAJHU
        self.p1plus_VAJHU  = p1plus_VAJHU 
        self.p1_VAJHU      = p1_VAJHU     
        self.p2_VAJHU      = p2_VAJHU     
        self.p2qqb_VAJHU   = p2qqb_VAJHU              
        self.bkg_VAMCFM    = bkg_VAMCFM
        self.p0plus_m4l    = p0plus_m4l        
        self.bkg_m4l       = bkg_m4l        
        self.Dgg10_VAMCFM  = Dgg10_VAMCFM
        self.pvbf_VAJHU    = pvbf_VAJHU
        self.phjj_VAJHU    = phjj_VAJHU
        self.D_bkg_kin     = -1.
        self.D_g4          = -1.
        self.Dbkg          = -1.
        self.Djet_VAJHU    = -1.
        self.KD_pseudo     = -1.
        self.KD_highdim    = -1.
        self.KD_vec        = -1.
        self.KD_psvec      = -1.
        self.KD_gggrav     = -1.
        self.KD_qqgrav     = -1.
        self.computeKDs()


    def computeKDs(self):

        self.D_bkg_kin  = self.p0plus_VAJHU/(self.p0plus_VAJHU + self.bkg_VAMCFM)    
        self.D_bkg      = self.p0plus_VAJHU*self.p0plus_m4l/(self.p0plus_VAJHU*self.p0plus_m4l+self.bkg_VAMCFM*self.bkg_m4l)
        self.D_g4       = self.p0plus_VAJHU/(self.p0plus_VAJHU + self.p0minus_VAJHU) # D_0-
        self.Djet_VAJHU = self.pvbf_VAJHU/(self.pvbf_VAJHU+self.phjj_VAJHU )         # D^VBF_HJJ
        self.KD_highdim = self.p0plus_VAJHU/(self.p0plus_VAJHU + self.p0hplus_VAJHU)
        self.KD_vec     = self.p0plus_VAJHU/(self.p0plus_VAJHU + self.p1plus_VAJHU) 	 
        self.KD_psvec   = self.p0plus_VAJHU/(self.p0plus_VAJHU + self.p1_VAJHU) 	 
        self.KD_gggrav  = self.p0plus_VAJHU/(self.p0plus_VAJHU + self.p2_VAJHU) 	 
        self.KD_qqgrav  = self.p0plus_VAJHU/(self.p0plus_VAJHU + self.p2qqb_VAJHU) 


class Candidate:

    def __init__(self,event,m,mZ1,mZ2,mErr,mErrCorr,m4lRefit,m4lRefitErr,pt,nExtraLep,jets30pt,jets30eta,jets30phi,jets30mass,njets30Btag,mjj,detajj,kds,weight):

        self.eventInfo   = event
        self.weight      = weight
        self.mass4l      = m
        self.mZ1         = mZ1
        self.mZ2         = mZ2
        self.massErrRaw  = mErr
        self.massErrCorr = mErrCorr
        self.m4lRefit    = m4lRefit
        self.m4lRefitErr = m4lRefitErr
        self.kds         = kds
        self.pt4l        = pt
        self.nExtraLep   = nExtraLep
        self.jets30pt    = jets30pt
        self.jets30eta   = jets30eta
        self.jets30phi   = jets30phi
        self.jets30mass  = jets30mass
        self.njets30     = len(jets30pt)
        self.njets30Btag = njets30Btag
        self.mjj         = mjj
        self.detajj      = detajj
        self.fishjj      = -1.
        self.isDiJet     = False
        self.jet1pt      = -1.
        self.jet2pt      = -1.
        self.fillJetInfo()
        self.category    = ctypes.CDLL('libZZAnalysisAnalysisStep.so').category(
            c_int(nExtraLep),
            c_float(self.pt4l),
            c_float(self.mass4l),
            c_int(self.njets30),
            c_int(self.njets30Btag),
            (ctypes.c_float * len(self.jets30pt  ))(*self.jets30pt  ),
            (ctypes.c_float * len(self.jets30eta ))(*self.jets30eta ),
            (ctypes.c_float * len(self.jets30phi ))(*self.jets30phi ),
            (ctypes.c_float * len(self.jets30mass))(*self.jets30mass),
            c_float(self.fishjj),
            )

    def fillJetInfo(self):
        
        if self.njets30==1:
            self.jet1pt = self.jets30pt[0]
            self.mjj = -1.
            self.detajj = -1.
        elif self.njets30>=2:
            self.jet1pt = self.jets30pt[0]            
            self.jet2pt = self.jets30pt[1]                   
            self.fishjj = 0.18*abs(self.detajj) + 1.92e-04*self.mjj
        else:
            self.mjj = -1.
            self.detajj = -1.
            

    def printOut(self):

        line = ""
        line  += "{0:.2f}".format(self.mass4l)
        line  += ":" + "{0:.2f}".format(self.mZ1)
        line  += ":" + "{0:.2f}".format(self.mZ2)
#        line  += ":" + "{0:.2f}".format(self.massErrRaw)
#        line  += ":" + "{0:.2f}".format(self.massErrCorr)
        line  += ":" + "{0:.3f}".format(self.kds.D_bkg_kin)
        line  += ":" + "{0:.3f}".format(self.kds.D_bkg)
        line  += ":" + "{0:.3f}".format(self.kds.Dgg10_VAMCFM)
        if self.njets30 >= 2 :
            line  += ":" + "{0:.3f}".format(self.kds.Djet_VAJHU)
        else :
            line  += ":-1.000"
        line  += ":" + "{0:.3f}".format(self.kds.D_g4)
#        line  += ":" + "{0:.2f}".format(self.pt4l)
        line  += ":" + "{0:d}".format(self.njets30)
        line  += ":" + "{0:.2f}".format(self.jet1pt)
        line  += ":" + "{0:.2f}".format(self.jet2pt)
#        line  += ":" + "{0:.2f}".format(self.mjj)
#        line  += ":" + "{0:.3f}".format(self.detajj)
#        line  += ":" + "{0:.3f}".format(self.fishjj)
#        line  += ":" + "{0:.3f}".format(self.kds.KD_highdim)
#        line  += ":" + "{0:.3f}".format(self.kds.KD_vec)
#        line  += ":" + "{0:.3f}".format(self.kds.KD_psvec)
#        line  += ":" + "{0:.3f}".format(self.kds.KD_gggrav)
#        line  += ":" + "{0:.3f}".format(self.kds.KD_qqgrav) 
        line  += ":" + "{0:d}".format(self.category)
        line  += ":" + "{0:.2f}".format(self.m4lRefit)
        line  += ":" + "{0:.2f}".format(self.m4lRefitErr)
        line  += ":" + "{0:.3f}".format(self.weight)

        return line
