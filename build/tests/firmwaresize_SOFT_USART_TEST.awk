BEGIN {ORS="";print "\n\033[1;33mFirmware size ("}
/^Device/ {print $2 ") is...  "}
/^Program/ {print "Flash (program): " $2 " " $3 " " $4 ")  "}
/^Data/ {print "RAM (globals): " $2 " " $3 " " $4 ")  "}
END {print "\033[0m\n\n"}
