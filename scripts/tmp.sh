#!/bin/bash

ffmpeg -i /Users/charlie/code/VideoPan/scripts/FRONT-final.mov -q 1 -vf transpose=1 /Volumes/VALIANT-MOB/temp/FRONT/%05d.jpg
ffmpeg -i /Users/charlie/code/VideoPan/scripts/RIGHT-final.mov -q 1 -vf transpose=1 /Volumes/VALIANT-MOB/temp/RIGHT/%05d.jpg