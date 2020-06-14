#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2020 - LBS - (Single person developer.)                 #
# Tue Mar 24 11:56:17 CET 2020                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                                                                      #
# Input paramete:                                                      #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

LC_TIME=en_US.UTF-8
LANG=en_US.UTF-8

source /home/burmist/root_v6.14.00/root-6.14.00-install/bin/thisroot.sh

function ex01_push_sh {
    vecparID=0
    $sourceHome/ex01_push 0 $outRootFile $vecparID 2 40.5 55.3
}

function ex01_push_bunch_sh {
    vecparID=0
    for VAR in `shuf -i1-10 -n10 | xargs`
    do
	echo $VAR
	sleep 1
	$sourceHome/ex01_push 0 $outRootFile $vecparID 2 $VAR `shuf -i1-10 -n1`
    done
}

function ex02_plot_sh {
    $sourceHome/ex02_plot 0 $inRootFile $vecNamesFile
}

function printHelp {
    echo " --> ERROR in input arguments "
    echo " [0] -push   : example of the push "
    echo " [0] -pushb  : example of the consecutive push "
    echo " [0] -plot   : example of the plot "
    echo " [0] -h      : print help"
}

sourceHome=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
dataFolder=$sourceHome'/data/'
outRootFile=$dataFolder'vector_in.root'
inRootFile=$outRootFile
vecNamesFile=$sourceHome'/vectorOfinDataValuesNames.dat'

mkdir -p $dataFolder

if [ $# -eq 0 ]; then    
    printHelp
else
    if [ "$1" = "-push" ]; then
	if [ $# -eq 1 ]; then
	    ex01_push_sh
	else
            printHelp
        fi
    elif [ "$1" = "-pushb" ]; then
	    ex01_push_bunch_sh
    elif [ "$1" = "-plot" ]; then
	    ex02_plot_sh
    elif [ "$1" = "-h" ]; then
        printHelp
    else
        printHelp
    fi
fi
#espeak "I have done"
