/**
 * Main function for analysis 2, n=501 simulation.
 */ 
int main(int argc, char* argv[]) {
	if (argc != 5) {
		printf("Not enough arguments supplied.\n");
		return 1;
	}

	unsigned int n = 501;
	unsigned int m_x;
	unsigned int m_y;
	float alpha;
	unsigned int T;

	//sscanf(argv[1], "%d", &n);
	sscanf(argv[2], "%d", &m_x);
	sscanf(argv[3], "%d", &m_y);
	sscanf(argv[4], "%d", &alpha);
	sscanf(argv[5], "%d", &T);
	
	simulate(T, n, n, m_x, m_y);
	

}
