SECTIONS {
    .acpu : {
	. = ALIGN(8);

        _f_exc_redirect_img = .;
        KEEP (*(.exc_redirect))
	. = ALIGN(8);
        _e_exc_redirect_img = .;

	_f_exc_dispatch_img = .;
	KEEP (*(.exc_dispatch))
	. = ALIGN(8);
	_e_exc_dispatch_img = .;

	_f_a_entry_img = .;
	KEEP (*(.a_entry))
	. = ALIGN(8);
	_e_a_entry_img = .;

	_f_isrvideo_img = .;
	KEEP (*(.isrvideoimg))
	. = ALIGN(8);
	_e_isrvideo_img = .;

	_f_rosbootvector_img = .;
	KEEP (*(.rosbootvectorimg))
	. = ALIGN(8);
	_e_rosbootvector_img = .;
    }
}
INSERT AFTER .rodata;
