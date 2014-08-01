export LD_LIBRARY_PATH=$PWD:.:$PWD/lib:/usr/lib:$LD_LIBRARY_PATH

if [ -f ./cp_subsys_ert ]; then
    Subsys="eRT."
elif [ -e ./cp_subsys_emg ]; then
    Subsys="eMG."
else
    Subsys=""
fi

if [ "x"$Subsys = "x" ];then
    ./CcspCliSsp
else
    echo "./CcspCliSsp -subsys $Subsys"
    ./CcspCliSsp -subsys $Subsys
fi

