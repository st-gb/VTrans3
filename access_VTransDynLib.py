#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from ctypes import *


#vpath="/home/stefan/workspace-noneclipse/VTrans3/"
vpath="./"
vtransLib = cdll.LoadLibrary(vpath+"libVTrans.so")
obj=vtransLib.Init("configuration/VTrans_main_config.xml","configuration")
print("init return value:" + str(obj))
german=vtransLib.Translate("the man")
print("transl:" + german)

