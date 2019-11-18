/*
 * Copyright (c) 2016-2019, NVIDIA CORPORATION.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <nvgpu/gk20a.h>
#include <nvgpu/pmu/therm.h>
#include <nvgpu/clk_arb.h>

#include "thrm.h"
#include "thrmpmu.h"

int nvgpu_therm_domain_sw_setup(struct gk20a *g, struct nvgpu_pmu *pmu)
{
	int status;

	status = therm_device_sw_setup(g);
	if (status != 0) {
		nvgpu_err(g,
			"error creating boardobjgrp for therm devices, status - 0x%x",
			status);
		goto exit;
	}

	status = therm_channel_sw_setup(g);
	if (status != 0) {
		nvgpu_err(g,
			"error creating boardobjgrp for therm channel, status - 0x%x",
			status);
		goto exit;
	}

	pmu->therm_rpc_handler = nvgpu_pmu_therm_rpc_handler;

exit:
	return status;
}

int nvgpu_therm_domain_pmu_setup(struct gk20a *g, struct nvgpu_pmu *pmu)
{
	return therm_send_pmgr_tables_to_pmu(g);
}

int nvgpu_therm_pmu_init_pmupstate(struct gk20a *g, struct nvgpu_pmu *pmu)
{
	/* If already allocated, do not re-allocate */
	if (pmu->therm_pmu != NULL) {
		return 0;
	}

	pmu->therm_pmu = nvgpu_kzalloc(g, sizeof(*(pmu->therm_pmu)));
	if (pmu->therm_pmu == NULL) {
		return -ENOMEM;
	}

	return 0;
}

void nvgpu_therm_pmu_free_pmupstate(struct gk20a *g, struct nvgpu_pmu *pmu)
{
	nvgpu_kfree(g, pmu->therm_pmu);
	pmu->therm_pmu = NULL;
}
