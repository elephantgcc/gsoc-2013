#!/usr/bin/python
# -*- coding: utf-8 -*-

# Coding challenge for Apertium GSOC2013 
# of idea "Sliding-window part-of-speech tagger":

# Write a filter that reads in the output of Apertium 
# morphological analyser and writes out either a random 
# one (-r) or the first one (-f) of the lexical form for
# each surface form in a new format, respecting superblanks.
# http://wiki.apertium.org/wiki/Ideas_for_Google_Summer_of_Code/Sliding-window_part-of-speech_tagger

# Example input line:
# ^I/I<num><mf><sg>/I<prn><subj><p1><mf><sg>$ ^have/have<vbhaver><inf>/have<vbhaver><pres>/have<vblex><inf>/have<vblex><pres>$ 
# output:
# I.prn.subj.pl.mf.sg  have.vbhaver.inf

#2013-04-20 by Gang Chen

import sys
import re
import random
from optparse import OptionParser

# mode = True, output first one
# mode = False, output random one
def filt(mode):

    word_info_re = re.compile(r"\^.*?\$")
    attr_re = re.compile(r"<(.*?)>")

    line = sys.stdin.readline()
    while line != '':
        # respecting superblanks [.*], skip them
        line = re.sub("\[.*\]", "", line)
        # e.g. word_info = ^I/I<num><mf><sg>/I<prn><subj><p1><mf><sg>$
        for word_info in word_info_re.findall(line):
            # e.g. attr_info_list = ['I<num><mf><sg>', 'I<prn><subj><p1><mf><sg>']
            attr_info_list = word_info[ 1 : -1].split("/")[ 1 : ]
            if mode == True:
                print attr_re.sub(r".\1", attr_info_list[0]),
            else:
                print attr_re.sub(r".\1", random.choice(attr_info_list)),
        print ""
        line = sys.stdin.readline()

if __name__ == "__main__":
    usage = "%prog [option]"
    parser = OptionParser(usage)
    parser.add_option("-f", "--first", action="store_true", dest="first", default = False, \
                      help = "output first morphological analysis for each word")
    parser.add_option("-r", "--random", action="store_true", dest="random", default = False, \
                      help = "output random morphological analysis for each word")
    (options, args) = parser.parse_args()
    if options.first == True:
        filt(True)
    elif options.random == True:
        filt(False)
    else:
        exit(1)

