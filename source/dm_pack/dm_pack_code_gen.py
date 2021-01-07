#
# If not stated otherwise in this file or this component's Licenses.txt file the
# following copyright and licenses apply:
#
# Copyright 2015 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#
#  DM PACK CODE GENERATOR
#
#  Converts a datamodel xml into a series of c function calls 
#   which will create an xml dom tree directly at runtime.
#
#  Usage: python dm_pack_code_gen.py in_xml_file out_c_file
#

import xml.etree.ElementTree as ET
import sys
import re

ParamBool=0
ParamInt=1
ParamUlong=2
ParamString=3

ObjectPlain=0
ObjectStaticTable=1
ObjectDynamicTable=2,
ObjectWritableTable=3,
ObjectDynWritableTable=4

func_GetEntryCount=0
func_GetEntryStatus=1
func_GetEntry=2
func_AddEntry=3
func_DelEntry=4
func_IsUpdated=5
func_Synchronize=6
func_Lock=7
func_Unlock=8
func_CheckInstance=9
func_GetParamBoolValue=10
func_GetParamIntValue=11
func_GetParamUlongValue=12
func_GetParamStringValue=13
func_GetBulkParamValues=14
func_SetParamBoolValue=15
func_SetParamIntValue=16
func_SetParamUlongValue=17
func_SetParamStringValue=18
func_SetBulkParamValues=19
func_Validate=20
func_Commit=21
func_Rollback=22
func_Init=23
func_Async_Init=24
func_Unload=25
func_MemoryCheck=26
func_MemoryUsage=27
func_MemoryTable=28
func_IsObjSupported=29

def initObjectNames(objectMap):
  objectMap["object"]=0
  objectMap["staticTable"]=1
  objectMap["dynamicTable"]=2
  objectMap["writableTable"]=3
  objectMap["dynWritableTable"]=4

def initFuncNames(funcMap):
  funcMap["func_GetEntryCount"]=0
  funcMap["func_GetEntryStatus"]=1
  funcMap["func_GetEntry"]=2
  funcMap["func_AddEntry"]=3
  funcMap["func_DelEntry"]=4
  funcMap["func_IsUpdated"]=5
  funcMap["func_Synchronize"]=6
  funcMap["func_Lock"]=7
  funcMap["func_Unlock"]=8
  funcMap["func_CheckInstance"]=9
  funcMap["func_GetParamBoolValue"]=10
  funcMap["func_GetParamIntValue"]=11
  funcMap["func_GetParamUlongValue"]=12
  funcMap["func_GetParamStringValue"]=13
  funcMap["func_GetBulkParamValues"]=14
  funcMap["func_SetParamBoolValue"]=15
  funcMap["func_SetParamIntValue"]=16
  funcMap["func_SetParamUlongValue"]=17
  funcMap["func_SetParamStringValue"]=18
  funcMap["func_SetBulkParamValues"]=19
  funcMap["func_Validate"]=20
  funcMap["func_Commit"]=21
  funcMap["func_Rollback"]=22
  funcMap["func_Init"]=23
  funcMap["func_Async_Init"]=24
  funcMap["func_Unload"]=25
  funcMap["func_MemoryCheck"]=26
  funcMap["func_MemoryUsage"]=27
  funcMap["func_MemoryTable"]=28
  funcMap["func_IsObjSupported"]=29

#printElement is only for debugging that we know how to write code that walks the tree 
def printElement(elem,depth):
  offset=""
  for i in range(depth):
    offset = offset + "  "
  if(len(elem)==0):
    if elem.text:
      print(offset+"<"+elem.tag+">"+elem.text+"</"+elem.tag+">")
    else:
      print(offset+"<"+elem.tag+">"+"</"+elem.tag+">")
  else:
    print(offset+"<"+elem.tag+">")
    for subelem in elem:
      printElement(subelem,depth+1)
    print(offset+"</"+elem.tag+">")

def calcMaxDepth(elem,depth):
  if elem.tag=="parameter":
    return
  if elem.tag=="function":
    return;
  maxdepth = depth
  for subelem in elem:
    childdepth = calcMaxDepth(subelem,depth+1)
    if childdepth > maxdepth:
      maxdepth = childdepth
  return maxdepth

