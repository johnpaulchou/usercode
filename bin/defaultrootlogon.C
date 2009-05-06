void defaultrootlogon()
{
  std::cout << "Loading CMS-specific Libraries." << std::endl;
  gSystem->Load("libFWCoreFWLite.so");
//  gSystem->Load("../lib/slc4_ia32_gcc345/libDataFormatsMETReco.so");
  AutoLibraryLoader::enable();

  const float FONTSIZE = 0.06;
  const int NCOL = 256;
  
  gROOT->SetStyle("Plain");
  gStyle->SetLabelFont(132);
  gStyle->SetLabelFont(12,"y");
  gStyle->SetLabelFont(12,"x");
  gStyle->SetLabelFont(12,"z");
  gStyle->SetLabelSize(FONTSIZE) ;
  gStyle->SetLabelSize(FONTSIZE,"y") ;
  gStyle->SetLabelSize(FONTSIZE,"x") ;
  gStyle->SetLabelSize(FONTSIZE,"z") ;
  gStyle->SetNdivisions(505,"x") ;
  gStyle->SetNdivisions(505,"y") ;
  gStyle->SetNdivisions(505,"z") ;
  gStyle->SetLabelOffset(0.010,"y") ;
  gStyle->SetLabelOffset(0.010,"x") ;
  gStyle->SetLabelOffset(0.005,"z") ;
  gStyle->SetTextFont(132);
  gStyle->SetTextSize(FONTSIZE);
  gStyle->SetTitleFont(132,"xyz");
  gStyle->SetTitleFont( 42,"bla" );
  gStyle->SetTitleFontSize(12);
  gStyle->SetTitleX(FONTSIZE);
  gStyle->SetTitleW(0.9);
  gStyle->SetTitleH(0.08);
  //gStyle->SetTitleOffset(0.95,"y") ;
  gStyle->SetTitleOffset(1.3,"y") ;
  gStyle->SetTitleOffset(1.25,"x") ;
  gStyle->SetTitleXSize(FONTSIZE);
  gStyle->SetTitleYSize(FONTSIZE);
  gStyle->SetTitleSize(0.10);
  gStyle->SetTitleSize(FONTSIZE,"x");
  gStyle->SetTitleSize(FONTSIZE,"y");
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleColor(1);
  gStyle->SetPadBottomMargin(0.18) ;
  gStyle->SetPadTopMargin(0.13) ;
  gStyle->SetPadLeftMargin(0.15) ;
  gStyle->SetPadRightMargin(0.12) ;
  //gStyle->SetPadTickX( 1 );
  gStyle->SetPadTickY( 1 );
  //  gStyle->SetErrorMarker( 8 );
  gStyle->SetPaperSize(TStyle::kUSLetter) ;
  
  gStyle->SetCanvasBorderSize(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0);
  
  gStyle->SetPadBorderMode(0) ;
  gStyle->SetPadColor(0);
  gStyle->SetFrameLineWidth(0);
  //gStyle->SetFrameFillStyle(4001);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(0);
  // gStyle->SetFitFormat("6.4f");
  // gStyle->SetStatFormat("6.4f");
  gStyle->SetStatFont(132) ;
  gStyle->SetStatX(0.87) ; 
  gStyle->SetStatY(0.87) ; 
  gStyle->SetStatW(0.28);
  gStyle->SetStatH(0.30);
  gStyle->SetStatBorderSize(0) ;
  gStyle->SetStatColor(0) ;
  gStyle->SetStatStyle(0);
  //gStyle->SetStatStyle(4001);
  gStyle->SetTextAlign(13);
  gStyle->SetHistFillColor(5);
  gStyle->SetHistLineWidth(2);
  gStyle->SetLineWidth(2);
  gStyle->SetFrameLineWidth(2);
  gStyle->SetFuncWidth(2);
  gStyle->SetDrawBorder(2);
  
  // create color map
  /*  int colors[ NCOL ] = { 0 };
  for( int i = 0; i < NCOL; ++i ) {
    TColor* color = new TColor( 501+i,
				1 - float(i)/NCOL,
				1 - float(i)/NCOL,
				1 - float(i)/NCOL );
    colors[i] = 501+i;
    }
    int colors[10] = {10,19,18,17,16,15,14,13,12,1};
    gStyle->SetPalette(NCOL,colors);*/
  
  //int colors[10] = {10,19,18,17,16,15,14,13,12,1};
  //gStyle->SetPalette(10,colors);
  gStyle->SetPalette( 1 );
  
  gStyle->SetMarkerStyle(1);
  gStyle->SetMarkerSize(0.5);
  gROOT->ForceStyle();
}
