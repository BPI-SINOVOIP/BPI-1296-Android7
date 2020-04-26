#! /usr/bin/perl -w

################################################################################
# Realtek Semiconductor Corp. 2014
#
# The script is used to generate the image binaries needed by Android system,
# such as /system, /data, and /cache. In the meantime, improving the usability 
# is another goal, any partition planning changes the user should modify the
# partition.txt/partition_customize.txt only, the rest jobs will be done by this 
# tool.
#
# PS: For nand case, the user must set nand_page_size, nand_block_size located 
# in the Makefile.in under each package folder.
################################################################################


my $cnt = $#ARGV + 1;
die "Too short arguments!" if ($cnt < 3);
print "ARGC=".$cnt."\n";

my $ANDROID_PRODUCT_OUT = $ENV{"ANDROID_PRODUCT_OUT"};
my $ANDROID_HOST_OUT = $ENV{"ANDROID_HOST_OUT"};

### utilities path
my $MKUBIFS_PATH = "../bin/mkfs.ubifs";  # for android project.
my $MKUBIFS_GENERIC_PATH = "../bin/mkfs.ubifs_nas_64";  # for generic linux project. Using dynamic linked version, so that fakeroot can work normally.
my $UBINIZE_PATH = "../bin/ubinize";
my $MAKE_EXT4FS  = "../bin/make_ext4fs";
my $SIMG2IMG     = "../bin/simg2img";
my $E2FSCK_PATH  = "../bin/e2fsck";
my $RESIZE2FS_PATH = "../bin/resize2fs";

### global variables
my $cur_path = `pwd`;
chomp($cur_path);
my $tmp_path = "$cur_path";
print "tmp_path=$tmp_path\n";
my $layout_type = $ARGV[0];
my $layout_size = $ARGV[1];
my $package = $ARGV[2];
# for generic linux and NAS project, we should use different tool.
if(($package eq "generic") || ($package eq "omv")) {
    $MKUBIFS_PATH = $MKUBIFS_GENERIC_PATH;
}
my $package_path = "$cur_path/../packages/$ARGV[2]";
chomp($package_path);
my $customer_path;
my $customer_id = $ARGV[3];
my $layout_use_emmc_swap = $ARGV[4];

my $USED_FILE_CONTEXTS_PATH = "";
my $FILE_CONTEXTS_PATH ="$package_path/root/file_contexts";
if (-e $FILE_CONTEXTS_PATH)
{
    system("cp $FILE_CONTEXTS_PATH $tmp_path/file_contexts ");
    $USED_FILE_CONTEXTS_PATH = "-S $tmp_path/file_contexts";
    print "\033[0;32m Found $FILE_CONTEXTS_PATH set USED_FILE_CONTEXTS_PATH to $USED_FILE_CONTEXTS_PATH\033[m\n";
}
else 
{
### For Android N 
    $FILE_CONTEXTS_PATH ="$ANDROID_PRODUCT_OUT/root/file_contexts.bin";
    if (-e $FILE_CONTEXTS_PATH)
    {
        $MAKE_EXT4FS  = "$ANDROID_HOST_OUT/bin/make_ext4fs";
        $SIMG2IMG     = "$ANDROID_HOST_OUT/bin/simg2img";
        $E2FSCK_PATH  = "$ANDROID_HOST_OUT/bin/e2fsck";
        $RESIZE2FS_PATH = "$ANDROID_HOST_OUT/bin/resize2fs";
        #$MKSQUASHFS   = "$ANDROID_HOST_OUT/bin/mksquashfs";
        system("cp $FILE_CONTEXTS_PATH $tmp_path/file_contexts.bin ");
        $USED_FILE_CONTEXTS_PATH = "-S $tmp_path/file_contexts.bin";
        print "\033[0;32m Using 7.0  file_context \033[m\n";
    }
    else
    {
        print "\033[0;32mCan't found $FILE_CONTEXTS_PATH set to $USED_FILE_CONTEXTS_PATH \033[m\n";
    }
}


my $partition_count;
my @partitions_by_row;
my @partitions_labels;       # partition label
my %partitions_by_labels;   # partition content by label
my $nand_page_size;
my $nand_block_size;
my $nand_leb_size;   # for ubifs only

### subroutine

