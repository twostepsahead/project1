#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.
# Copyright 2012 Nexenta Systems, Inc. All rights reserved.
# Copyright 2014 Garrett D'Amore <garrett@damore.org>
# Copyright 2017, Joyent, Inc.
#

#
# The default for payload-bearing actions in this package is to appear in the
# global zone only.  See the include file for greater detail, as well as
# information about overriding the defaults.
#
dir path=etc group=sys
dir path=etc/usb group=sys
dir path=kernel group=sys
dir path=kernel/dacf group=sys
dir path=kernel/drv group=sys
dir path=kernel/kmdb group=sys
dir path=kernel/misc group=sys
dir path=kernel/strmod group=sys
dir path=lib
dir path=lib/svc
dir path=lib/svc/manifest group=sys
dir path=lib/svc/manifest/system group=sys
dir path=lib/svc/method
dir path=sbin group=sys
dir path=usr group=sys
dir path=usr/lib
dir path=usr/lib/mdb group=sys
dir path=usr/lib/mdb/kvm group=sys
dir path=usr/lib/xhci group=sys
dir path=usr/share
dir path=usr/share/man
dir path=usr/share/man/man7m
dir path=usr/share/man/man9e
dir path=usr/share/man/man9f
dir path=usr/share/man/man9s
driver name=ehci alias=pciclass,0c0320 perms="* 0644 root sys"
driver name=hid alias=usbif,class3 perms="* 0600 root sys"
driver name=hubd alias=usbif,class9 perms="* 0644 root sys"
driver name=ohci alias=pciclass,0c0310 perms="* 0644 root sys"
driver name=scsa2usb \
    alias=usb584,222 \
    alias=usbif,class8