def codeGenObject(elem,depth,fout):
  name=""
  objectType=0
  maxInstances=False
  for subelem in elem:
    if subelem.tag=="name":
      name=subelem.text
    elif subelem.tag=="objectType":
      objectType = objectMap[subelem.text];
    elif subelem.tag=="maxInstance":
      maxInstances=subelem.text;
  offset=" "
  for i in range(depth):
    offset = offset + " "
  if maxInstances:
    fout.write("%sP%d=DMPackCreateObject(P%d, %d,\"%s\",\"%s\");\n" % (offset, depth, depth-1, objectType, name, maxInstances));
  else:
    fout.write("%sP%d=DMPackCreateObject(P%d, %d,\"%s\",0);\n" % (offset, depth, depth-1, objectType, name));
  for subelem in elem:
    if subelem.tag=="parameters":
      codeGenParams(subelem,depth+1,fout)
    elif subelem.tag=="functions":
      codeGenFuncs(subelem,depth+1,fout)
    elif subelem.tag != "name" and subelem.tag != "objectType" and subelem.tag != "maxInstance":
      codeGenElement(subelem,depth+1,fout)

def codeGenFuncs(elem,depth,fout):
  global funcMap
  funcPrefix=""
  numParams=0
  params=""
  offset=" "
  for i in range(depth):
    offset = offset + " "
  for subelem in elem:
    funcName=subelem.tag
    prefix=subelem.text.split(funcName.split("func")[1])[0];
    if prefix != funcPrefix:
      if funcPrefix!="":
        #I saw this weird case happen 2 times in TR181-USGv2.XML
        #one param was prefixed CodeBigFirst but its other sibling was prefixed CodeBig_First*
        #In another object, all the sibling params were were GreTunnelIf but 1 oddball was GreTunnelIF (note the cap I)
        #I don't know if this was put in the xml intentionally or this is human error
        #to be safe I will preserve what is there by switching to the new unexpected prefix
        fout.write("%sDMPackCreateFunctions(P%d,\"%s\",%d,%s);\n" %  (offset, depth-1, funcPrefix, numParams, params))
        numParams=0
        params=""
      funcPrefix = prefix
    if len(params) > 0:
      params = params + ","
    params = params + str(funcMap[funcName]);
    numParams = numParams + 1
  if numParams > 0:
    fout.write("%sDMPackCreateFunctions(P%d,\"%s\",%d,%s);\n" %  (offset, depth-1, funcPrefix, numParams, params))
  else:
    fout.write("%sDMPackCreateFunctions(P%d,\"%s\",0);\n" %  (offset, depth-1, funcPrefix))

