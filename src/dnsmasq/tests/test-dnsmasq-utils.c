/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Copyright (C) 2013 Red Hat, Inc.
 *
 */

#include "nm-default.h"

#include <arpa/inet.h>

#include "dnsmasq/nm-dnsmasq-utils.h"

#include "nm-test-utils-core.h"

static void
test_address_ranges (void)
{
	NMPlatformIP4Address addr;
	char first[INET_ADDRSTRLEN];
	char last[INET_ADDRSTRLEN];
	char *error_desc = NULL;

	addr = *nmtst_platform_ip4_address ("192.168.0.1", NULL, 24);
	g_assert (nm_dnsmasq_utils_get_range (&addr, first, last, &error_desc));
	g_assert (error_desc == NULL);
	g_assert_cmpstr (first, ==, "192.168.0.10");
	g_assert_cmpstr (last, ==, "192.168.0.254");

	addr = *nmtst_platform_ip4_address ("192.168.0.99", NULL, 24);
	g_assert (nm_dnsmasq_utils_get_range (&addr, first, last, &error_desc));
	g_assert (error_desc == NULL);
	g_assert_cmpstr (first, ==, "192.168.0.108");
	g_assert_cmpstr (last, ==, "192.168.0.254");

	addr = *nmtst_platform_ip4_address ("192.168.0.254", NULL, 24);
	g_assert (nm_dnsmasq_utils_get_range (&addr, first, last, &error_desc));
	g_assert (error_desc == NULL);
	g_assert_cmpstr (first, ==, "192.168.0.1");
	g_assert_cmpstr (last, ==, "192.168.0.245");

	/* Smaller networks */
	addr = *nmtst_platform_ip4_address ("1.2.3.1", NULL, 30);
	g_assert (nm_dnsmasq_utils_get_range (&addr, first, last, &error_desc));
	g_assert (error_desc == NULL);
	g_assert_cmpstr (first, ==, "1.2.3.2");
	g_assert_cmpstr (last, ==, "1.2.3.2");

	addr = *nmtst_platform_ip4_address ("1.2.3.1", NULL, 29);
	g_assert (nm_dnsmasq_utils_get_range (&addr, first, last, &error_desc));
	g_assert (error_desc == NULL);
	g_assert_cmpstr (first, ==, "1.2.3.2");
	g_assert_cmpstr (last, ==, "1.2.3.6");

	addr = *nmtst_platform_ip4_address ("1.2.3.1", NULL, 28);
	g_assert (nm_dnsmasq_utils_get_range (&addr, first, last, &error_desc));
	g_assert (error_desc == NULL);
	g_assert_cmpstr (first, ==, "1.2.3.3");
	g_assert_cmpstr (last, ==, "1.2.3.14");

	addr = *nmtst_platform_ip4_address ("1.2.3.1", NULL, 26);
	g_assert (nm_dnsmasq_utils_get_range (&addr, first, last, &error_desc));
	g_assert (error_desc == NULL);
	g_assert_cmpstr (first, ==, "1.2.3.8");
	g_assert_cmpstr (last, ==, "1.2.3.62");

	addr = *nmtst_platform_ip4_address ("1.2.3.1", NULL, 31);
	g_assert (nm_dnsmasq_utils_get_range (&addr, first, last, &error_desc) == FALSE);
	g_assert (error_desc);
	g_free (error_desc);
}

/*****************************************************************************/

NMTST_DEFINE ();

int
main (int argc, char **argv)
{
	nmtst_init_assert_logging (&argc, &argv, "INFO", "DEFAULT");

	g_test_add_func ("/dnsmasq/address-ranges", test_address_ranges);

	return g_test_run ();
}

