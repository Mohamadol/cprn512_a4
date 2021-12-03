

if { $argc != 2 } {
	 set script [info script]
	 puts stderr "USAGE: xsdb ${script} PROCESSOR_TARGET \[executable.elf=\[glob *.elf\]\]"
	 exit 1
}
connect
set target_name [lindex $argv 0]
set elf [lindex $argv 1]
#set elf [lindex [glob *.elf] 0]
#try to connect to arm, otherwise connect to A53 otherwise connect to mb
target -set -filter {name =~"$target_name"}

rst -processor

dow $elf
con
configparams force-mem-access 0