sub get_partition_count
{
    ### get partition parameters
    #my $cmd1 = "cat $package_path"."/customer/rtk_generic/partition.txt | grep -e '^part' | wc -l";
    #print "cmd1=$cmd1\n";
    if($package eq "omv")
    {
        $partition_count = `cat $tmp_path/pkgfile/config.txt | grep -e \"^part\" | wc -l`;
    }
    else
    {
        my $partition_num;
        if( $layout_use_emmc_swap eq "emmc_swap_true" ) {
            $partition_num = `cat $package_path/$customer_path/partition.emmc_swap_700MB.txt | grep -e \"^part\" | wc -l`;
        }
        else {
            $partition_num = `cat $package_path/$customer_path/partition.txt | grep -e \"^part\" | wc -l`;
        }        
        print "partition_num: $partition_num";  # total partition count
        my $partition_num_cust = `cat $package_path/$customer_path/partition_customize.txt | grep -e \"^part\" | wc -l`;
        print "partition_num_cust: $partition_num_cust";  # total partition customize count
        $partition_count = $partition_num + $partition_num_cust; 
    }
}

sub get_partition_detail_info
{
    my $partitions_info_tmp;
    if($package eq "omv")
    {
        $partitions_info_tmp = `cat $tmp_path/pkgfile/config.txt | grep -e \"^part\"`;
    }
    else
    {
        if( $layout_use_emmc_swap eq "emmc_swap_true" ) {
            $partitions_info_tmp = `cat $package_path/$customer_path/partition.emmc_swap_700MB.txt | grep -e \"^part\";cat $package_path/$customer_path/partition_customize.txt | grep -e \"^part\"`;
        }
        else {
        	$partitions_info_tmp = `cat $package_path/$customer_path/partition.txt | grep -e \"^part\";cat $package_path/$customer_path/partition_customize.txt | grep -e \"^part\"`;
        }
    }
    #print $partitions_info_tmp; # partition content
    
    my @partitions = split('\n', $partitions_info_tmp);
    #print $#partitions."\n";
    
    my $tmp_cnt = 0;
    for($j = 0; $j <= $#partitions; $j++) {
        #print $j.": ".$partitions[$j]."\t";
    
        @ttt = split /\s/, $partitions[$j];
    
        # 2: label name
        # 3: mount point
        # 4: type
        # 5: image name
        # 6: size
        #print "\n___".$#ttt." ".$ttt[6]."\n";
        for($z = 0; $z <= $#ttt; $z++) {
            $partitions_by_row[$tmp_cnt][$z] = $ttt[$z];
            if(2 == $z)  # label
            {
                $partitions_labels[$tmp_cnt] = $ttt[$z];
            }
        }
        $tmp_cnt++;
    }

    #print "\n===>$partitions_by_row[0][6]\n";
    #print "\n===>$partitions_by_row[1][6]\n";
    #print "\n===>$partitions_by_row[2][6]\n";
}

#0: part = system /system ubifs package5/ubi_system.img 734003200
#1: part = data /data ubifs package5/ubi_data.img 6710886400
#2: part = cache /cache ubifs package5/ubi_cache.img 314572800
sub rearrange_partition_detail_info
{
    for($i = 0; $i < $partition_count; $i++) 
    {
        my $label = $partitions_labels[$i];
        #for($j = 2; $j < 7; $j++) 
        #{
            $partitions_by_labels{$label}{"label"} = $partitions_by_row[$i][2];
            $partitions_by_labels{$label}{"mount_point"} = $partitions_by_row[$i][3];
            $partitions_by_labels{$label}{"type"} = $partitions_by_row[$i][4];
            $partitions_by_labels{$label}{"image_name"} = $partitions_by_row[$i][5];
            $partitions_by_labels{$label}{"image_size"} = $partitions_by_row[$i][6];
            
            # calculate leb count
            if($layout_type eq "nand") 
            {
                $partitions_by_labels{$label}{"leb_count"} = ($partitions_by_labels{$label}{"image_size"} / $nand_block_size) - 8;
            } 
            #$partitions_by_labels{$label}[$j] = $partitions_by_row[$i][$j];
        #}
    }
    #print $partitions_by_labels{'system'}{'label'}."\n";
    #print "$_ $partitions_by_labels{$_}{'mount_point'}\n" for (keys %partitions_by_labels);
    #print $partitions_by_labels{'system'}{'image_name'}."\n";
    #print $partitions_by_labels{'data'}{'image_name'}."\n";
    #print $partitions_by_labels{'cache'}{'image_name'}."\n";
}

sub get_nand_flash_parameters
{
    # get nand page size
    my $nand_page_size_str = `cat $package_path/Makefile.in | grep -e \"^NAND_PAGE_SIZE\"`;
    my @out = split /[= ]/, $nand_page_size_str;
    #print "\nnand_page_size=$out[1]\n";
    $nand_page_size = $out[1];
    chomp($nand_page_size);

    # get nand block size
    $nand_block_size_str = `cat $package_path/Makefile.in | grep -e \"^NAND_BLOCK_SIZE\"`;
    @out = split /[= ]/, $nand_block_size_str;
    #print "\nnand_block_size=$out[1]\n";
    $nand_block_size = $out[1];
    chomp($nand_block_size);

    $nand_leb_size = $nand_block_size - ($nand_page_size * 2);
    #print "\nnand_leb_size=$nand_leb_size\n";
    chomp($nand_leb_size);
}

