#!/bin/bash

projname=$( ls *.pro | head -n1 | sed 's/\.pro$//' )
mv -v "gerbers/${projname}-Edge_Cuts.gbr" "gerbers/${projname}-Edge_Cuts.gko"