driver name=uhci alias=pciclass,0c0300 perms="* 0644 root sys"
driver name=usb_ac alias=usbif,class1.1 perms="* 0600 root sys"
driver name=usb_as alias=usbif,class1.2 perms="* 0600 root sys"
driver name=usb_ia alias=usb,ia
driver name=usb_mid alias=usb,device
driver name=usbprn alias=usbif,class7.1 perms="* 0666 root sys"
driver name=xhci alias=pciclass,0c0330 perms="* 0644 root sys"
file path=etc/usb/config_map.conf group=sys preserve=true
file path=kernel/drv/ehci group=sys
file path=kernel/drv/ehci.conf group=sys
file path=kernel/drv/hid group=sys
file path=kernel/drv/hubd group=sys
file path=kernel/drv/ohci group=sys
file path=kernel/drv/ohci.conf group=sys
file path=kernel/drv/scsa2usb group=sys
file path=kernel/drv/scsa2usb.conf group=sys preserve=true
file path=kernel/drv/uhci group=sys
file path=kernel/drv/uhci.conf group=sys
file path=kernel/drv/usb_ac group=sys
file path=kernel/drv/usb_ac.conf group=sys
file path=kernel/drv/usb_as group=sys
file path=kernel/drv/usb_ia group=sys
file path=kernel/drv/usb_mid group=sys
file path=kernel/drv/usbprn group=sys
file path=kernel/drv/xhci group=sys
file path=kernel/drv/xhci.conf group=sys
file path=kernel/kmdb/xhci group=sys mode=0555
file path=kernel/misc/hidparser group=sys mode=0755
file path=kernel/misc/usba group=sys mode=0755
file path=kernel/misc/usba10 group=sys mode=0755
file path=kernel/strmod/usb_ah group=sys mode=0755
file path=kernel/strmod/usbkbm group=sys mode=0755
file path=kernel/strmod/usbms group=sys mode=0755
file path=kernel/strmod/usbwcm group=sys mode=0755
file path=usr/lib/mdb/kvm/$(ARCH64)/xhci.so group=sys mode=0555
file path=usr/lib/xhci/xhci_portsc group=sys mode=0555
file path=usr/share/man/man7d/ehci.7d
file path=usr/share/man/man7d/hid.7d
file path=usr/share/man/man7d/hubd.7d
file path=usr/share/man/man7d/ohci.7d
file path=usr/share/man/man7d/scsa2usb.7d
file path=usr/share/man/man7d/uhci.7d
file path=usr/share/man/man7d/usb_ac.7d
file path=usr/share/man/man7d/usb_as.7d
file path=usr/share/man/man7d/usb_ia.7d
file path=usr/share/man/man7d/usb_mid.7d
file path=usr/share/man/man7d/usba.7d
file path=usr/share/man/man7d/usbprn.7d
file path=usr/share/man/man7d/xhci.7d
file path=usr/share/man/man7m/usb_ah.7m
file path=usr/share/man/man7m/usbkbm.7m
file path=usr/share/man/man7m/usbms.7m
file path=usr/share/man/man9e/usba_hcdi.9e
file path=usr/share/man/man9e/usba_hcdi_cb_ops.9e
file path=usr/share/man/man9e/usba_hcdi_device_address.9e
file path=usr/share/man/man9e/usba_hcdi_device_init.9e
file path=usr/share/man/man9e/usba_hcdi_hub_update.9e
file path=usr/share/man/man9e/usba_hcdi_pipe_bulk_xfer.9e
file path=usr/share/man/man9e/usba_hcdi_pipe_ctrl_xfer.9e
file path=usr/share/man/man9e/usba_hcdi_pipe_intr_xfer.9e
file path=usr/share/man/man9e/usba_hcdi_pipe_isoc_xfer.9e
file path=usr/share/man/man9e/usba_hcdi_pipe_open.9e
file path=usr/share/man/man9e/usba_hcdi_pipe_reset.9e
file path=usr/share/man/man9e/usba_hcdi_pipe_stop_intr_polling.9e
file path=usr/share/man/man9f/usb_alloc_request.9f
file path=usr/share/man/man9f/usb_client_attach.9f
file path=usr/share/man/man9f/usb_clr_feature.9f
file path=usr/share/man/man9f/usb_create_pm_components.9f
file path=usr/share/man/man9f/usb_ep_xdescr_fill.9f
file path=usr/share/man/man9f/usb_get_addr.9f
file path=usr/share/man/man9f/usb_get_alt_if.9f
file path=usr/share/man/man9f/usb_get_cfg.9f
file path=usr/share/man/man9f/usb_get_current_frame_number.9f
file path=usr/share/man/man9f/usb_get_dev_data.9f
file path=usr/share/man/man9f/usb_get_max_pkts_per_isoc_request.9f
file path=usr/share/man/man9f/usb_get_status.9f
file path=usr/share/man/man9f/usb_get_string_descr.9f
file path=usr/share/man/man9f/usb_handle_remote_wakeup.9f
file path=usr/share/man/man9f/usb_lookup_ep_data.9f
file path=usr/share/man/man9f/usb_parse_data.9f
file path=usr/share/man/man9f/usb_pipe_bulk_xfer.9f
file path=usr/share/man/man9f/usb_pipe_close.9f
file path=usr/share/man/man9f/usb_pipe_ctrl_xfer.9f
file path=usr/share/man/man9f/usb_pipe_drain_reqs.9f
file path=usr/share/man/man9f/usb_pipe_get_max_bulk_transfer_size.9f
file path=usr/share/man/man9f/usb_pipe_get_state.9f
file path=usr/share/man/man9f/usb_pipe_intr_xfer.9f
file path=usr/share/man/man9f/usb_pipe_isoc_xfer.9f
file path=usr/share/man/man9f/usb_pipe_reset.9f
file path=usr/share/man/man9f/usb_pipe_set_private.9f
file path=usr/share/man/man9f/usb_pipe_xopen.9f
file path=usr/share/man/man9f/usb_register_hotplug_cbs.9f
file path=usr/share/man/man9f/usb_reset_device.9f
file path=usr/share/man/man9f/usba_alloc_hcdi_ops.9f
file path=usr/share/man/man9f/usba_hcdi_cb.9f
file path=usr/share/man/man9f/usba_hcdi_dup_intr_req.9f
file path=usr/share/man/man9f/usba_hcdi_dup_isoc_req.9f
file path=usr/share/man/man9f/usba_hcdi_get_device_private.9f
file path=usr/share/man/man9f/usba_hcdi_register.9f
file path=usr/share/man/man9f/usba_hubdi_bind_root_hub.9f
file path=usr/share/man/man9f/usba_hubdi_cb_ops.9f
file path=usr/share/man/man9f/usba_hubdi_dev_ops.9f
file path=usr/share/man/man9s/usb_bulk_req.9s
file path=usr/share/man/man9s/usb_callback_flags.9s
file path=usr/share/man/man9s/usb_cfg_descr.9s
file path=usr/share/man/man9s/usb_client_dev_data.9s
file path=usr/share/man/man9s/usb_completion_reason.9s
file path=usr/share/man/man9s/usb_ctrl_req.9s
file path=usr/share/man/man9s/usb_dev_descr.9s
file path=usr/share/man/man9s/usb_dev_qlf_descr.9s
file path=usr/share/man/man9s/usb_ep_descr.9s
file path=usr/share/man/man9s/usb_ep_ss_comp_descr.9s
file path=usr/share/man/man9s/usb_ep_xdescr.9s
file path=usr/share/man/man9s/usb_if_descr.9s
file path=usr/share/man/man9s/usb_intr_req.9s
file path=usr/share/man/man9s/usb_isoc_req.9s
file path=usr/share/man/man9s/usb_other_speed_cfg_descr.9s
file path=usr/share/man/man9s/usb_request_attributes.9s
file path=usr/share/man/man9s/usb_string_descr.9s
file path=usr/share/man/man9s/usba_device.9s
file path=usr/share/man/man9s/usba_hcdi_ops.9s
file path=usr/share/man/man9s/usba_hcdi_register_args.9s
file path=usr/share/man/man9s/usba_pipe_handle_data.9s
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
license license_in_headers license=license_in_headers
license usr/src/uts/common/sys/usb/hcd/xhci/THIRDPARTYLICENSE \
    license=usr/src/uts/common/sys/usb/hcd/xhci/THIRDPARTYLICENSE
