#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

OPENWRT_GIT = http://git.openwrt.org

DOWNLOAD_RDEP=$(STAMP_PREPARED) $(HOST_STAMP_PREPARED)

# Try to guess the download method from the URL
define dl_method
$(strip \
  $(if $(2),$(2), \
    $(if $(filter @GNOME/% @GNU/% @KERNEL/% @SF/% @SAVANNAH/% ftp://% http://% https://% file://%,$(1)),default, \
      $(if $(filter git://%,$(1)),git, \
        $(if $(filter svn://%,$(1)),svn, \
          $(if $(filter cvs://%,$(1)),cvs, \
            $(if $(filter hg://%,$(1)),hg, \
              $(if $(filter sftp://%,$(1)),bzr, \
                unknown \
              ) \
            ) \
          ) \
        ) \
      ) \
    ) \
  ) \
)
endef

TIMESTAMP="2017-11-16 11:31:59 +0800"
# code for creating tarballs from cvs/svn/git/bzr/hg/darcs checkouts - useful for mirror support
dl_pack/bz2=$(TAR) --format=gnu --sort=name -c --mtime=$(TIMESTAMP) $(2) | bzip2 -9 > $(1)
dl_pack/gz=$(TAR) --format=gnu --sort=name -cf - --mtime=$(TIMESTAMP) $(2) | gzip -n -9 - > $(1)
dl_pack/xz=$(TAR) --format=gnu --sort=name -c --mtime=$(TIMESTAMP) $(2) | xz --check=crc32 --lzma2=dict=8MiB -zc > $(1)
dl_pack/unknown=echo "ERROR: Unknown pack format for file $(1)"; false
define dl_pack
	$(if $(dl_pack/$(call ext,$(1))),$(dl_pack/$(call ext,$(1))),$(dl_pack/unknown))
endef

define DownloadMethod/unknown
	@echo "ERROR: No download method available"; false
endef

define DownloadMethod/wget
	wget -O "$(DL_DIR)/$(FILE)" "$(URL)"
endef

define DownloadMethod/default
	$(SCRIPT_DIR)/download.pl "$(DL_DIR)" "$(FILE)" "$(MD5SUM)" "$(URL_FILE)" $(foreach url,$(URL),"$(url)")
endef

define wrap_mirror
	$(if $(if $(MIRROR),$(filter-out x,$(MIRROR_MD5SUM))),@$(SCRIPT_DIR)/download.pl "$(DL_DIR)" "$(FILE)" "$(MIRROR_MD5SUM)" "" || ( $(1) ),$(1))
endef

define DownloadMethod/cvs
	$(call wrap_mirror, \
		echo "Checking out files from the cvs repository..."; \
		mkdir -p $(TMP_DIR)/dl && \
		cd $(TMP_DIR)/dl && \
		rm -rf $(SUBDIR) && \
		[ \! -d $(SUBDIR) ] && \
		cvs -d $(URL) export $(VERSION) $(SUBDIR) && \
		echo "Packing checkout..." && \
		$(call dl_pack,$(TMP_DIR)/dl/$(FILE),$(SUBDIR)) && \
		mv $(TMP_DIR)/dl/$(FILE) $(DL_DIR)/ && \
		rm -rf $(SUBDIR); \
	)
endef

define DownloadMethod/svn
	$(call wrap_mirror, \
		echo "Checking out files from the svn repository..."; \
		mkdir -p $(TMP_DIR)/dl && \
		cd $(TMP_DIR)/dl && \
		rm -rf $(SUBDIR) && \
		[ \! -d $(SUBDIR) ] && \
		( svn help export | grep -q trust-server-cert && \
		svn export --non-interactive --trust-server-cert -r$(VERSION) $(URL) $(SUBDIR) || \
		svn export --non-interactive -r$(VERSION) $(URL) $(SUBDIR) ) && \
		echo "Packing checkout..." && \
		$(call dl_pack,$(TMP_DIR)/dl/$(FILE),$(SUBDIR)) && \
		mv $(TMP_DIR)/dl/$(FILE) $(DL_DIR)/ && \
		rm -rf $(SUBDIR); \
	)
endef

define DownloadMethod/git
	$(call wrap_mirror, \
		echo "Checking out files from the git repository..."; \
		mkdir -p $(TMP_DIR)/dl && \
		cd $(TMP_DIR)/dl && \
		rm -rf $(SUBDIR) && \
		[ \! -d $(SUBDIR) ] && \
		git clone $(if $(BRANCH),--branch $(BRANCH)) $(URL) $(SUBDIR) && \
		(cd $(SUBDIR) && git checkout $(VERSION) && \
		git submodule update --init --recursive) && \
		echo "Packing checkout..." && \
		(if [ -z $(VCSKEEP) ]; then \
		cd $(SUBDIR) && \
		git rev-parse --verify --short=8 $(VERSION) 2>/dev/null > .rtkver; \
		git describe --exact-match 2>/dev/null \
		    || git describe $(VERSION) 2>/dev/null >> .rtkver; \
		rm -rf .git; \
		fi) && \
		$(call dl_pack,$(TMP_DIR)/dl/$(FILE),$(SUBDIR)) && \
		mv $(TMP_DIR)/dl/$(FILE) $(DL_DIR)/ && \
		rm -rf $(SUBDIR); \
	)
endef

define DownloadMethod/bzr
	$(call wrap_mirror, \
		echo "Checking out files from the bzr repository..."; \
		mkdir -p $(TMP_DIR)/dl && \
		cd $(TMP_DIR)/dl && \
		rm -rf $(SUBDIR) && \
		[ \! -d $(SUBDIR) ] && \
		bzr export -r$(VERSION) $(SUBDIR) $(URL) && \
		echo "Packing checkout..." && \
		$(call dl_pack,$(TMP_DIR)/dl/$(FILE),$(SUBDIR)) && \
		mv $(TMP_DIR)/dl/$(FILE) $(DL_DIR)/ && \
		rm -rf $(SUBDIR); \
	)
endef

define DownloadMethod/hg
	$(call wrap_mirror, \
		echo "Checking out files from the hg repository..."; \
		mkdir -p $(TMP_DIR)/dl && \
		cd $(TMP_DIR)/dl && \
		rm -rf $(SUBDIR) && \
		[ \! -d $(SUBDIR) ] && \
		hg clone -r $(VERSION) $(URL) $(SUBDIR) && \
		find $(SUBDIR) -name .hg | xargs rm -rf && \
		echo "Packing checkout..." && \
		$(call dl_pack,$(TMP_DIR)/dl/$(FILE),$(SUBDIR)) && \
		mv $(TMP_DIR)/dl/$(FILE) $(DL_DIR)/ && \
		rm -rf $(SUBDIR); \
	)
endef

define DownloadMethod/darcs
	$(call wrap_mirror, \
		echo "Checking out files from the darcs repository..."; \
		mkdir -p $(TMP_DIR)/dl && \
		cd $(TMP_DIR)/dl && \
		rm -rf $(SUBDIR) && \
		[ \! -d $(SUBDIR) ] && \
		darcs get -t $(VERSION) $(URL) $(SUBDIR) && \
		find $(SUBDIR) -name _darcs | xargs rm -rf && \
		echo "Packing checkout..." && \
		$(call dl_pack,$(TMP_DIR)/dl/$(FILE),$(SUBDIR)) && \
		mv $(TMP_DIR)/dl/$(FILE) $(DL_DIR)/ && \
		rm -rf $(SUBDIR); \
	)
endef

Validate/cvs=VERSION SUBDIR
Validate/svn=VERSION SUBDIR
Validate/git=VERSION SUBDIR
Validate/bzr=VERSION SUBDIR
Validate/hg=VERSION SUBDIR
Validate/darcs=VERSION SUBDIR

define Download/Defaults
  URL:=
  FILE:=
  URL_FILE:=
  PROTO:=
  MD5SUM:=
  SUBDIR:=
  MIRROR:=1
  MIRROR_MD5SUM:=x
  VERSION:=
  USERNAME:=$(shell echo $(CONFIG_GIT_REPO_USERNAME))
endef

define Download/rtk
  ifeq ($(PROTO),git)
  URL:=$(subst @DHCGERRIT,ssh://$(if $(USERNAME),$(USERNAME)@)dhcgerrit.realtek.com.tw:20002,$(subst @SD10GERRIT,ssh://$(if $(USERNAME),$(USERNAME)@)cn1sd10gerrit1.realtek.com.tw:20009,$(URL)))
  endif
endef

define rtkheadversion
	PKG_REMOTE_VER:=$(shell \
		git ls-remote $(URL) refs/heads/$(if $(BRANCH),$(BRANCH),master) | cut -c 1-8 2>/dev/null; \
	)
endef

define Download/Head
  $(eval $(rtkheadversion))
  $(shell \
	if [ -f $(DL_DIR)/$(FILE) ]; then \
		if [ -n "$(PKG_REMOTE_VER)" ]; then \
			[ "$(PKG_REMOTE_VER)" != "`tar -xO -f $(DL_DIR)/$(FILE) $(SUBDIR)/.rtkver 2>/dev/null | head -n1`" ] && rm -rf $(DL_DIR)/$(FILE); \
		fi; \
	fi; \
  )
endef

define Download
  $(eval $(Download/Defaults))
  $(eval $(Download/$(1)))
  $(eval $(Download/rtk))
  $(if $(VCSKEEP),,$(if $(findstring HEAD,$(VERSION)),$(call Download/Head)))
  $(foreach FIELD,URL FILE $(Validate/$(call dl_method,$(URL),$(PROTO))),
    ifeq ($($(FIELD)),)
      $$(error Download/$(1) is missing the $(FIELD) field.)
    endif
  )

  $(foreach dep,$(DOWNLOAD_RDEP),
    $(dep): $(DL_DIR)/$(FILE)
  )
  download: $(DL_DIR)/$(FILE)

  $(DL_DIR)/$(FILE):
	mkdir -p $(DL_DIR)
	$(call locked,$(if $(DownloadMethod/$(call dl_method,$(URL),$(PROTO))),$(DownloadMethod/$(call dl_method,$(URL),$(PROTO))),$(DownloadMethod/unknown)),$(FILE))

endef
