void init_monitor(int, char *[]);
void reg_test();
void restart();
void ui_mainloop();
void my_test();

int main(int argc, char *argv[]) {
	my_test();
	/* Initialize the monitor. */
	init_monitor(argc, argv);

	/* Test the implementation of the `CPU_state' structure. */
	reg_test();

	/* Initialize the virtual computer system. */
	restart();

	/* Receive commands from user. */
	ui_mainloop();

	return 0;
}
