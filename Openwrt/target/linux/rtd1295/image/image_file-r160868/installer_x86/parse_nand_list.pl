#!/usr/bin/perl

my %VENDOR = (
	'K' => 'SAMSUNG',
	'H' => 'HYNIX',
	'N' => 'ST',
	'T' => 'TOSHIBA',
	'M' => 'MICRON'
);

my %EMMC_SIZE = (
	'FLASH_EMMC_SIZE_32G' => '32G [29184MB]',
	'FLASH_EMMC_SIZE_16G' => '16G [14592MB]',
	'FLASH_EMMC_SIZE_8G' => '8G [7296MB]',
	'FLASH_EMMC_SIZE_4G' => '4G [3648MB]'
);

my @line;
my $start;
my $vendor;
my $page, $block, $size;
my $flashOutput;

if( @ARGV < 1 ) {
	die "Usage: $0 flash_nand_list.h\n";
}

# flash type selection(emmc/nand)
print "choice\n";
print "\tprompt \"Flash Type\"\n";
print "\tdefault FLASH_TYPE_EMMC\n";
print "config FLASH_TYPE_EMMC\n\tbool \"EMMC\"\n";
print "config FLASH_TYPE_NAND\n\tbool \"NAND\"\n";
print "endchoice\n\n";

# emmc size selection(32G/16G/8G/4G)
print "\n";
foreach (sort keys %EMMC_SIZE) {
	print "menuconfig $_\n";
	print "\tdepends on FLASH_TYPE_EMMC\n";
	print "\tbool \"$EMMC_SIZE{$_}\"\n";
}
print "\n";

# each item in emmc size
foreach (sort keys %EMMC_SIZE) {
	$EMMC_SIZE{$_} =~ /([0-9]+)MB/;
	$size = $1;
	$flashOutput .= sprintf "if $_\n";
	# --
	$flashOutput .= sprintf "\tconfig FLASH_TYPE\n";
	$flashOutput .= sprintf "\tstring\n";
	$flashOutput .= sprintf "\tdefault \"emmc\"\n";
	
	$flashOutput .= sprintf "\tconfig FLASH_SIZE_KB\n";
	$flashOutput .= sprintf "\tint\n";
	$flashOutput .= sprintf ("\tdefault %d\n", $size*1024);
	$flashOutput .= sprintf "endif\n\n";
}
$flashOutput .= sprintf "\n";


# nand manufacturer selection
print "choice\n";
print "\tprompt \"Flash Manufacturer\"\n";
print "\tdefault FLASH_MANUFACTURER_$VENDOR{'H'}\n\tdepends on FLASH_TYPE_NAND\n";
foreach (sort keys %VENDOR) {
	print "config FLASH_MANUFACTURER_$VENDOR{$_}\n";
	print "\tbool \"$VENDOR{$_}\"\n";
}
print "endchoice\n\n";

while(<>) {
	# n_device_type n_device[] in flash_nand_list.h
	if( /n_device_type n_device/ ) { $start = 1; next;}
	if( $start ) {
		s/\s+//g;			# remove space
		s/^\{//;			# remove leading {
		if( /^([KHNTM])/ ) {
			if( $vendor ne $VENDOR{$1} ) {
				if( $vendor ) {
					print "endchoice\n\n";
				}
				$vendor = $VENDOR{$1};
				print "choice\n";
				print "\tprompt \"$vendor Flash\"\n";
				print "\tdepends on FLASH_MANUFACTURER_$vendor\n";				
			}

			@line = split /,/;
			$page = eval($line[1]);
			$block = eval($line[2]);
			$size = hex($line[3]);
			
			print "config FLASH_${vendor}_$line[0]\n";
			printf( "\tbool \"$vendor $line[0]\/%d-MByte\/%d-KByte\/%d-KByte\"\n", 
					$size>>20, $page>>10, $block>>10);

			$flashOutput .= sprintf "if FLASH_${vendor}_$line[0]\n";
			$flashOutput .= sprintf "\tconfig FLASH_PARTNAME\n";
			$flashOutput .= sprintf "\tstring\n";
			$flashOutput .= sprintf "\tdefault \"$vendor $line[0]\"\n";
			# --
			$flashOutput .= sprintf "\tconfig FLASH_TYPE\n";
			$flashOutput .= sprintf "\tstring\n";
			$flashOutput .= sprintf "\tdefault \"nand\"\n";
			# --
			$flashOutput .= sprintf "\tconfig FLASH_SIZE_KB\n";
			$flashOutput .= sprintf "\tint\n";
			$flashOutput .= sprintf( "\tdefault %d\n", $size>>10);
			# --
			$flashOutput .= sprintf "\tconfig FLASH_ERASE_SIZE_KB\n";
			$flashOutput .= sprintf "\tint\n";
			$flashOutput .= sprintf( "\tdefault %d\n", $block>>10);
			# --
			$flashOutput .= sprintf "\tconfig FLASH_PAGE_SIZE\n";
			$flashOutput .= sprintf "\tint\n";
			$flashOutput .= sprintf( "\tdefault %d\n", $page);
			# --
			$flashOutput .= sprintf "\tconfig FLASH_OOB_SIZE\n";
			$flashOutput .= sprintf "\tint\n";
			$flashOutput .= sprintf( "\tdefault %d\n", $page);
			
			$flashOutput .= sprintf "endif\n\n";
		}
	}
}
print "endchoice\n\n";

print $flashOutput;
