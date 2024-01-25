unsigned char *cmd;

int main() {
	cmd = 0xFF00;
	cmd[1] = 0;
	cmd[4] = 42;
	cmd[0] = 2;
	return 0;
}