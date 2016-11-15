//Helper functions for Config parsing.
#include "ConfigParser.C"

TString HIST_OUTPUT_LOCATION="/home/users/bhashemi/public_html/ZMET2016_NovemberClean/";
TString PLOT_OUTPUT_LOCATION="/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/";

TString parseConfDir(TString conf_path){
  /* Replace *configs/.../FNAME.conf with .../ */
  conf_path = TString(conf_path(conf_path.Index("configs/")+8, conf_path.Last('/')-conf_path.Index("configs/")-7)); 
  return conf_path;
}

TString getOutputDir(ConfigParser *conf, TString type){
  /*Determines the proper output locations of files by parsing the option conf_path to get the directory structure above the level 'configs/'*/
	if (type == "hist"){
		if (conf->get("histo_output_dir") != ""){
			return TString(conf->get("histo_output_dir"));
		}
		else{
			TString output_dir = parseConfDir(conf->get("conf_path"));
			return output_dir.Prepend(HIST_OUTPUT_LOCATION);
		}
	}
	else if (type == "plot" )
	{
		if (conf->get("save_dir") != ""){
			return TString(conf->get("save_dir"));
		}
		else{
      TString conf_path = conf->get("conf_path");
      TString output_dir = parseConfDir(conf_path);
      output_dir.Prepend(PLOT_OUTPUT_LOCATION);
      conf_path = TString(conf_path(conf_path.Last('/')+1, conf_path.Last('.')-conf_path.Last('/')-1)); //get name of config file
      output_dir+=conf_path+"/";
      //also add filename for conf script
      return output_dir
		}	
	}
}

TString getDefaultHistDir(ConfigParser *conf){
	/*Returns the default hist output location + the conf_path defined by the location of the config file*/
	return HIST_OUTPUT_LOCATION+parseConfDir(conf->get("conf_path"));
}

TString parseLatex(TString opt){
  /* Replaces \ with # to be in line with ROOT's latex syntax */
  opt.ReplaceAll("\\","#");
  return opt;
}

vector<double> parseVector(TString opt){
	/* Parses options in the config files which are formatted to be vectors [double,double,double,...]*/
	vector<double> ret;
  TString token;
  Ssiz_t from=0;
  //cout<<"got vector in string form: "<<opt<<endl;
  while(opt.Tokenize(token, from, "[,]")){
    token.ReplaceAll("[", "");
    token.ReplaceAll("]", "");
    token.ReplaceAll(",", "");
    token.ReplaceAll(" ", "");
    //cout<<"token: "<<token<<endl;
    ret.push_back(stod(token.Data()));
  }
  return ret;
}