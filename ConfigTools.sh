#ConfigTools.sh
#This script contains methods which are useful in copying configs to make another analysis

function addDefaultToConfigs {
  if [[ $# < 3 ]]
  then
    echo "addDefaultToConfigs <opt=arg> <Directory> <Filename>"
    return 1
  fi

  echo "Would you like to add: DEFAULT::$1"
  echo "To the first line of the following:"
  find $2 -name "$3"
  echo -n "Please type yes if so: "
  read input_from_user
  if [[ $input_from_user == "yes" ]]
  then
    for f in `find $2 -name "$3"`
    do
      sed -i "1 i\DEFAULT::$1" $f
      #echo "File Succesfully Altered: "`head -n1 $f`
    done
    echo "File Succesfully Altered"
  else
    echo "Aborting, nothing added to files..."
  fi
}

function replaceInConfigs {

  if [[ $# < 4 ]]
  then
    echo "replaceInConfigs <lineInFile> <Replacement> <Directory> <Filename>"
    return 1
  fi

  echo "Would you like to replace the instances of $1 with $2"
  echo "In the following files:"
  find $3 -name "$4"
  echo -n "Please type yes if so: "
  read input_from_user
  if [[ $input_from_user == "yes" ]]
  then
    for f in `find $3 -name "$4"`
    do
      sed -i.bak "s,$1,$2,g" $f
      #echo "File Succesfully Altered: "`head -n1 $f`
    done
    echo "File Succesfully Altered"
  else
    echo "Aborting, nothing added to files..."
  fi
}

function findConfLocation {
  #1 = file location
  #2 = Config name

  ConfigTools_Conf_Location=`grep -n -m1 "^Name=$2[[:space:]]*$" $1 | cut -d':' -f1`
}

function AddOpt {
  #1 == file location
  #2 == config name
  #3 == opt name
  #4 == opt arg

  echo "Would you like to add the option $3=$4"
  echo "In the config $2 of $1"
  echo -n "Please type yes if so: "
  read input_from_user
  if [[ $input_from_user == "yes" ]]
  then
    findConfLocation $1 $2
    if [[ ! -z $ConfigTools_Conf_Location ]]
    then
      sed -i.bak "$((ConfigTools_Conf_Location+1)) i$3=$4" $1
      echo "File Succesfully Altered"
    else
      echo "Error: Could not find config named $2 in $1."
    fi
  else
    echo "Aborting, nothing added to files..."
  fi
}

function _AddOpt {
  #This should not be used by a human, only for automating the addition of optionds
  #1 == file location
  #2 == config name
  #3 == opt name
  #4 == opt arg
  findConfLocation $1 $2
  if [[ ! -z $ConfigTools_Conf_Location ]]
  then
    sed -i.bak "$((ConfigTools_Conf_Location+1)) i$3=$4" $1
    echo "File Succesfully Altered: $1"
  else
    echo "Error: Could not find config named $2 in $1."
  fi
}

function AddOptToDir {
  #1 = dir 
  #2 = file name
  #3 = config name
  #4 = opt name
  #5 = opt arg

  echo "Would you like to add the option $4=$5"
  echo "To the config $3 in the following files: "
  find $1 -name "$2"
  echo -n "Please type yes if so: "
  read input_from_user
  if [[ $input_from_user == "yes" ]]
  then
    for f in `find $1 -name "$2"`
    do
      _AddOpt $f $3 $4 $5
    done
  else
    echo "Aborting, nothing added to files..."
  fi
}