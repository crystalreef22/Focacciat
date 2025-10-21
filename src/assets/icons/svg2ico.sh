#!/bin/bash
# from https://graphicdesign.stackexchange.com/questions/77359/how-to-convert-a-square-svg-to-all-size-ico
magick focacciat-icon.svg -transparent white -background none -alpha set -define icon:auto-resize focacciat-icon.ico
