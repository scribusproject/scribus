#!/usr/bin/env python
# $Id: setup.py,v 1.15 2005/01/03 17:58:41 tavis_rudd Exp $
import os

try:
    os.remove('MANIFEST')               # to avoid those bloody out-of-date manifests!!
except:
    pass
    
import SetupTools
import SetupConfig
configurations = (SetupConfig,)
SetupTools.run_setup( configurations )




