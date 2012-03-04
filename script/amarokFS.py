#!/usr/bin/env python
#
# AmarokFS startup script for Amarok
# Copyright (c) 2007 Martin Kossler <martin.kossler@students.uni-marburg.de>
#############################################################################
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#############################################################################

from commands import getoutput
from os import system, getenv
from signal import signal
from sys import exit, stdin
import re

# Ask for number of Screens
numberOfScreens = int(getoutput('xdpyinfo |grep "number of screens"|sed "s/^number of screens:\s*//"'))

def main():
	""" Main application loop """
	# Test Amarok DCOP connection
	if system('dcop amarok'):
		print 'This script is meant to be called from within Amarok.\n'
		exit(1)
	
	# Add "amarokFS" custom menu entry
	for i in range(numberOfScreens):
		if numberOfScreens > 1:
			menuString = 'Run AmarokFS on Screen ' + str(i+1)
		else:
			menuString = 'Run Amarok Full Screen'
		if system('dcop amarok script addCustomMenuItem "AmarokFS" "' + menuString + '"'):
			system('kdialog --error "Could not create \'amarokFS\' menu entries."')
			exit(1)
	
	while 1:
		# Read notifications
		notification = stdin.readline()
		
		if notification != '':
			if 'customMenuClicked' in notification:
				if 'AmarokFS' in notification:
					display = getenv("DISPLAY")
					if numberOfScreens > 1:
						screen = int(re.search('Screen (\d+)', notification).group(1))-1
						display = re.sub('\.\d+$', '.' + str(screen), display)
					if (system('amarokFS -display ' + display)) != 0:
						system('dcop amarok playlist popupMessage "There was an error launching AmarokFS.\nPlease check your installation and configuration."')
						stopScript(None, None)
						exit(1)
			elif 'configure' in notification:
				system('amarokFS --settings')
		else:
			exit(0)


def stopScript(signum, stackframe):
	""" Executed when script is stopped by the user """
	for i in range(numberOfScreens):
		if numberOfScreens > 1:
			menuString = 'Run AmarokFS on Screen ' + str(i+1)
		else:
			menuString = 'Run Amarok Full Screen'
		system('dcop amarok script removeCustomMenuItem "AmarokFS" "' + menuString + '"')

def forceStop(signum, stackframe):
	""" Handler to stop the script when Amarok crashes """
	exit(1)

if __name__ == "__main__":
	signal(6, stopScript)
	signal(15, stopScript)
	main()
