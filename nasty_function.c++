
#include <cmath>

double calc(double& val1, double& val2)
{
	double J_m2 = J_m*J_m;
	double L_m2 = L_m*L_m;
	double V_t2 = V_t*V_t;
	double n_g2 = n_g*n_g;
	double n_l2 = n_l*n_l;
	double K2 = K*K;
	double R2 = R*R;

	double part1 = (
			-J_m*R*n_g +
			2*K*R*n_g +
			K*R*n_l -
			L_m*n_g -
			V_t
	);

	double part2 = std::sqrt(
			J_m2*R2*n_g2 +
			2*J_m*K*R2*n_g*n_l +
			2*J_m*L_m*R*n_g2 +
			4*J_m*L_m*R*n_g*n_l +
			4*J_m*L_m*R*n_l2 -
			2*J_m*R*V_t*n_g -
			4*J_m*R*V_t*n_l +
			K2*R2*n_l2 -
			2*K*L_m*R*n_g*n_l -
			4*K*L_m*R*n_l2 +
			2*K*R*V_t*n_l +
			L_m2*n_g2 +
			4*L_m2*n_g*n_l +
			4*L_m2*n_l2 -
			2*L_m*V_t*n_g -
			4*L_m*V_t*n_l +
			V_t2
	);

	double part3 = (2*(
			J_m*R -
			K*R +
			L_m
	));
	
	val1 = (part1 - part2) / part3;
	val2 = (part1 + part2) / part3;
}

