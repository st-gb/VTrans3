#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#Call this script from VTrans root directory as current working directory in
# order for the paths in LoadLibrary(...),  Init(...) and TranslateAsXML...(...) to function

from ctypes import * #for C dynamic libraries, c_char_p

#vpath="/home/stefan/workspace-noneclipse/VTrans3/"
vpath="./"
vtransLib = cdll.LoadLibrary(vpath+"libVTrans.so")
obj=vtransLib.Init(c_char_p(b"configuration/VTrans_main_config.xml"),c_char_p(b"configuration"))
print("init return value:" + str(obj))
german = c_char_p(vtransLib.TranslateAsXML_char_array(c_char_p(b"the man"))).value
print("transl:" + str(german))

