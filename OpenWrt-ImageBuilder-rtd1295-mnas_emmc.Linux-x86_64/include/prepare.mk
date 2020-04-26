prepare: staging-install

staging-install:
	@( \
	mkdir -p staging_dir; \
	for part in toolchain; do \
	    [ -d staging_dir/$$part* ] || \
	    (cd staging_dir; tar --checkpoint=6000 -xf ../prebuilt/staging_$$part.txz;) \
	done; \
	)
