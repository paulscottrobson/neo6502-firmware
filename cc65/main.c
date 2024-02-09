#include <stdio.h>
#include <stdint.h>

unsigned char *cmd = (unsigned char *)0xFF00;

int write (unsigned char* buf, unsigned count) {
 	while (count--) {
 		while(cmd[0]) {}	
	 	cmd[1] = 0;
 		cmd[4] = *buf++;
 		cmd[0] = 2;
 	}
}

void drawLine(int x1,int y1,int x2,int y2) {
 	while(cmd[0]) {}						// Waits for the previous command to finish.
 	cmd[4] = x1 & 0xFF;  					// P0P1
 	cmd[5] = x1 >> 8;
 	cmd[6] = y1 & 0xFF; 					// P2P3
 	cmd[7] = y1 >> 8;
 	cmd[8] = x2 & 0xFF;  					// P4P5
 	cmd[9] = x2 >> 8;
 	cmd[10] = y2 & 0xFF;  					// P6P7
 	cmd[11] = y2 >> 8;

 	cmd[1] = 2;  							// Command 2, draw line
 	cmd[0] = 5;  							// In group 5 - this write triggers the action.
 	while(cmd[0]) {}						// Wait for the command to finish (not strictly required)
}

int main() {
	write("hello world\n",12);
	write("hello world\n",12);
	drawLine(10,10,100,100);
	return 0;
}