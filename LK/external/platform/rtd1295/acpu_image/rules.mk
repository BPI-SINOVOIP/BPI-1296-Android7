LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)
ACPU_SRC := $(LOCAL_DIR)

MODULE_SRCS += \
	$(LOCAL_DIR)/bootload.c \

EXTRA_LINKER_SCRIPTS += $(LOCAL_DIR)/acpu_boot.ld
EXTRA_OBJS += $(BUILDDIR)/acpu_imgs/acpu_img.o

include make/module.mk

$(EXTRA_OBJS)::	$(BUILDDIR)/$(ACPU_SRC)/bootload.o
	mkdir -p $(BUILDDIR)/acpu_imgs
	$(OBJCOPY) -j .dummy $(BUILDDIR)/$(ACPU_SRC)/bootload.o $(BUILDDIR)/acpu_imgs/acpu_img.o
	$(OBJCOPY) --add-section .exc_redirect=$(ACPU_SRC)/exc_redirect.img $(BUILDDIR)/acpu_imgs/acpu_img.o
	$(OBJCOPY) --add-section .exc_dispatch=$(ACPU_SRC)/exc_dispatch.img $(BUILDDIR)/acpu_imgs/acpu_img.o
	$(OBJCOPY) --add-section .isrvideoimg=$(ACPU_SRC)/isr_video.img $(BUILDDIR)/acpu_imgs/acpu_img.o
	$(OBJCOPY) --add-section .rosbootvectorimg=$(ACPU_SRC)/ros_bootvector.img $(BUILDDIR)/acpu_imgs/acpu_img.o
	$(OBJCOPY) --add-section .a_entry=$(ACPU_SRC)/a_entry.img $(BUILDDIR)/acpu_imgs/acpu_img.o