sub generate_ubifs_ini
{
    #[system-volume]
    #mode=ubi
    #image=system.ubifs.img
    #vol_id=0
    #vol_type=dynamic
    #vol_name=system
    #vol_alignment=1
    #vol_flags=autoresize

    my $label = $_[0];

    my $volume_name = "[".$label."-volume]";
    my $image_name = $label."\.ubifs.img";

    my $pattern = "$volume_name\n".
                  "mode=ubi\n".
                  "image=$image_name\n".
                  "vol_id=0\n".
                  "vol_type=dynamic\n".
                  "vol_name=$label\n".
                  "vol_alignment=1\n".
                  "vol_flags=autoresize\n";

    print "=> => => gen_ini: $pattern\n";
    my $temp_ubifs_ini = $label.".ini";  # TODO:// should generate automatically???
    open(OUT_FILE, '>', "$package_path/$temp_ubifs_ini") || die "Can't open and write to $temp_ubifs_ini file!\n";
    print OUT_FILE $pattern;
    close(OUT_FILE);    
}

sub make_ubifs_image
{
    $my_count = $_[0]; # partition count

    for($i = 0; $i < $my_count; $i++) 
    {
        # making ubifs image
        my $label_name = $partitions_labels[$i];
        my $temp_ubifs_img = $label_name.".ubifs.img";

        next if($partitions_by_labels{$label_name}{"type"} ne "ubifs");
        # The NAS etc ubifs partition is handled in mkrootfs
        next if(($package eq "omv") && ($label_name eq "etc"));

        if(($package eq "omv") && ($label_name eq "rootfs"))
        {
            generate_ubifs_ini("rootfs");
            system("fakeroot $package_path/mkrootfs.sh $layout_type");
            next;
        }

        #my $cmd_string = "$MKUBIFS_PATH -v -m $nand_page_size -e $nand_leb_size -c $partitions_by_labels{$label_name}{\"leb_count\"} -r $package_path/$partitions_by_labels{$label_name}{\"label\"} $temp_ubifs_img"; 
        #print "<<<<<<$cmd_string\n";
        if($package eq "generic")
        {
            system("fakeroot $MKUBIFS_PATH -v -m $nand_page_size -e $nand_leb_size -c $partitions_by_labels{$label_name}{\"leb_count\"} -r $package_path/$partitions_by_labels{$label_name}{\"label\"} $temp_ubifs_img");
        }
        else
        {
            system("$MKUBIFS_PATH -v -m $nand_page_size -e $nand_leb_size -c $partitions_by_labels{$label_name}{\"leb_count\"} -r $package_path/$partitions_by_labels{$label_name}{\"label\"} $temp_ubifs_img");
        }

        # making ubi image
        my $temp_ubifs_ini = $label_name.".ini";  # TODO:// should generate automatically???
        generate_ubifs_ini($label_name);
        my ($path, $ubi_img) = split /\//, $partitions_by_labels{$label_name}{"image_name"};
        #$cmd_string = "$UBINIZE_PATH -v -o $ubi_img -m $nand_page_size -p $nand_block_size $package_path/$temp_ubifs_ini"; 
        #print "<<<<<<$cmd_string\n";

        system("$UBINIZE_PATH -v -o $ubi_img -m $nand_page_size -p $nand_block_size $package_path/$temp_ubifs_ini"); 

        # copy
        copy_binary_to_target($ubi_img, "$tmp_path/pkgfile/$package/");
    }

}

