import sys       
import subprocess
import re

# Calls the R system specifying that commands come from file commands.R
# The commands.R provided with this assignment will read the file named
# data and will output a histogram of that data to the file pageshist.pdf
def runR( ):
    res = subprocess.call(['R', '-f', 'commands.R'])

# log2hist analyzes a log file to calculate the total number of pages
# printed by each user during the period represented by this log file,
# and uses R to produce a pdf file pageshist.pdf showing a histogram
# of these totals.  logfilename is a string which is the name of the
# log file to analyze.
#
def log2hist(logfilename):
    # fill in your code here
    printers = dict() #creation of dictionary
    file = open(logfilename, "r")
    for line in file:
        index_user = line.find("user:")
        if index_user == -1:
            continue
        new_line = line[index_user + 5 : len(line)]
        #get rid of spaces
        new_line = new_line.strip()
        index_space = new_line.find(" ")
        user = new_line[0:index_space]
        index_pages = new_line.find("pages:")
        new_line = new_line[index_pages + 6 : len(new_line)]
        #get rid of front spaces
        new_line = new_line.strip()
        index_space = new_line.find(" ")
        # print index_space
        if index_space != -1:
            pages = int(new_line[0:index_space])
        else:
            pages = int(new_line)
        #print 'user: ' + user + '\tpages: ' + str(pages)
        if user in printers:
            printers[user] = printers[user] + pages
        else:
            printers[user] = pages
   # print printers
    file.close()
    new_file = open("data", "w")
    for x in printers.values():
        new_file.write(str(x) + '\n')
    new_file.close()
    runR()
    return

if __name__ == '__main__':
    log2hist(sys.argv[1])   # get the log file name from command line

# line above may be changed to log2hist("log") to make the file name
#    always be log

