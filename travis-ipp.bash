if [[ "$TRAVIS_IPP" == "TRUE" ]]; then
    sudo echo deb https://apt.repos.intel.com/ipp all main > /etc/apt/sources.list.d/intel-ipp.list
    sudo apt-get install intel-ipp-64bit
    IPPARG=""
else
    IPPARG="--noipp"
fi