sub make_ext4_image
{
    $my_count = $_[0]; # partition count

    for($i = 0; $i < $my_count; $i++) 
    {
        my $label_name = $partitions_labels[$i];

        next if($partitions_by_labels{$label_name}{"type"} ne "ext4");
        # The NAS etc ext4 partition is handled in mkrootfs
        next if(($package eq "omv") && ($label_name eq "etc"));

        if(($package eq "omv") && ($label_name eq "rootfs"))
        {
            system("fakeroot $package_path/mkrootfs.sh $layout_type");
            next;
        }

        print "\n<<<<<<label_name: $label_name\n";
#        system("rm -rf $(CURDIR)/$(EXT4FS_SYSTEMIMG); sync"); 
        if (($label_name eq "uboot") || ($label_name eq "logo") || ($label_name eq "verify") || ($label_name eq "swap") || ($label_name eq "blank") || ($label_name eq "blank2"))
	{
	    my ($path, $ext4_img) = split /\//, $partitions_by_labels{$label_name}{"image_name"};
            my $size_in_blocks = 1024 / 512; #default size 1KB
            system("dd if=/dev/zero of=$ext4_img bs=512 count=$size_in_blocks");
            # copy
            copy_binary_to_target($ext4_img, "$tmp_path/pkgfile/$package/");

	}
	elsif (($label_name eq "rootfs") && ($package eq "generic"))
	{
	    print "\ngLinux rootfs done.\n";
	}
	else
	{

        my $size_in_mb = $partitions_by_labels{$label_name}{"image_size"} / 1024 / 1024;
	if ($label_name eq "backup")
	{
	    system("$MAKE_EXT4FS -l $size_in_mb"."M -L $label_name -a $label_name $tmp_path/s.bin $package_path/$partitions_by_labels{$label_name}{\"label\"}; sync;");
	}
	elsif (($package eq "omv") && ($label_name eq "xen"))
	{
	    system("$MAKE_EXT4FS -l $size_in_mb"."M -L $label_name $tmp_path/s.bin $package_path/$partitions_by_labels{$label_name}{\"label\"}; sync;");
	}
	else
	{       
	    system("$MAKE_EXT4FS -l $size_in_mb"."M -L $label_name $USED_FILE_CONTEXTS_PATH -a $label_name $tmp_path/s.bin $package_path/$partitions_by_labels{$label_name}{\"label\"}; sync;") && exit 1;
	}
        my ($path, $ext4_img) = split /\//, $partitions_by_labels{$label_name}{"image_name"};
        #system("$SIMG2IMG $tmp_path/s.bin $ext4_img; sync") && exit 1; 
        system("mv -f $tmp_path/s.bin $ext4_img") && exit 1; 
        system("$E2FSCK_PATH -f -p $ext4_img; sync;") && exit 1;
        system("$RESIZE2FS_PATH -d 32 -M $ext4_img") && exit 1;

        # copy
        copy_binary_to_target($ext4_img, "$tmp_path/pkgfile/$package/");
    }
	}

}

sub make_squash_image
{
    $my_count = $_[0]; # partition count

    for($i = 0; $i < $my_count; $i++)
    {
        my $label_name = $partitions_labels[$i];

        next if($partitions_by_labels{$label_name}{"type"} ne "squashfs");

        print "\n<<<<<<squashfs: $label_name\n";

        if(($package eq "omv") && ($label_name eq "rootfs"))
        {
            if($partitions_by_labels{"etc"}{"type"} eq "ubifs")
            {
                generate_ubifs_ini("etc");
            }
            system("fakeroot $package_path/mkrootfs.sh $layout_type");
        }
    }
}

sub copy_binary_to_target
{
    my $file = $_[0];
    my $path = $_[1];
    system("cp $file $path");
}


### main function start

# determine customer folder path
if($customer_id){
    $customer_path = "customer/$customer_id";
}
elsif($layout_type eq "nand")
{
    $customer_path = "customer/rtk_generic";    
}
elsif ($layout_type eq "emmc")
{
    if ($layout_size eq "4gb")
    {
        $customer_path = "customer/rtk_generic_emmc";
    } elsif ($layout_size eq "8gb")
    {
        $customer_path = "customer/rtk_generic_emmc_8gb";    
    } elsif ($layout_size eq "16gb")
    {
        $customer_path = "customer/rtk_generic_emmc_16gb";
    } elsif ($layout_size eq "32gb")
    {
        $customer_path = "customer/rtk_generic_emmc_32gb";
    }
}
elsif ($layout_type eq "sata")
{
    $customer_path = "customer/rtk_generic_sata";    
} 
print "customer_path=$customer_path\n";

get_partition_count();
print "partition_count=".$partition_count;

if($layout_type eq "nand")
{
    get_nand_flash_parameters();
    print "nand_page_size=$nand_page_size\n";
    print "nand_block_size=$nand_block_size\n";
    print "nand_leb_size=$nand_leb_size\n";
}

get_partition_detail_info();
rearrange_partition_detail_info();  # to calcuate leb count inside for nand case.

if($layout_type eq "nand")
{
    make_squash_image($partition_count);
    make_ubifs_image($partition_count);
} elsif ($layout_type eq "emmc") 
{
    make_squash_image($partition_count);
    make_ext4_image($partition_count);
} elsif ($layout_type eq "sata") 
{
    make_squash_image($partition_count);
    make_ext4_image($partition_count);
} else {
    print "invalid layout_type, no image created!\n";
}

#copy_binary_to_target();

### execute command
#my $cmd;
#for($i = 0; $i < $cnt; $i++) {
#    $cmd .= $ARGV[$i];
#    $cmd .= " ";
#}
#print $cmd."\n";

#exec($cmd);
