
#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"

#define MAX_STR 255
#define DATA_LEN 15

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[65];
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;
	char message[25];
	int row;
	short x[DATA_LEN];
	short y[DATA_LEN]; 
	short z[DATA_LEN]; 

	// Initialize the hidapi library
	res = hid_init();

	printf("Enter the row number you would like:  ");
	scanf("%d", &row);

	printf("Enter the string you would like:  ");
	scanf("%s", message);

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hid_open(0x4d8, 0x3f, NULL);

	// Read the Manufacturer String
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"Manufacturer String: %s\n", wstr);

	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %s\n", wstr);

	// Read the Serial Number String
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

	// Read Indexed String 1
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	wprintf(L"Indexed String 1: %s\n", wstr);

	// Toggle LED (cmd 0x80). The first byte is the report number (0x0).
	buf[0] = 0x0;
	buf[1] = 0x80;
	buf[2] = row; 
	for (i=0; i < 25; i ++) {
		buf[i+3] = message[i];
	}
	res = hid_write(handle, buf, 65); 

	for (i=0; i < DATA_LEN; i ++)  {

		// Request state (cmd 0x81). The first byte is the report number (0x0).
		buf[0] = 0x0;
		buf[1] = 0x81;
		res = hid_write(handle, buf, 65);

		// Read requested state
		res = hid_read(handle, buf, 65);

		if (buf[0]) {
			x[i] = (buf[1] << 8 )| buf[2];
			y[i] = (buf[3] << 8 )| buf[4];
			z[i] = (buf[5] << 8 )| buf[6];

			printf("Read Number:  %d\n", i + 1);
			printf("X acceleration: %d\n", x[i]);
			printf("Y acceleration: %d\n", y[i]);
			printf("Z acceleration: %d\n", z[i]);
			 
		}
	}

	// Finalize the hidapi library
	res = hid_exit();

	FILE *ofp;

	ofp = fopen("accels.txt", "w");

	for (i=0; i<DATA_LEN; i++) {

		fprintf(ofp,"%d %d %d\r\n",x[i],y[i],z[i]);
	
	}

	fclose(ofp);

	return 0;
}
