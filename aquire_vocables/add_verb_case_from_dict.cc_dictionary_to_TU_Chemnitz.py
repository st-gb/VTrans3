#!/usr/bin/python

#https://www.tutorialspoint.com/python/python_command_line_arguments.htm
import sys #for command line arguments
import re #for regular expressions

if len(sys.argv) < 3 :
	print "too few command line arguments. Usage <file path of TU Chemnitz dictionary> <file path of dict.cc dictionary>"
	print "e.g.\"../dictionaries/TUchemnitz/en-de/en-de_composed_from_word_classes2,sorted,wout\' \'.txt ../dictionaried/dict.cc_EN-DE_2017-08-29_14.09_cmkbkkgbgs-77477285-ei8io7.txt\""
	sys.exit()

filePathOfTUchemnitzDictionary = sys.argv[1]
print "file path of TU Chemnitz dictionary:" + filePathOfTUchemnitzDictionary
filePathOfDict_ccDictionary = sys.argv[1]
print "file path of dict.cc dictionary:" + filePathOfDict_ccDictionary

TUchemnitzDictionaryFile = open(filePathOfTUchemnitzDictionary,"r")
Dict_ccDictionaryFile = open(filePathOfDict_ccDictionary,"r")

# https://docs.python.org/2/library/re.html

transitiveVerbRegEx = re.compile(".*{vt}.*")
dict_ccVerbLineRegEx = re.compile(".*verb")

for TUchemnitzDictionaryLine in TUchemnitzDictionaryFile :
    #if transitiveVerbRegEx.match(TUchemnitzDictionaryLine) :
    posOfTransitiveVerbMark = TUchemnitzDictionaryLine.find("{vt}")
    if posOfTransitiveVerbMark > -1 :
	#the first non-letter
	index = 0
	for character in TUchemnitzDictionaryLine :
		#if not character.isalpha() and character != ' ':
		if character == ':' or character == ';' or character == '|' :
			posOf1stNonLetterChar = index
			break
		index = index + 1
	#posOf1stNonLetterChar = TUchemnitzDictionaryLine.find(" ")
	verb = TUchemnitzDictionaryLine[0:posOf1stNonLetterChar]
	print  TUchemnitzDictionaryLine + "verb:" + verb
	#print TUchemnitzDictionaryLine + "matches"
	for dict_ccLine in Dict_ccDictionaryFile :
	      if dict_ccLine.find("verb") > -1 :
	        print "\"verb\" found in dict.cc dictionary"
	      if dict_ccLine.find("verb") > -1 and dict_ccLine.find(verb) > -1 :
	        print verb + "found in dict.cc dictionary"
    		if dict_ccLine.find("jdm.") > -1 :
			TUchemnitzDictionaryLine = TUchemnitzDictionaryLine.replace("{vt}", "{vdI}")
			print TUchemnitzDictionaryLine + "matches"
			print dict_ccLine + "matches" + verb
		elif dict_ccLine.find("jdn.") > -1 :
			TUchemnitzDictionaryLine.replace("{vt}", "{vaI}")
			print TUchemnitzDictionaryLine + "matches"

