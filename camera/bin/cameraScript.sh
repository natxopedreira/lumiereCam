#!/bin/bash

# Edit this absolute paths
appPath=/Users/Patricio/Desktop/openFrameworks/apps/Lumiere/camera/bin/camera.app/Contents/MacOS/
dataPath=/Users/Patricio/Desktop/openFrameworks/apps/Lumiere/camera/bin/data/

zipName="photos_"`eval date +%Y%m%d%H%M`".tgz"

# Zip previus photos ( if they are in case something went wrong )
cd $dataPath
tar -zcvf $zipName *.jpg
rm -rf *.jpg

# Run the app
sudo $appPath./camera

# Zip photos
cd $dataPath
tar -zcvf $zipName *.jpg
rm -rf *.jpg
