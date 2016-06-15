/* Attempt to make sense of AMD APM support */
/* Application power management */

/* Fam 15h Model 30h (piledriver) */
/* See amd_fam15h_30_bkdg */
/* D18F5xE8 TDP Limit 3 */

/* note http://lm-sensors.lm-sensors.narkive.com/5MI2vOwZ/wrong-output-with-fam15h-power */


#include <stdio.h>
#include <string.h>

#include <cpuid.h>

int main(int argc, char **argv) {

	unsigned int eax,ebx,ecx,edx=0;
	unsigned int stepping,model,family;
	char vendor_string[13];

	/* Get CPUID leaf 0 which has name and level */
	__get_cpuid (0x0,&eax,&ebx,&ecx,&edx);
	memcpy(vendor_string,&ebx,4);
	memcpy(vendor_string+4,&edx,4);
	memcpy(vendor_string+8,&ecx,4);
	vendor_string[12]=0;

	__get_cpuid (0x1,&eax,&ebx,&ecx,&edx);
	stepping=eax&0xf;
	model=(eax>>4)&0xf;
	family=(eax>>8)&0xf;

	if (family==15) {
		family+=((eax>>20)&0xff);
	}
	if ((family==15) || (family==6)) {
		model+=(((eax>>16)&0xf)<<4);
	}

	printf("Looking for AMD APM support...\n");

	printf("\tFound family %d model %d stepping %d %s processor\n",
		family,model,stepping,vendor_string);

	if (strcmp("AuthenticAMD",vendor_string)) {
		printf("Not an AMD processor!  Exiting.\n\n");

		return 0;
	}

	if (family<0x15) {
		printf("Family %x processor too old (need at least 0x15)\n\n",
			family);

		return 0;
	}

	/* 2.5.9.1 */
	/* Support for APM is specified by CPUID Fn8000_0007_EDX[CPB]. */
	/* which is bit 9, CPU "core performance boost" */
	__get_cpuid (0x80000007,&eax,&ebx,&ecx,&edx);

	if (!(edx&(1<<9))) {
		printf("Bit 9 (CDP) not set in cpuid 80000007:edx, no APM support\n\n");
		return 0;
	}

	/* the drivers/hwmon/fam15h_power.c tweaks the value in */
	/* 00:18.5 Host bridge: Advanced Micro Devices, Inc. [AMD] Family 15h Processor Function 5 */
	/* D18F5xE0 */
	/* sudo setpci -s 18.5 0xe0.l */
	/* only low 4 bits matter?  Seem to be 0x9 on 4.7/piledriver */
	/* which should be right */



	/* APM is enabled if all of the following conditions are true: */
	/* * MSRC001_0015[CpbDis] = 0 for all cores.
	   * D18F4x15C[ApmMasterEn] = 1.
	   *  D18F4x15C[BoostSrc] = 01b.
	   * D18F4x15C[NumBoostStates] != 0.
	*/


	/* D18F5xE0 Processor TDP Running Average */
	/* Time interval = 2^(RunAvgRange + 1) * FreeRunSampleTimer rate. */
	/* A value of 0 disables the TDP running average accumulator capture function. See 2.5.9 [Application Power Management (APM)]. */

	/* D18F4x110 Sample and Residency Timers */

	/* D18F5xE8 TDP Limit 3 */
	/* 28:16 ApmTdpLimit */
	/* 9:0 Tdp2Watt fixed point 0.10 conversion factor */

	/* On our machine 029e007e 00 0111 1110 */


/* D18F3 */
#define REG_NORTHBRIDGE_CAP             0xe8

/* D18F4 */
#define REG_PROCESSOR_TDP               0x1b8

/* D18F5 */
#define REG_TDP_RUNNING_AVERAGE         0xe0
#define REG_TDP_LIMIT3                  0xe8


	return 0;

}
