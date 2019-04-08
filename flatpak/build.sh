#!/bin/sh
rm -rf ./app
mkdir ./app
rm -rf ./repo
mkdir ./repo
flatpak-builder --ccache --repo=repo --subject="Test Build `date`" app org.open1x.Rollin.json
