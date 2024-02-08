#include <stdio.h>
#include <stdint.h>

int write (unsigned char* buf, unsigned count) {
 	unsigned char *cmd = (unsigned char *)0xFF00;
 	while (count--) {
 		while(cmd[0]) {}	
	 	cmd[1] = 0;
 		cmd[4] = *buf++;
 		cmd[0] = 2;
 	}
}

int main() {
	write("hello world\n",12);
	write("hello world\n",12);
	return 0;
}