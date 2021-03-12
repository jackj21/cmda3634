#include "grid.h"

/**
 * Main function that takes n, m_x, m_y, alpha, and T as
 * command line arguments to run various simulations.
 */ 
int main(int argc, char* argv[]) {
	if (argc != 6) {
		printf("Not enough arguments supplied.\n");
		return 1;
	}

	printf("Running %s...", argv[0]);

	unsigned int n = argv[1];
	unsigned int m_x = argv[2];
	unsigned int m_y = argv[3];
	float alpha = argv[4];
	unsigned int T = argv[5];
	
	simulate(T, n, n, m_x, m_y);
}
