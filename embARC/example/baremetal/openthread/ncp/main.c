/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */

#include "embARC.h"
#include "embARC_debug.h"

#include <openthread/config.h>

#include <assert.h>

#include <openthread/diag.h>
#include <openthread/ncp.h>
#include <openthread/openthread.h>
#include <openthread/platform/platform.h>

#define NUM_INPUT_MANUAL (-1)
#define NUM_NCP          (3)

#if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES
void *otPlatCAlloc(size_t aNum, size_t aSize)
{
	return calloc(aNum, aSize);
}

void otPlatFree(void *aPtr)
{
	free(aPtr);
}
#endif

void otTaskletsSignalPending(otInstance *aInstance)
{
	(void)aInstance;
}

int main(void)
{
	otInstance *sInstance;

#if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES
	size_t otInstanceBufferLength = 0;
	uint8_t *otInstanceBuffer = NULL;
#endif
	int argc = 0;
	char **argv = NULL;

	PlatformInit(argc, argv, NUM_INPUT_MANUAL);

#if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES
	// Call to query the buffer size
	(void)otInstanceInit(NULL, &otInstanceBufferLength);

	// Call to allocate the buffer
	otInstanceBuffer = (uint8_t *)malloc(otInstanceBufferLength);
	assert(otInstanceBuffer);

	// Initialize OpenThread with the buffer
	sInstance = otInstanceInit(otInstanceBuffer, &otInstanceBufferLength);
#else
	sInstance = otInstanceInitSingle();
#endif
	assert(sInstance);

	otNcpInit(sInstance);

#if OPENTHREAD_ENABLE_DIAG
	otDiagInit(sInstance);
#endif

	while (1)
	{
		otTaskletsProcess(sInstance);
		PlatformProcessDrivers(sInstance);
	}

	// otInstanceFinalize(sInstance);
#if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES
	// free(otInstanceBuffer);
#endif

	return 0;
}