def codeGenParams(elem,depth,fout):
  offset=" "
  for i in range(depth):
    offset = offset + " "

  fout.write("%sP%d=DMPackCreatePNode(P%d,\"parameters\");\n" % (offset, depth, depth-1))

  offset = offset + " "
  depth = depth+1

  for subelem in elem:
    pname=""
    ptype=-1
    pstrtype=""
    psyntax1=""
    psyntax2=""
    pwritable=-1
    pnotify=-1
    for subelem2 in subelem:
      if subelem2.tag=="name":
        pname=subelem2.text;
      elif subelem2.tag=="type":
        pstrtype=subelem2.text;
        if pstrtype=="boolean":
          ptype=ParamBool;
          psyntax1="bool"
        elif pstrtype=="int":
          ptype=ParamInt;
          psyntax1="int"
        elif pstrtype=="unsignedInt":
          ptype=ParamUlong;
          psyntax1="uint32"
        elif subelem2.text=="string":
          ptype=ParamString;
          psyntax1="string"
      elif subelem2.tag=="syntax":
        psyntax2=subelem2.text;
      elif subelem2.tag=="writable":
        if subelem2.text == "false":
          pwritable=0
        elif subelem2.text == "true":
          pwritable=1
      elif subelem2.tag=="notify":
        if subelem2.text == "off":
          pnotify=0
        elif subelem2.text == "on":
          pnotify=1
    if ptype == -1:
      if psyntax1 != psyntax2:
        if pwritable != -1:
          if pnotify != -1:
            code = "%sDMPackCreateParamTSWN(P%d,\"%s\",\"%s\",\"%s\",%d,%d);\n" % (offset, depth-1, pname, pstrtype, psyntax2, pwritable, pnotify);
          else:
            code = "%sDMPackCreateParamTSW(P%d,\"%s\",\"%s\",\"%s\",%d);\n" % (offset, depth-1, pname, pstrtype, psyntax2, pwritable);
        elif pnotify != -1:
          code = "%sDMPackCreateParamTSN(P%d,\"%s\",\"%s\",\"%s\",%d);\n" % (offset, depth-1, pname, pstrtype, psyntax2, pnotify);
        else:
          code = "%sDMPackCreateParamTS(P%d,\"%s\",\"%s\",\"%s\");\n" % (offset, depth-1, pname, pstrtype, psyntax2);
      elif pwritable != -1:
        if pnotify != -1:
          code = "%sDMPackCreateParamTWN(P%d,\"%s\",\"%s\",%d,%d);\n" % (offset, depth-1, pname, pstrtype, pwritable, pnotify);
        else:
          code = "%sDMPackCreateParamTW(P%d,\"%s\",\"%s\",%d);\n" % (offset, depth-1, pname, pstrtype, pwritable);
      elif pnotify != -1:
          code = "%sDMPackCreateParamTN(P%d,\"%s\",\"%s\",%d);\n" % (offset, depth-1, pname, pstrtype, pnotify);
      else:
        code = "%sDMPackCreateParamT(P%d,\"%s\",\"%s\");\n" % (offset, depth-1, pname, pstrtype);
    elif psyntax1 != psyntax2:
      if pwritable != -1:
        if pnotify:
          code = "%sDMPackCreateParamSWN(P%d,\"%s\",%d,\"%s\",%d,%d);\n" % (offset, depth-1, pname, ptype, psyntax2, pwritable, pnotify);
        else:
          code = "%sDMPackCreateParamSW(P%d,\"%s\",%d,\"%s\",%d);\n" % (offset, depth-1, pname, ptype, psyntax2, pwritable);
      elif pnotify != -1:
          code = "%sDMPackCreateParamSN(P%d,\"%s\",%d,\"%s\",%d);\n" % (offset, depth-1, pname, ptype, psyntax2, pnotify);
      else:
          code = "%sDMPackCreateParamS(P%d,\"%s\",%d,\"%s\");\n" % (offset, depth-1, pname, ptype, psyntax2);
    elif pwritable != -1:
      if pnotify != -1:
        code = "%sDMPackCreateWN(P%d,\"%s\",%d,%d,%d);\n" % (offset, depth-1, pname, ptype, pwritable, pnotify);
      else:
        code = "%sDMPackCreateW(P%d,\"%s\",%d,%d);\n" % (offset, depth-1, pname, ptype, pwritable);
    elif pnotify != -1:
      code = "%sDMPackCreateN(P%d,\"%s\",%d,%d);\n" % (offset, depth-1, pname, ptype, pnotify);
    else:
      code = "%sDMPackCreateParam(P%d,\"%s\",%d);\n" % (offset, depth-1, pname, ptype);
    fout.write(code);
    
def codeGenElement(elem,depth,fout):
  code = ""
  offset=" "
  code=""
  for i in range(depth):
    offset = offset + " "
  code = "%sP%d=" % (offset, depth)
  if len(elem)==0 and elem.text: 
    code = code + "DMPackCreateNode("
  else:
    code = code + "DMPackCreatePNode("
  if depth==0:
    code = code + "0,"
  else:
    code = code + "P%d," % (depth-1)
  code = code + "\"%s\"" % elem.tag
  if len(elem)==0 and elem.text:
    code = code + ",\"%s\",%d" % (elem.text,len(elem.text))
  code = code + ");\n"
  fout.write(code);
  for subelem in elem:
    if subelem.tag=="object":
      codeGenObject(subelem,depth+1,fout)
    else:
      codeGenElement(subelem,depth+1,fout)

def codeGen(xmlFile, outFile):
  fout = open(outFile, "w")
  tree = ET.parse(xmlFile)
  root = tree.getroot()
  fout.write("/*\nDM PACK\ncreated with:\npython %s %s %s\n*/\n\n" % (sys.argv[0], sys.argv[1], sys.argv[2]));
  fout.write("#include \"ansc_platform.h\"\n")
  fout.write("#include \"ansc_xml_dom_parser_interface.h\"\n")
  fout.write("#include \"dm_pack_xml_helper.h\"\n")
  fout.write("PVOID DMPackCreateDataModelXML(){\n")
  maxdepth = calcMaxDepth(root,0)
  fout.write(" PANSC_XML_DOM_NODE_OBJECT P0")
  for i in range(1,maxdepth):
    fout.write(",P%d" % i)
  fout.write(";\n")
  codeGenElement(root,0,fout)
  fout.write(" return P0;\n}\n")
  fout.close()

