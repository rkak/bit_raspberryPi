#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE	10
#define CPU_CUR_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"
#define CPU_MIN_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
#define CPU_MAX_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"
#define CPU_TEMP	 "/sys/class/thermal/thermal_zone0/temp"

#define CUR_FREQ	1
#define MIN_FREQ	2
#define MAX_FREQ	3


float readCpuTemp(void) {
	int fd;
	float temp;
	char buf[BUF_SIZE];

	// before read the cpu temperature buf initialization
	memset(buf, 0, BUF_SIZE);

	fd = open(CPU_TEMP, O_RDONLY);
	if (fd < 3) {	// 0, 1, 2 are already existed
		printf("Error : Can't open temp file\n");
		exit(0);
	}

	read(fd, buf, 5);

	temp = atoi(buf) / 1000.0;
	close(fd);
	return temp;
}

float readCpuFreq(int freq_type) {
	int fd;
	float cpu_freq;
	char buf[BUF_SIZE];

	// before read the cpu frequency buf initialization
	memset(buf, 0, BUF_SIZE);

	switch (freq_type) {
	case CUR_FREQ:
		fd = open(CPU_CUR_FREQ, O_RDONLY);
		break;
	case MIN_FREQ:
		fd = open(CPU_MIN_FREQ, O_RDONLY);
		break;
	case MAX_FREQ:
		fd = open(CPU_MAX_FREQ, O_RDONLY);
		break;
	}

	if (fd < 3) {	// 0, 1, 2 are already existed
		printf("Error : Can't open frequency file\n");
		exit(0);
	}

	read(fd, buf, 5);

	cpu_freq = atoi(buf) / 100.0;
	close(fd);
	return cpu_freq;
}

// main에서는 전체적인 큰 밑그림만 보이도록!
int main(int argc, char *argv[]) {
	float temp;
	float cpu_freq;
	char buf[BUF_SIZE];

	// repeat open read, close.
	while (1) {
		temp = readCpuTemp();
		printf("Temperature : %6.3fC\n", temp);

		cpu_freq = readCpuFreq(CUR_FREQ);
		printf("CPU current frequency : %6.3fMHz\n", cpu_freq);
		cpu_freq = readCpuFreq(MIN_FREQ);
		printf("CPU min frequency : %6.3fMHz\n", cpu_freq);
		cpu_freq = readCpuFreq(MAX_FREQ);
		printf("CPU max frequency : %6.3fMHz\n", cpu_freq);

		sleep(1);
	}

}

	