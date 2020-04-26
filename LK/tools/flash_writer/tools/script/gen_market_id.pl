#! /usr/bin/perl

#------------------
#
# Gen Market ID Binary
#
#------------------

#------------------
# check argument
#------------------
if ($#ARGV < 1) {
	print "$#ARGV\n";
	die "\nUsage:\n\tgen_market_id.pl <market_id> <market_id_output_file>\n";
}

#------------------
# read argv
#------------------
my $market_id = $ARGV[0];
my $fout = $ARGV[1];

open (MARKET_ID_BIN, ">$fout") or die "cannot open output file $!\n";

my $hex = sprintf("%08x", $market_id);
print "market_id: " . $market_id . ", 0x" . $hex . "\n";

print MARKET_ID_BIN pack('I', $market_id) ;
close(MARKET_ID_BIN);