class Preprocessor():
    def __init__(self):
        self.cus_vars = {}

    def parse_cus_var(self, xml_str):
        define_regex = r"(<\?define\s*(\w+)\s*=\s*([\w\s\"]+)\s*\?>)"
        matches = re.findall(define_regex, xml_str)
        for group_def, group_name, group_var in matches:
            group_name = group_name.strip()
            group_var = group_var.strip().strip("\"")
            self.cus_vars[group_name] = group_var
            xml_str = xml_str.replace(group_def, "")
        cusvar_regex = r"(\$\(var\.(\w+)\))"
        matches = re.findall(cusvar_regex, xml_str)
        for group_cus, group_var in matches:
            if group_var not in self.cus_vars:
                continue
            xml_str = xml_str.replace(group_cus, self.cus_vars[group_var])
        return xml_str

    def parse_if_elseif(self, xml_str):
        ifelif_regex = r"(<\?(if|elseif)\s*([^\"\s=<>!]+)\s*([!=<>]+)\s*\"*([^\"=<>!]+)\"*\s*\?>)"
        matches = re.findall(ifelif_regex, xml_str)
        for group_ifelif, group_tag, group_left, group_operator, group_right in matches:
            if "<" in group_operator or ">" in group_operator:
                result = eval(group_left + group_operator + group_right)
            else:
                result = eval('"{}" {} "{}"'.format(group_left, group_operator, group_right))
            xml_str = xml_str.replace(group_ifelif, "<?" + group_tag + " " + str(result) + "?>")
        return xml_str

    def parse_ifdef_ifndef(self, xml_str):
        ifdef_cond_regex = r"(<\?(ifdef|ifndef)\s*(!)?([\w]*)\s*([|&]?)\s*(!)?([\w]*)\s*([|&]?)\s*(!)?([\w]*)\s*([|&]?)\s*(!)?([\w]*)\s*([|&]?)\s*(!)?([\w]*)\s*\?>)"
        matches = re.findall(ifdef_cond_regex, xml_str)
        for group_ifdef_cond, group_tag, group_1_neg, group_1,  group_operator1, group_2_neg, group_2, group_operator2, group_3_neg, group_3, group_operator3, group_4_neg, group_4,  group_operator4, group_5_neg, group_5 in matches:
            if "|" in group_operator4 or "&" in group_operator4:
                if group_tag == "ifdef":
                    result = eval(str((group_1 not in self.cus_vars) if "!" in group_1_neg else (group_1 in self.cus_vars)) + group_operator1 + str((group_2 not in self.cus_vars) if "!" in group_2_neg else (group_2 in self.cus_vars)) + group_operator2 + str((group_3 not in self.cus_vars) if "!" in group_3_neg else (group_3 in self.cus_vars)) + group_operator3 + str((group_4 not in self.cus_vars) if "!" in group_4_neg else (group_4 in self.cus_vars)) + group_operator4 + str((group_5 not in self.cus_vars) if "!" in group_5_neg else (group_5 in self.cus_vars)))
                else:
                    result = eval(str((group_1 in self.cus_vars) if "!" in group_1_neg else (group_1 not in self.cus_vars)) + group_operator1 + str(group_2 in self.cus_vars if "!" in group_2_neg else group_2 not in self.cus_vars) + group_operator2 + str(group_3 in self.cus_vars if "!" in group_3_neg else group_3 not in self.cus_vars) + group_operator3 + str(group_4 in self.cus_vars if "!" in group_4_neg else group_4 not in self.cus_vars) + group_operator4 + str(group_5 in self.cus_vars if "!" in group_5_neg else group_5 not in self.cus_vars))
            elif "|" in group_operator3 or "&" in group_operator3:
                if group_tag == "ifdef":
                    result = eval(str((group_1 not in self.cus_vars) if "!" in group_1_neg else (group_1 in self.cus_vars)) + group_operator1 + str((group_2 not in self.cus_vars) if "!" in group_2_neg else (group_2 in self.cus_vars)) + group_operator2 + str((group_3 not in self.cus_vars) if "!" in group_3_neg else (group_3 in self.cus_vars)) + group_operator3 + str((group_4 not in self.cus_vars) if "!" in group_4_neg else (group_4 in self.cus_vars)))
                else:
                    result = eval(str((group_1 in self.cus_vars) if "!" in group_1_neg else (group_1 not in self.cus_vars)) + group_operator1 + str(group_2 in self.cus_vars if "!" in group_2_neg else group_2 not in self.cus_vars) + group_operator2 + str(group_3 in self.cus_vars if "!" in group_3_neg else group_3 not in self.cus_vars) + group_operator3 + str(group_4 in self.cus_vars if "!" in group_4_neg else group_4 not in self.cus_vars))
            elif "|" in group_operator2 or "&" in group_operator2:
                if group_tag == "ifdef":
                    result = eval(str((group_1 not in self.cus_vars) if "!" in group_1_neg else (group_1 in self.cus_vars)) + group_operator1 + str((group_2 not in self.cus_vars) if "!" in group_2_neg else (group_2 in self.cus_vars)) + group_operator2 + str((group_3 not in self.cus_vars) if "!" in group_3_neg else (group_3 in self.cus_vars)))
                else:
                    result = eval(str((group_1 in self.cus_vars) if "!" in group_1_neg else (group_1 not in self.cus_vars)) + group_operator1 + str(group_2 in self.cus_vars if "!" in group_2_neg else group_2 not in self.cus_vars) + group_operator2 + str(group_3 in self.cus_vars if "!" in group_3_neg else group_3 not in self.cus_vars))
            elif "|" in group_operator1 or "&" in group_operator1:
                if group_tag == "ifdef":
                    result = eval(str((group_1 not in self.cus_vars) if "!" in group_1_neg else (group_1 in self.cus_vars)) + group_operator1 + str(group_2 not in self.cus_vars if "!" in group_2_neg else group_2 in self.cus_vars))
                else:
                    result = eval(str((group_1 in self.cus_vars) if "!" in group_1_neg else (group_1 not in self.cus_vars)) + group_operator1 + str(group_2 in self.cus_vars if "!" in group_2_neg else group_2 not in self.cus_vars))
            else:
                if group_tag == "ifdef":
                    result = group_1 in self.cus_vars
                else:
                    result = group_1 not in self.cus_vars
            xml_str = xml_str.replace(group_ifdef_cond, "<?if " + str(result) + "?>")
        return xml_str

    def parse_if_else_if(self, xml_str):
        if_elif_else_regex = r"(<\?if\s(True|False)\?>\n((.(?!<\?))*)\n<\?elseif\s(True|False)\?>\n((.(?!<\?))*)\n<\?else\?>\n((.(?!<\?))*)\n<\?endif\?>\n)"
        if_else_regex = r"(<\?if\s(True|False)\?>\n((.(?!<\?))*)\n<\?else\?>\n((.(?!<\?))*)\n<\?endif\?>\n)"
        if_regex = r"(<\?if\s(True|False)\?>\n((.(?!<\?))*)\n<\?endif\?>\n)"
        matches = re.findall(if_elif_else_regex, xml_str, re.DOTALL)
        for group_full, group_if, group_if_elif,group_none, group_elif, group_elif_else,group_none, group_else, group_none in matches:
            result = ""
            if group_if == "True":
                result = group_if_elif
            elif group_elif == "True":
                result = group_elif_else
            else:
                result = group_else
            xml_str = xml_str.replace(group_full, result+"\n")
        matches = re.findall(if_else_regex, xml_str, re.DOTALL)
        for group_full, group_if, group_if_else, group_none, group_else, group_none in matches:
            result = ""
            if group_if == "True":
                result = group_if_else
            else:
                result = group_else
            xml_str = xml_str.replace(group_full, result+"\n")
        matches = re.findall(if_regex, xml_str, re.DOTALL)
        for group_full, group_if, group_text, group_none in matches:
            result = ""
            if group_if == "True":
                result = group_text
                xml_str = xml_str.replace(group_full, result+"\n")
            else:
                xml_str = xml_str.replace(group_full, result)
        return xml_str

    def need_parse(self, xml_str):
        for keyword in ["<?include", "$(env", "$(var", "$(sys", "<?if", "<?else", "<?end", "<?for", "<?err", "<?war", "<?cmd"]:
            if keyword in xml_str:
                return True
        return False

    def preprocess(self, xml_file, out_file):
        with open(xml_file, "r") as original_file:
	        xml_str = original_file.read()
        proc_functions = [
            self.parse_cus_var,
            self.parse_if_elseif,
            self.parse_ifdef_ifndef,
            self.parse_if_else_if
        ]
        while(self.need_parse(xml_str)):
            for i in range(len(proc_functions)):
                xml_str = proc_functions[i](xml_str)

        try:
            with open(out_file, "w") as processed_file:
                processed_file.write(xml_str)
            return 0
        except:
            return -1

funcMap={}
objectMap={}
if len(sys.argv) != 3:
  print("usage: python ccsp_xml_code_gen.py in_xml_file out_c_file")
else:
  proc = Preprocessor()
  proc.preprocess(sys.argv[1], "output.XML")
  initFuncNames(funcMap)
  initObjectNames(objectMap)
  codeGen("output.XML", sys.argv[2])


