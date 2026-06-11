#define ESTADISTICAS_H

double calc_maximo(const double* Y, int n);
double calc_minimo(const double* Y, int n);
double calc_media_aritmetica(const double* X, int n);
double calc_media_geometrica(const double* X, int n);
double calc_mediana(const double* Y, int n);
double calc_moda(const double* Y, int n);
double calc_varianza(const double* X, int n, double mu);
double calc_desviacion_std(double varianza);
double calc_coef_variacion(double sigma, double mu);
double calc_cuartil(const double* Y, int n, int i);
double calc_decil(const double* Y, int n, int i);
double calc_percentil(const double* Y, int n, int i);
double calc_momento_no_centrado(const double* X, int n, int orden);
double calc_momento_centrado(const double* X, int n, double mu, int orden);
double calc_curtosis(double mu_4, double sigma);

