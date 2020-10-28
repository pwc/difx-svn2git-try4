if [[ "$TRAVIS_IPP" == "TRUE" ]]; then
    sudo bash -c 'echo deb https://apt.repos.intel.com/ipp all main > /etc/apt/sources.list.d/intel-ipp.list'
    sudo apt-get update
    sudo apt-get install intel-ipp-2020.0-088
    IPPARG=""
else
    IPPARG="--noipp"
fi
