#!/bin/bash
#   - sudo apt-get install lazarus lcl-nogui
#   - fpc -i
#   - lazbuild -v
cd CodeFmt && lazbuild *.lpi && cd ..
cd E09Compiler && lazbuild *.lpi && cd ..
cd GodFather && lazbuild *.lpi && cd ..
cd GrEnDemo && lazbuild *.lpi && cd ..
cd ImageIsolator && lazbuild *.lpi && cd ..
cd ImageKnife && lazbuild *.lpi && cd ..
cd VFretboard && lazbuild *.lpi && cd ..
