
#define DISTRIBUCIONES_H

double get_base_uniform(void);

void gen_uniforme(double* X, int n, double min, double max);
void gen_normal(double* X, int n, double mu, double sigma, double min, double max, int recorte);
void gen_laplace(double* X, int n, double mu, double b, double min, double max, int recorte);
