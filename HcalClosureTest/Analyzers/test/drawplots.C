TFile* rootfile=0;

TH1* h[20];

void drawplots(void)
{
  rootfile = new TFile("plots.root");
  
  h[0]=hRespIetaLowEtLowDEta->ProfileY("_0");
  h[1]=hRespIetaLowEtMidDEta->ProfileY("_1");
  h[2]=hRespIetaLowEtTopDEta->ProfileY("_2");
  h[3]=hRespIetaMidEtLowDEta->ProfileY("_3");
  h[4]=hRespIetaMidEtMidDEta->ProfileY("_4");
  h[5]=hRespIetaMidEtTopDEta->ProfileY("_5");
  h[6]=hRespIetaTopEtLowDEta->ProfileY("_6");
  h[7]=hRespIetaTopEtMidDEta->ProfileY("_7");
  h[8]=hRespIetaTopEtTopDEta->ProfileY("_8");

  TH1* ha=h[0];
  TH1* hb=h[3];
  TH1* hc=h[6];

  ha->SetLineColor(kBlue+2);
  hb->SetLineColor(kCyan+2);
  hc->SetLineColor(kRed+2);
  ha->SetLineWidth(2);
  hb->SetLineWidth(2);
  hc->SetLineWidth(2);
  ha->SetMarkerStyle(21);
  hb->SetMarkerStyle(22);
  hc->SetMarkerStyle(23);
  ha->SetMarkerSize(0.3);
  hb->SetMarkerSize(0.3);
  hc->SetMarkerSize(0.3);
  ha->SetMarkerColor(kBlue+2);
  hb->SetMarkerColor(kCyan+2);
  hc->SetMarkerColor(kRed+2);

  ha->Draw();
  ha->GetXaxis()->SetTitle("|#eta_{i}|");
  ha->GetYaxis()->SetTitle("E_{T}^{T}-E_{T}^{P} / E_{T}^{T}+E_{T}^{P}");
  hb->Draw("same");
  hc->Draw("same");
  ha->SetStats(0);

  ha->SetMinimum(-.3);
  ha->SetMaximum(.5);
  
  TLegend *leg=new TLegend(.25,.6,.75,.85);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);

  for(int i=0; i<=8; i++) {
    TString str;
    if(i==0) str="50 GeV > E_{T} > 20 GeV; #Delta|#eta|<0.5";
    if(i==1) str="50 GeV > E_{T} > 20 GeV; 0.5<#Delta|#eta|<1.0";
    if(i==2) str="50 GeV > E_{T} > 20 GeV; 1.0<#Delta|#eta|<1.5";
    if(i==3) str="80 GeV > E_{T} > 50 GeV; #Delta|#eta|<0.5";
    if(i==4) str="80 GeV > E_{T} > 50 GeV; 0.5<#Delta|#eta|<1.0";
    if(i==5) str="80 GeV > E_{T} > 50 GeV; 1.0<#Delta|#eta|<1.5";
    if(i==6) str="E_{T} > 80 GeV; #Delta|#eta|<0.5";
    if(i==7) str="E_{T} > 80 GeV; 0.5<#Delta|#eta|<1.0";
    if(i==8) str="E_{T} > 80 GeV; 1.0<#Delta|#eta|<1.5";
    if(ha==h[i])   leg->AddEntry(ha, str, "lp");
    if(hb==h[i])   leg->AddEntry(hb, str, "lp");
    if(hc==h[i])   leg->AddEntry(hc, str, "lp");
  }
  leg->Draw();

}
