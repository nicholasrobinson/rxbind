/*
 * Copyright (c) 2017, Kalopa Research. All rights reserved.
 *
 * For bugs or info, contact Dermot Tynan <dtynan@kalopa.com>
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * It is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details (in the "LICENSE" file).
 *
 * You should have received a copy of the GNU General Public License
 * along with this product; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139,
 * USA.
 *
 * THIS SOFTWARE IS PROVIDED BY KALOPA RESEARCH "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL KALOPA RESEARCH BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pigpio.h>
 
/*
 * Set it up in DSMX mode (internal, 11ms).
 */
#define NPULSES		9

void	delay(int);
 
/*
 * Program a Spektrum DSMX Remote Receiver SPM9645 so it binds with the
 * transmitter.
 */
int
main(int argc, char *argv[])
{
	int i;

	/*
	 * Set GPIO modes
	 */
	if (gpioInitialise() < 0) {
		fprintf(stderr, "?Error - cannot initialize the GPIO\n");
		exit(1);
	}
	gpioSetMode(2, PI_OUTPUT);
	gpioSetMode(3, PI_INPUT);
	/*
	 * Give the operator a chance to power up the device.
	 */
	printf("Waiting for device power...\n");
	gpioWrite(2, 1);
	while (gpioRead(3) == 0)
		;
	/*
	 * Wait for it to settle and toggle the bit.
	 */
	delay(70000);
	printf("Begin!\n");
	for (i = 0; i < NPULSES; i++) {
		delay(50);
		gpioWrite(2, 0);
		delay(50);
		gpioWrite(2, 1);
	}
	printf("Receiver in BIND mode.\n");
	gpioSetMode(2, PI_INPUT);
	gpioTerminate();
	exit(0);
}

/*
 * Delay a specific number of microseconds, with a loop.
 */
void
delay(int usecs)
{
	long cycles = 0;
	struct timeval start, tval;

	/*
	 * This is not pretty, but it works...
	 */
	gettimeofday(&start, NULL);
	while(1) {
		gettimeofday(&tval, NULL);
		tval.tv_usec -= start.tv_usec;
		if (tval.tv_usec < 0)
			tval.tv_usec += 1000000;
		if (tval.tv_usec > usecs)
			break;
		cycles++;
	}
}
