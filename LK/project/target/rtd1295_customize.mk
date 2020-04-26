CUSTOMIZE_SETTING ?=

ifneq ($(CUSTOMIZE_SETTING),)
MODULES += \
	app/rtkboot/customize
endif
